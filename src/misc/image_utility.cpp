/*
 * image_utility.cpp
 *
 *  Created on: 2025年12月08日
 *      Author: ZKSWE Develop Team
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <jpeglib.h>
#include <jerror.h>
#include <png.h>
#include <setjmp.h>
#include "base/defer.h"
#include "internal/base_utility.h"
#include "image_utility.h"

using namespace ui;

namespace misc {

#define JPG_MAGIC       0xD8FF
#define BMP_MAGIC       0x4D42

#define BI_RGB          0
#define BI_RLE8         1
#define BI_RLE4         2
#define BI_BITFIELDS    3

#define ALIGN_UP(s, m)      (((s) + (m - 1)) & (~(m - 1)))
#define FREAD_DATA(f, d)    if (fread(&d, sizeof(d), 1, f)) {}

enum EStatusCode {
	E_STATUS_CODE_OK,
	E_STATUS_CODE_FIT,
	E_STATUS_CODE_NOT_MATCH,
	E_STATUS_CODE_READ_ERROR,
	E_STATUS_CODE_WRITE_ERROR,
	E_STATUS_CODE_DECODE_ERROR,
	E_STATUS_CODE_ENCODE_ERROR,
	E_STATUS_CODE_MEM_ERROR,
	E_STATUS_CODE_UNKNOWN_ERROR,
};

struct ImgDriver {
	EImageType type;
	EStatusCode (*load)(ImgDriver *driver, FILE *pf, bitmap_t &bmp, Size *src_size, const Size *max_size, int flag);
	EStatusCode (*save)(ImgDriver *driver, FILE *pf, const bitmap_t &bmp, int flag);
	func_img_decode_hook decode_hook;
	func_img_encode_hook encode_hook;
};

struct BmpFileHeader {
	uint16_t type;
	uint32_t size;
	uint16_t reserved1;
	uint16_t reserved2;
	uint32_t offBits;
} __attribute__((packed));

struct BmpInfoHeader {
	uint32_t size;
	int32_t width;
	int32_t height;
	uint16_t planes;
	uint16_t bitCount;
	uint32_t compression;
	uint32_t sizeImage;
	uint32_t xPelsPerMeter;
	uint32_t yPelsPerMeter;
	uint32_t clrUsed;
	uint32_t clrImportant;
} __attribute__((packed));

struct OS2BmpInfoHeader {
	uint32_t size;
	int16_t width;
	int16_t height;
	uint16_t planes;
	uint16_t bitCount;
} __attribute__((packed));

struct Palette {
	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t a;
};

struct BmpParam {
	int32_t w;
	int32_t h;
	uint32_t pitch;
	uint16_t bitCount;
	uint32_t compression;
	uint32_t gmask;
	bool win;
	Palette *palData;
	uint32_t palCount;
	uint8_t *lineBuffer;
	FILE *pf;
};

struct JpgErrorParam {
	struct jpeg_error_mgr mgr;
	jmp_buf escape;
};

static inline EStatusCode deside_size(uint32_t &target_w, uint32_t &target_h, uint32_t max_w, uint32_t max_h, bool pack) {
	if ((max_w == 0) || (max_h == 0)) {
		return E_STATUS_CODE_OK;
	}

	if (pack) {
		pack_size(target_w, target_h, max_w, max_h);
	} else {
		if ((target_w <= max_w) && (target_h <= max_h)) {
			return E_STATUS_CODE_FIT;
		}

		fit_size(target_w, target_h, max_w, max_h);
	}

	LOGD("target w %d, h %d\n", target_w, target_h);

	return E_STATUS_CODE_OK;
}

static inline void swap_line_rb(uint8_t *line, uint32_t width, uint8_t bpp) {
	uint8_t tmp;
	for (uint32_t i = 0, j = 0; i < width; ++i, j += bpp) {
		tmp = line[j];
		line[j] = line[j + 2];
		line[j + 2] = tmp;
	}
}

static inline void swap_bitmap_rb(bitmap_t &bmp) {
	//LOGD("swap rb start\n");
	uint8_t *line = bmp.data;
	for (uint32_t i = 0; i < bmp.height; ++i, line += bmp.pitch) {
		swap_line_rb(line, bmp.width, bmp.bytes);
	}
	//LOGD("swap rb end\n");
}

static void* load_file_data(FILE *pf, uint32_t &size) {
	fseek(pf, 0, SEEK_END);
	size = ftell(pf);
	fseek(pf, 0, SEEK_SET);

	void *data = malloc(size);
	if (!data) {
		return NULL;
	}

	if (fread(data, 1, size, pf) != size) {
		free(data);
		return NULL;
	}

	return data;
}

/***************************** jpg *****************************/
static inline uint32_t scale_factor(uint32_t src, uint32_t dst) {
	if (dst * 8 <= src) {
		return 8;
	}
	if (dst * 4 <= src) {
		return 4;
	}
	if (dst * 2 <= src) {
		return 2;
	}
	return 0;
}

static EStatusCode jpg_load(ImgDriver *driver, FILE *pf, bitmap_t &bmp, Size *src_size, const Size *max_size, int flag) {
	uint16_t magic = 0;
	FREAD_DATA(pf, magic);
	if (magic != JPG_MAGIC) {
		return E_STATUS_CODE_NOT_MATCH;
	}

	fseek(pf, 0, SEEK_SET);

	struct jpeg_decompress_struct cinfo;
	JpgErrorParam err;
	bool started = false;

	defer {
		// LOGD("jpg load end, started %d\n", started);
		if (started) {
			jpeg_finish_decompress(&cinfo);
		}
		jpeg_destroy_decompress(&cinfo);
	};

	cinfo.err = jpeg_std_error(&err.mgr);
	err.mgr.error_exit = [](j_common_ptr info) {
		longjmp(((JpgErrorParam *) info->err)->escape, -1);
	};

	if (setjmp(err.escape)) {
		LOGE("jpg decode error!\n");
		return E_STATUS_CODE_DECODE_ERROR;
	}

	jpeg_create_decompress(&cinfo);
	jpeg_stdio_src(&cinfo, pf);
	jpeg_read_header(&cinfo, TRUE);

	LOGD("src w %d, h %d\n", cinfo.image_width, cinfo.image_height);

	if (jpeg_has_multiple_scans(&cinfo)) {
		LOGD("is progressive jpg\n");
	}

	if (src_size) {
		src_size->w = cinfo.image_width;
		src_size->h = cinfo.image_height;
	}

	uint32_t target_w = cinfo.image_width;
	uint32_t target_h = cinfo.image_height;
	if (max_size) {
		if (deside_size(target_w, target_h, max_size->w, max_size->h, flag & E_IMAGE_FLAG_PACK) == E_STATUS_CODE_FIT) {
			if (flag & E_IMAGE_FLAG_FIT) {
				return E_STATUS_CODE_FIT;
			}
		}
	}

	cinfo.out_color_space = JCS_RGB;
	cinfo.quantize_colors = FALSE;
	jpeg_calc_output_dimensions(&cinfo);

	uint8_t bpp = cinfo.output_components;
	if (!bitmap_create(bmp, target_w, target_h, bpp)) {
		return E_STATUS_CODE_MEM_ERROR;
	}

	if (driver->decode_hook) {
		long off = ftell(pf);
		uint32_t file_size;
		void *file_data = load_file_data(pf, file_size);
		if (file_data) {
			bool ret = driver->decode_hook(file_data, file_size, Size{cinfo.image_width, cinfo.image_height}, bmp);
			free(file_data);

			if (ret) {
				return E_STATUS_CODE_OK;
			} else {
				fseek(pf, off, SEEK_SET);
			}
		}
	}

	uint32_t factor_w = scale_factor(cinfo.image_width, target_w);
	uint32_t factor_h = scale_factor(cinfo.image_height, target_h);
	uint32_t factor = std::min(factor_w, factor_h);

	if (factor) {
		cinfo.scale_num = 1;
		cinfo.scale_denom = factor;
	}

	jpeg_start_decompress(&cinfo);
	started = true;

	uint32_t output_w = cinfo.output_width;
	uint32_t output_h = cinfo.output_height;

	LOGD("factor %d, output_w %d, output_h %d, bpp %d\n", factor, output_w, output_h, bpp);

	bitmap_scale(bmp, output_w, output_h, output_w * bpp,
			[](int y, uint8_t *buf, void *user_data) {
				JSAMPROW rp[1] = { buf };
				jpeg_read_scanlines((j_decompress_ptr) user_data, rp, 1);
			}, &cinfo);

	if (flag & E_IMAGE_FLAG_BGR) {
		swap_bitmap_rb(bmp);
	}

	return E_STATUS_CODE_OK;
}

static EStatusCode jpg_save(ImgDriver *driver, FILE *pf, const bitmap_t &bmp, int flag) {
	struct jpeg_compress_struct cinfo;
	JpgErrorParam err;
	uint8_t *line_buffer = NULL;
	bool started = false;

	if (driver->encode_hook) {
		uint32_t size;
		void *data = NULL;
		bool ret = driver->encode_hook(bmp, data, size) && (fwrite(data, 1, size, pf) == size);
		free(data);

		if (ret) {
			return E_STATUS_CODE_OK;
		} else {
			fseek(pf, 0, SEEK_SET);
		}
	}

	defer {
		// LOGD("jpg save end, started %d\n", started);
		if (started) {
			jpeg_finish_compress(&cinfo);
		}
		jpeg_destroy_compress(&cinfo);
		free(line_buffer);
	};

	cinfo.err = jpeg_std_error(&err.mgr);
	err.mgr.error_exit = [](j_common_ptr info) {
		longjmp(((JpgErrorParam *) info->err)->escape, -1);
	};

	if (setjmp(err.escape)) {
		LOGE("jpg encode error!\n");
		return E_STATUS_CODE_ENCODE_ERROR;
	}

	jpeg_create_compress(&cinfo);
	jpeg_stdio_dest(&cinfo, pf);

	cinfo.image_width = bmp.width;
	cinfo.image_height = bmp.height;
	cinfo.input_components = bmp.bytes;
	cinfo.in_color_space = JCS_RGB;
	jpeg_set_defaults(&cinfo);

	jpeg_set_quality(&cinfo, 90, TRUE);
	jpeg_start_compress(&cinfo, TRUE);
	started = true;

	if (flag & E_IMAGE_FLAG_BGR) {
		line_buffer = (uint8_t *) malloc(bmp.pitch);
	}

	JSAMPROW row_pointer[1];
	while (cinfo.next_scanline < cinfo.image_height) {
		if (line_buffer) {
			memcpy(line_buffer, bmp.data + cinfo.next_scanline * bmp.pitch, bmp.pitch);
			swap_line_rb(line_buffer, bmp.width, bmp.bytes);
			row_pointer[0] = (JSAMPROW) line_buffer;
		} else {
			row_pointer[0] = (JSAMPROW) (bmp.data + cinfo.next_scanline * bmp.pitch);
		}

		jpeg_write_scanlines(&cinfo, row_pointer, 1);
	}

	return E_STATUS_CODE_OK;
}
/***************************************************************/

/***************************** png *****************************/
static EStatusCode png_load(ImgDriver *driver, FILE *pf, bitmap_t &bmp, Size *src_size, const Size *max_size, int flag) {
	uint8_t sig[8] = { 0 };
	FREAD_DATA(pf, sig);
	if (!png_check_sig(sig, sizeof(sig))) {
		return E_STATUS_CODE_NOT_MATCH;
	}

	fseek(pf, 0, SEEK_SET);

	png_structp pngp = NULL;
	png_infop infop = NULL;

	defer {
		png_destroy_read_struct(&pngp, &infop, NULL);
	};

	pngp = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!pngp) {
		return E_STATUS_CODE_MEM_ERROR;
	}
	infop = png_create_info_struct(pngp);
	if (!infop) {
		return E_STATUS_CODE_MEM_ERROR;
	}

#ifdef PNG_SETJMP_SUPPORTED
	if (setjmp(png_jmpbuf(pngp))) {
		LOGE("png decode error!\n");
		return E_STATUS_CODE_DECODE_ERROR;
	}
#endif

	png_init_io(pngp, pf);
	png_read_info(pngp, infop);

	png_uint_32 width, height;
	int bit_depth, intent, color_type;
	png_get_IHDR(pngp, infop, &width, &height, &bit_depth, &color_type, NULL, NULL, NULL);

	LOGD("src w %d, h %d\n", (int) width, (int) height);

	if (src_size) {
		src_size->w = width;
		src_size->h = height;
	}

	uint32_t target_w = width;
	uint32_t target_h = height;
	if (max_size) {
		if (deside_size(target_w, target_h, max_size->w, max_size->h, flag & E_IMAGE_FLAG_PACK) == E_STATUS_CODE_FIT) {
			if (flag & E_IMAGE_FLAG_FIT) {
				return E_STATUS_CODE_FIT;
			}
		}
	}

	if (bit_depth == 16) {
		png_set_strip_16(pngp);
	} else if (bit_depth < 8) {
		png_set_packing(pngp);
	}

	if ((color_type == PNG_COLOR_TYPE_GRAY) && (bit_depth < 8)) {
#if PNG_LIBPNG_VER >= 10209
		png_set_expand_gray_1_2_4_to_8(pngp);
#else
		png_set_gray_1_2_4_to_8(pngp);
#endif
	}

	if ((color_type == PNG_COLOR_TYPE_GRAY) || (color_type == PNG_COLOR_TYPE_GRAY_ALPHA)) {
		png_set_gray_to_rgb(pngp);
	} else if (color_type == PNG_COLOR_TYPE_PALETTE) {
		png_set_palette_to_rgb(pngp);
	}

	if (png_get_valid(pngp, infop, PNG_INFO_tRNS)) {
		png_set_tRNS_to_alpha(pngp);
	}

	if (flag & E_IMAGE_FLAG_OPAQUE) {
		png_set_strip_alpha(pngp);
	}

	double screen_gamma = 2.2;
	if (png_get_sRGB(pngp, infop, &intent)) {
		png_set_gamma(pngp, screen_gamma, 0.45455);
	} else {
		double image_gamma;
		if (png_get_gAMA(pngp, infop, &image_gamma)) {
			png_set_gamma(pngp, screen_gamma, image_gamma);
		} else {
			png_set_gamma(pngp, screen_gamma, 0.45455);
		}
	}

	int passes = png_set_interlace_handling(pngp);
	png_read_update_info(pngp, infop);

	uint8_t bpp = png_get_channels(pngp, infop);
	uint32_t pitch = png_get_rowbytes(pngp, infop);

	LOGD("passes %d, bpp %d, pitch %d\n", passes, bpp, pitch);

	if (!bitmap_create(bmp, target_w, target_h, bpp)) {
		return E_STATUS_CODE_MEM_ERROR;
	}

	if (passes == 1) {
		bitmap_scale(bmp, width, height, pitch,
				[](int y, uint8_t *buf, void *user_data) {
					png_read_row((png_structp) user_data, buf, NULL);
				}, pngp);
	} else {
		if ((width == bmp.width) && (height == bmp.height)) {
			while (passes--) {
				png_bytep rp = bmp.data;
				for (png_uint_32 i = 0; i < height; ++i, rp += pitch) {
					png_read_row(pngp, rp, NULL);
				}
			}
		} else {
			uint8_t *buffer = (uint8_t *) malloc(pitch * height);
			if (!buffer) {
				return E_STATUS_CODE_MEM_ERROR;
			}

			defer {  // 解码库异常有jmp的情况，这里确保释放
				free(buffer);
			};

			while (passes--) {
				png_bytep rp = buffer;
				for (png_uint_32 i = 0; i < height; ++i, rp += pitch) {
					png_read_row(pngp, rp, NULL);
				}
			}

			bitmap_t src_bmp;
			src_bmp.width = width;
			src_bmp.height = height;
			src_bmp.bytes = bpp;
			src_bmp.pitch = pitch;
			src_bmp.data = buffer;
			bitmap_scale(bmp, src_bmp);
		}
	}

	png_read_end(pngp, infop);

	if (flag & E_IMAGE_FLAG_BGR) {
		swap_bitmap_rb(bmp);
	}

	return E_STATUS_CODE_OK;
}

static EStatusCode png_save(ImgDriver *driver, FILE *pf, const bitmap_t &bmp, int flag) {
	png_structp pngp = NULL;
	png_infop infop = NULL;
	uint8_t *line_buffer = NULL;

	defer {
		png_destroy_write_struct(&pngp, &infop);
		free(line_buffer);
	};

	pngp = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (!pngp) {
		return E_STATUS_CODE_MEM_ERROR;
	}
	infop = png_create_info_struct(pngp);
	if (!infop) {
		return E_STATUS_CODE_MEM_ERROR;
	}

#ifdef PNG_SETJMP_SUPPORTED
	if (setjmp(png_jmpbuf(pngp))) {
		LOGE("png encode error!\n");
		return E_STATUS_CODE_ENCODE_ERROR;
	}
#endif

	png_init_io(pngp, pf);
	png_set_IHDR(pngp, infop, bmp.width, bmp.height, 8,
				(bmp.bytes == 4) ? PNG_COLOR_TYPE_RGBA : PNG_COLOR_TYPE_RGB,
				PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
	png_write_info(pngp, infop);

	if (flag & E_IMAGE_FLAG_BGR) {
		line_buffer = (uint8_t *) malloc(bmp.pitch);
	}

	png_bytep rp = bmp.data;
	for (uint32_t i = 0; i < bmp.height; i++, rp += bmp.pitch) {
		if (line_buffer) {
			memcpy(line_buffer, rp, bmp.pitch);
			swap_line_rb(line_buffer, bmp.width, bmp.bytes);
			png_write_row(pngp, line_buffer);
		} else {
			png_write_row(pngp, rp);
		}
	}

	png_write_end(pngp, infop);

	return E_STATUS_CODE_OK;
}
/***************************************************************/

/***************************** bmp *****************************/
static void read_bmp_palette(FILE *pf, Palette *pal, uint32_t count, bool win) {
	for (uint32_t i = 0; i < count; i++) {
		FREAD_DATA(pf, pal[i].b);
		FREAD_DATA(pf, pal[i].g);
		FREAD_DATA(pf, pal[i].r);
		if (win) {
			FREAD_DATA(pf, pal[i].a);    // not used
		}
	}
}

static void read_bmp_8bit(FILE *pf, uint8_t *line, int width, const Palette *pal, uint32_t count) {
	uint8_t index;
	for (int i = 0; i < width; i++) {
		FREAD_DATA(pf, index);
		if (index >= count) {
			continue;
		}

		line[0] = pal[index].b;
		line[1] = pal[index].g;
		line[2] = pal[index].r;

		line += 3;
	}

	int pad = (width % 4) ? (4 - (width % 4)) : 0;
	for (int i = 0; i < pad; ++i) {
		FREAD_DATA(pf, index);    // skip padding bytes
	}
}

static void read_bmp_16bit(FILE *pf, uint8_t *line, int width, uint32_t gmask) {
	uint16_t pixel;
	for (int i = 0; i < width; i++) {
		FREAD_DATA(pf, pixel);
		if (gmask == 0x03e0) { /* 5-5-5 */
			line[0] = (pixel & 0x1f) << 3;
			line[1] = ((pixel >> 5) & 0x1f) << 3;
			line[2] = ((pixel >> 10) & 0x1f) << 3;
		} else { /* 5-6-5 */
			line[0] = (pixel & 0x1f) << 3;
			line[1] = ((pixel >> 5) & 0x3f) << 2;
			line[2] = ((pixel >> 11) & 0x1f) << 3;
		}

		line += 3;
	}

	if (width & 0x01) {
		FREAD_DATA(pf, pixel);	// read the gap
	}
}

static EStatusCode bmp_load(ImgDriver *driver, FILE *pf, bitmap_t &bmp, Size *src_size, const Size *max_size, int flag) {
	BmpFileHeader bf = { 0 };
	FREAD_DATA(pf, bf);
	if (bf.type != BMP_MAGIC) {
		return E_STATUS_CODE_NOT_MATCH;
	}

	BmpParam param = { 0 };
	uint32_t biSize = 0;
	FREAD_DATA(pf, biSize);

	if (biSize >= sizeof(BmpInfoHeader)) {
		BmpInfoHeader bi = { 0 };
		if (fread(&bi.width, sizeof(BmpInfoHeader) - 4, 1, pf) != 1) {
			return E_STATUS_CODE_READ_ERROR;
		}
		fseek(pf, biSize - sizeof(BmpInfoHeader), SEEK_CUR);

		param.w = bi.width;
		param.h = bi.height;
		param.bitCount = bi.bitCount;
		param.compression = bi.compression;
		param.win = true;
	} else if (biSize == sizeof(OS2BmpInfoHeader)) {
		OS2BmpInfoHeader bi = { 0 };
		if (fread(&bi.width, sizeof(OS2BmpInfoHeader) - 4, 1, pf) != 1) {
			return E_STATUS_CODE_READ_ERROR;
		}

		param.w = bi.width;
		param.h = bi.height;
		param.bitCount = bi.bitCount;
	} else {
		return E_STATUS_CODE_UNKNOWN_ERROR;
	}

	LOGD("src w %d, h %d\n", param.w, param.h);

	if (param.h < 0) {  // 正表示bottom-up, 负表示up-bottom
		param.h = -param.h;
		if (flag & E_IMAGE_FLAG_UP_TO_BOTTOM) {
			flag &= ~E_IMAGE_FLAG_UP_TO_BOTTOM;
		} else {
			flag |= E_IMAGE_FLAG_UP_TO_BOTTOM;
		}
	}

	if (src_size) {
		src_size->w = param.w;
		src_size->h = param.h;
	}

	uint32_t target_w = param.w;
	uint32_t target_h = param.h;
	if (max_size) {
		if (deside_size(target_w, target_h, max_size->w, max_size->h, flag & E_IMAGE_FLAG_PACK) == E_STATUS_CODE_FIT) {
			if (flag & E_IMAGE_FLAG_FIT) {
				return E_STATUS_CODE_FIT;
			}
		}
	}

	uint32_t rmask = 0x001F, gmask = 0x03E0, bmask = 0x7C00;
	switch (param.compression) {
	case BI_BITFIELDS:
		FREAD_DATA(pf, rmask);
		FREAD_DATA(pf, gmask);
		FREAD_DATA(pf, bmask);
		break;

	case BI_RGB:
	case BI_RLE8:
	case BI_RLE4:
		break;

	default:
		return E_STATUS_CODE_UNKNOWN_ERROR;
	}

	uint8_t bpp = (param.bitCount <= 24) ? 3 : 4;
	uint8_t target_bpp = (flag & E_IMAGE_FLAG_OPAQUE) ? 3 : bpp;
	if (!bitmap_create(bmp, target_w, target_h, target_bpp)) {
		return E_STATUS_CODE_MEM_ERROR;
	}

	if (param.bitCount <= 8) {
		param.palCount = param.win ? ((bf.offBits - sizeof(BmpFileHeader) - biSize) / 4) :
									((bf.offBits - sizeof(BmpFileHeader) - sizeof(OS2BmpInfoHeader)) / 3);
		param.palData = (Palette *) malloc(param.palCount * sizeof(Palette));
		read_bmp_palette(pf, param.palData, param.palCount, param.win);
	}

	param.pitch = ALIGN_UP(param.w * bpp, 4);
	param.gmask = gmask;
	param.pf = pf;

	uint32_t src_pitch = param.pitch;
	if (bpp != target_bpp) {  // 32bit转24bit
		param.lineBuffer = (uint8_t *) malloc(param.pitch);
		src_pitch = param.w * target_bpp;
	}

	bitmap_scale(bmp, param.w, param.h, src_pitch,
			[](int y, uint8_t *buf, void *user_data) {
				BmpParam *param = (BmpParam *) user_data;
				switch (param->compression) {
				case BI_BITFIELDS:
				case BI_RGB:
					switch (param->bitCount) {
					case 1:
					case 4:  // not implemented
						break;

					case 8:
						read_bmp_8bit(param->pf, buf, param->w, param->palData, param->palCount);
						break;

					case 16:
						read_bmp_16bit(param->pf, buf, param->w, param->gmask);
						break;

					case 24:
					case 32:
						if (fread(param->lineBuffer ? param->lineBuffer : buf, 1, param->pitch, param->pf)) {}
						if (param->lineBuffer) {
							for (int i = 0, j = 0, k = 0; i < param->w; ++i, j += 3, k += 4) {
								buf[j + 0] = param->lineBuffer[k + 0];
								buf[j + 1] = param->lineBuffer[k + 1];
								buf[j + 2] = param->lineBuffer[k + 2];
							}
						}
						break;
					}
					break;

				case BI_RLE4:  // not implemented
				case BI_RLE8:
					break;
				}
			}, &param);

	if (flag & E_IMAGE_FLAG_UP_TO_BOTTOM) {
		LOGD("swap bmp line data, make it up to bottom\n");
		uint8_t *tmp_line = (uint8_t *) malloc(bmp.pitch);
		for (uint32_t i = 0; i < bmp.height / 2; ++i) {
			uint8_t *up_line = bmp.data + i * bmp.pitch;
			uint8_t *bottom_line = bmp.data + (bmp.height - i - 1) * bmp.pitch;
			memcpy(tmp_line, up_line, bmp.pitch);
			memcpy(up_line, bottom_line, bmp.pitch);
			memcpy(bottom_line, tmp_line, bmp.pitch);
		}
		free(tmp_line);
	}

	free(param.palData);
	free(param.lineBuffer);

	return E_STATUS_CODE_OK;
}

static EStatusCode bmp_save(ImgDriver *driver, FILE *pf, const bitmap_t &bmp, int flag) {
	uint32_t bfSize = sizeof(BmpFileHeader);
	uint32_t biSize = sizeof(BmpInfoHeader);
	uint32_t pitch = ALIGN_UP(bmp.pitch, 4);

	BmpFileHeader bf = { 0 };
	BmpInfoHeader bi = { 0 };

	bi.size = biSize;
	bi.width = bmp.width;
	bi.height = bmp.height;
	bi.planes = 1;
	bi.bitCount = bmp.bytes << 3;
	bi.sizeImage = pitch * bmp.height;

	bf.type = BMP_MAGIC;
	bf.offBits = bfSize + biSize;
	bf.size = bf.offBits + bi.sizeImage;

	if ((fwrite(&bf, 1, bfSize, pf) != bfSize) || (fwrite(&bi, 1, biSize, pf) != biSize)) {
		LOGE("write bmp header error!\n");
		return E_STATUS_CODE_WRITE_ERROR;
	}

	const uint8_t *line = bmp.data;
	int stride = bmp.pitch;

	if (flag & E_IMAGE_FLAG_UP_TO_BOTTOM) {
		line += (bmp.height - 1) * bmp.pitch;
		stride = -bmp.pitch;
	}

	uint32_t alignment = 0, alignSize = pitch - bmp.pitch;
	for (uint32_t i = 0; i < bmp.height; ++i, line += stride) {
		if (fwrite(line, 1, bmp.pitch, pf) != bmp.pitch) {
			LOGE("write bmp data error!\n");
			return E_STATUS_CODE_WRITE_ERROR;
		}
		if (alignSize && (fwrite(&alignment, 1, alignSize, pf) != alignSize)) {
			LOGE("write align data error!\n");
			return E_STATUS_CODE_WRITE_ERROR;
		}
	}

	return E_STATUS_CODE_OK;
}
/***************************************************************/

static ImgDriver img_drivers[] = {
	{ E_IMAGE_TYPE_JPG, jpg_load, jpg_save, NULL, NULL },
	{ E_IMAGE_TYPE_PNG, png_load, png_save, NULL, NULL },
	{ E_IMAGE_TYPE_BMP, bmp_load, bmp_save, NULL, NULL },
};

static bool copy_file(FILE *src_pf, FILE *dst_pf) {
	fseek(src_pf, 0, SEEK_END);
	int file_size = ftell(src_pf);
	fseek(src_pf, 0, SEEK_SET);

	if (file_size <= 0) {
		LOGE("unknown file size\n");
		return false;
	}

	int write_size = 0;
	int buf_size = 1024 * 1024;
	uint8_t *buf = (uint8_t *) malloc(buf_size);

	while (write_size < file_size) {
		size_t size = fread(buf, 1, buf_size, src_pf);
		if (size <= 0) {
			LOGE("read err %s\n", strerror(errno));
			break;
		}

		if (fwrite(buf, 1, size, dst_pf) != size) {
			LOGE("write err %s\n", strerror(errno));
			break;
		}

		write_size += size;
	}

	free(buf);

	return (file_size == write_size) ? true : false;
}

static bool copy_file(const std::string &src_file, const std::string &dst_file) {
	FILE *src_pf = fopen(src_file.c_str(), "rb");
	if (!src_pf) {
		return false;
	}

	FILE *dst_pf = fopen(dst_file.c_str(), "wb");
	if (!dst_pf) {
		fclose(src_pf);
		return false;
	}

	bool ret = copy_file(src_pf, dst_pf);
	fclose(src_pf);

	if (ret) {
		fflush(dst_pf);
		fclose(dst_pf);
	} else {
		fclose(dst_pf);
		remove(dst_file.c_str());
	}

	return ret;
}

bool image_load(const std::string &img, bitmap_t &bmp, Size *src_size, const Size *max_size, int flag) {
	FILE *img_pf = fopen(img.c_str(), "rb");
	if (!img_pf) {
		LOGE("fopen %s fail, %s\n", img.c_str(), strerror(errno));
		return false;
	}

	memset(&bmp, 0, sizeof(bitmap_t));
	flag |= E_IMAGE_FLAG_BGR | E_IMAGE_FLAG_UP_TO_BOTTOM;

	EStatusCode status;
	ImgDriver *driver = img_drivers;

	for (int i = 0; i < array_length(img_drivers); ++i, ++driver) {
		fseek(img_pf, 0, SEEK_SET);

		status = driver->load(driver, img_pf, bmp, src_size, max_size, flag);
		if (status != E_STATUS_CODE_NOT_MATCH) {
			break;
		}
	}

	fclose(img_pf);

	if (status != E_STATUS_CODE_OK) {
		bitmap_destroy(bmp);
		return false;
	}

	return true;
}

bool image_save(const std::string &img, const bitmap_t &bmp, EImageType type) {
	FILE *img_pf = fopen(img.c_str(), "wb");
	if (!img_pf) {
		LOGE("fopen %s fail, %s\n", img.c_str(), strerror(errno));
		return false;
	}

	ImgDriver *driver = img_drivers + type;
	if (driver->save(driver, img_pf, bmp, E_IMAGE_FLAG_BGR | E_IMAGE_FLAG_UP_TO_BOTTOM) != E_STATUS_CODE_OK) {
		fclose(img_pf);
		remove(img.c_str());
		return false;
	}

	fflush(img_pf);
	fclose(img_pf);

	return true;
}

bool image_resize(const std::string &src, const std::string &dst, const Size &max_size) {
	FILE *src_pf = fopen(src.c_str(), "rb");
	if (!src_pf) {
		LOGE("fopen %s fail, %s\n", src.c_str(), strerror(errno));
		return false;
	}

	bool ret = false;
	bitmap_t bmp = { 0 };
	ImgDriver *driver = img_drivers;

	for (int i = 0; i < array_length(img_drivers); ++i, ++driver) {
		fseek(src_pf, 0, SEEK_SET);

		EStatusCode status = driver->load(driver, src_pf, bmp, NULL, &max_size, E_IMAGE_FLAG_FIT);
		if (status == E_STATUS_CODE_NOT_MATCH) {
			continue;
		}

		if ((status == E_STATUS_CODE_OK) || (status == E_STATUS_CODE_FIT)) {
			FILE *dst_pf = fopen(dst.c_str(), "wb");
			if (!dst_pf) {
				LOGE("fopen %s fail, %s\n", dst.c_str(), strerror(errno));
				break;
			}

			if (status == E_STATUS_CODE_OK) {
				ret = (driver->save(driver, dst_pf, bmp, 0) == E_STATUS_CODE_OK);
			} else {
				ret = copy_file(src_pf, dst_pf);
			}

			if (ret) {
				fflush(dst_pf);
				fclose(dst_pf);
			} else {
				fclose(dst_pf);
				remove(dst.c_str());
			}
		}

		break;
	}

	bitmap_destroy(bmp);
	fclose(src_pf);

	return ret;
}

bool image_fit_bitmap(const std::string &img, bitmap_t &bmp, EImageBgMode mode, Region *fit_rgn) {
	if (bmp.bytes != 3) {
		LOGD("image_fit_bitmap only supported 3bpp\n");
		return false;
	}

	bitmap_t src_bmp = { 0 };
	Size max_size = { bmp.width, bmp.height };
	if (!image_load(img, src_bmp, NULL, &max_size)) {
		return false;
	}

	bitmap_fit(bmp, src_bmp, mode, fit_rgn);
	bitmap_destroy(src_bmp);

	return true;
}

bool image_fill_bitmap(const std::string &img, bitmap_t &bmp, const Region *rgn) {
	if (bmp.bytes != 3) {
		LOGD("image_fill_bitmap only supported 3bpp\n");
		return false;
	}

	bitmap_t src_bmp = { 0 };
	Size max_size = { bmp.width, bmp.height };
	if (!image_load(img, src_bmp, NULL, &max_size, E_IMAGE_FLAG_OPAQUE | E_IMAGE_FLAG_PACK)) {
		return false;
	}

	Region src_rgn = { 0, 0, (uint16_t) src_bmp.width, (uint16_t) src_bmp.height };
	if (!rgn || REGION_IS_EMPTY(*rgn)) {
		uint32_t fit_w = bmp.width;
		uint32_t fit_h = bmp.height;
		fit_size(fit_w, fit_h, src_bmp.width, src_bmp.height);
		src_rgn.left = (src_bmp.width - fit_w) / 2;
		src_rgn.top = (src_bmp.height - fit_h) / 2;
		src_rgn.right = src_rgn.left + fit_w;
		src_rgn.bottom = src_rgn.top + fit_h;
	} else if (!RegionIntersect(src_rgn, src_rgn, *rgn)) {
		bitmap_destroy(src_bmp);
		return false;
	}

	bitmap_fill(bmp, src_bmp, &src_rgn);
	bitmap_destroy(src_bmp);

	return true;
}

bool image_rotate(const std::string &dst_img, const std::string &src_img, ui::ERot rot) {
	if (rot == E_ROT_0) {
		if (dst_img == src_img) {
			return true;
		}

		return copy_file(src_img, dst_img);
	}

	bitmap_t src_bmp;
	if (!image_load(src_img, src_bmp)) {
		return false;
	}

	uint32_t w = src_bmp.width;
	uint32_t h = src_bmp.height;
	if (rot != E_ROT_180) {
		w = src_bmp.height;
		h = src_bmp.width;
	}

	bool ret = false;

	bitmap_t dst_bmp;
	if (bitmap_create(dst_bmp, w, h, src_bmp.bytes)) {
		bitmap_rotate(dst_bmp, src_bmp, rot);
		ret = image_save(dst_img, dst_bmp);
		bitmap_destroy(dst_bmp);
	}

	bitmap_destroy(src_bmp);

	return ret;
}

void image_set_decode_hook(ui::EImageType type, func_img_decode_hook decode) {
	for (int i = 0; i < array_length(img_drivers); ++i) {
		if (img_drivers[i].type == type) {
			img_drivers[i].decode_hook = decode;
			return;
		}
	}
}

void image_set_encode_hook(ui::EImageType type, func_img_encode_hook encode) {
	for (int i = 0; i < array_length(img_drivers); ++i) {
		if (img_drivers[i].type == type) {
			img_drivers[i].encode_hook = encode;
			return;
		}
	}
}

}
