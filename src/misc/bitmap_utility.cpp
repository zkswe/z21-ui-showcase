/*
 * bitmap_utility.cpp
 *
 *  Created on: 2025年12月22日
 *      Author: ZKSWE Develop Team
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "base/defer.h"
#include "internal/base_utility.h"
#include "bitmap_utility.h"

using namespace ui;

namespace misc {

static inline void scale_line(uint8_t *dst_line, const uint8_t *prev_line, const uint8_t *next_line,
		int sw, int dw, int dh, int bpp, int n) {
	int dsize = dw * dh;
	int hsize = dsize / 2;
	int dwn = dw * n;
	int x, b, bn, dhb, dsbn;
	const uint8_t *pA, *pB, *pC, *pD;

	for (int i = 0, sp = 0; i <= dw; ++i, sp += sw) {
		x = sp / dw * bpp;
		b = dw - sp % dw;
		pA = prev_line + x;
		pB = pA + bpp;
		pC = next_line + x;
		pD = pC + bpp;
		if (b == dw) {
			pB = pA;
			pD = pC;
		}

		bn = b * n;
		dhb = dh * b;
		dsbn = dsize - dhb - dwn;
		for (int j = 0; j < bpp; ++j) {
			*dst_line++ = (uint8_t) (int) ((bn * ((*pA++) - *pB - *pC + *pD)
					+ dwn * (*pB++) + dhb * (*pC++)
					+ dsbn * (*pD++) + hsize) / dsize);
		}
	}
}

static void cal_gaussian_coeff(float sigma, float *a0, float *a1, float *a2,
		float *a3, float *b1, float *b2, float *cprev, float *cnext) {
	float alpha, lamma, k;

	if (sigma < 0.5f) {
		sigma = 0.5f;
	}
	alpha = (float) exp((0.726) * (0.726)) / sigma;
	lamma = (float) exp(-alpha);
	*b2 = (float) exp(-2 * alpha);
	k = (1 - lamma) * (1 - lamma) / (1 + 2 * alpha * lamma - (*b2));
	*a0 = k;
	*a1 = k * (alpha - 1) * lamma;
	*a2 = k * (alpha + 1) * lamma;
	*a3 = -k * (*b2);
	*b1 = -2 * lamma;
	*cprev = (*a0 + *a1) / (1 + *b1 + *b2);
	*cnext = (*a2 + *a3) / (1 + *b1 + *b2);
}

static void gaussian_horizontal(uint8_t *bufferPerLine,
		uint8_t *lpRowInitial, uint8_t *lpColumn, int width,
		int height, int Channels, int Nwidth, float a0a1, float a2a3,
		float b1b2, float cprev, float cnext) {
	int HeightStep = Channels * height;
	int WidthSubOne = width - 1;
	if (Channels == 3) {
		float prevOut[3];
		prevOut[0] = (lpRowInitial[0] * cprev);
		prevOut[1] = (lpRowInitial[1] * cprev);
		prevOut[2] = (lpRowInitial[2] * cprev);
		for (int x = 0; x < width; ++x) {
			prevOut[0] = ((lpRowInitial[0] * (a0a1)) - (prevOut[0] * (b1b2)));
			prevOut[1] = ((lpRowInitial[1] * (a0a1)) - (prevOut[1] * (b1b2)));
			prevOut[2] = ((lpRowInitial[2] * (a0a1)) - (prevOut[2] * (b1b2)));
			bufferPerLine[0] = prevOut[0];
			bufferPerLine[1] = prevOut[1];
			bufferPerLine[2] = prevOut[2];
			bufferPerLine += Channels;
			lpRowInitial += Channels;
		}
		lpRowInitial -= Channels;
		lpColumn += HeightStep * WidthSubOne;
		bufferPerLine -= Channels;
		prevOut[0] = (lpRowInitial[0] * cnext);
		prevOut[1] = (lpRowInitial[1] * cnext);
		prevOut[2] = (lpRowInitial[2] * cnext);

		for (int x = WidthSubOne; x >= 0; --x) {
			prevOut[0] = ((lpRowInitial[0] * (a2a3)) - (prevOut[0] * (b1b2)));
			prevOut[1] = ((lpRowInitial[1] * (a2a3)) - (prevOut[1] * (b1b2)));
			prevOut[2] = ((lpRowInitial[2] * (a2a3)) - (prevOut[2] * (b1b2)));
			bufferPerLine[0] += prevOut[0];
			bufferPerLine[1] += prevOut[1];
			bufferPerLine[2] += prevOut[2];
			lpColumn[0] = bufferPerLine[0];
			lpColumn[1] = bufferPerLine[1];
			lpColumn[2] = bufferPerLine[2];
			lpRowInitial -= Channels;
			lpColumn -= HeightStep;
			bufferPerLine -= Channels;
		}
	} else if (Channels == 4) {
		float prevOut[4];

		prevOut[0] = (lpRowInitial[0] * cprev);
		prevOut[1] = (lpRowInitial[1] * cprev);
		prevOut[2] = (lpRowInitial[2] * cprev);
		prevOut[3] = (lpRowInitial[3] * cprev);
		for (int x = 0; x < width; ++x) {
			prevOut[0] = ((lpRowInitial[0] * (a0a1)) - (prevOut[0] * (b1b2)));
			prevOut[1] = ((lpRowInitial[1] * (a0a1)) - (prevOut[1] * (b1b2)));
			prevOut[2] = ((lpRowInitial[2] * (a0a1)) - (prevOut[2] * (b1b2)));
			prevOut[3] = ((lpRowInitial[3] * (a0a1)) - (prevOut[3] * (b1b2)));

			bufferPerLine[0] = prevOut[0];
			bufferPerLine[1] = prevOut[1];
			bufferPerLine[2] = prevOut[2];
			bufferPerLine[3] = prevOut[3];
			bufferPerLine += Channels;
			lpRowInitial += Channels;
		}
		lpRowInitial -= Channels;
		lpColumn += HeightStep * WidthSubOne;
		bufferPerLine -= Channels;

		prevOut[0] = (lpRowInitial[0] * cnext);
		prevOut[1] = (lpRowInitial[1] * cnext);
		prevOut[2] = (lpRowInitial[2] * cnext);
		prevOut[3] = (lpRowInitial[3] * cnext);

		for (int x = WidthSubOne; x >= 0; --x) {
			prevOut[0] = ((lpRowInitial[0] * a2a3) - (prevOut[0] * b1b2));
			prevOut[1] = ((lpRowInitial[1] * a2a3) - (prevOut[1] * b1b2));
			prevOut[2] = ((lpRowInitial[2] * a2a3) - (prevOut[2] * b1b2));
			prevOut[3] = ((lpRowInitial[3] * a2a3) - (prevOut[3] * b1b2));
			bufferPerLine[0] += prevOut[0];
			bufferPerLine[1] += prevOut[1];
			bufferPerLine[2] += prevOut[2];
			bufferPerLine[3] += prevOut[3];
			lpColumn[0] = bufferPerLine[0];
			lpColumn[1] = bufferPerLine[1];
			lpColumn[2] = bufferPerLine[2];
			lpColumn[3] = bufferPerLine[3];
			lpRowInitial -= Channels;
			lpColumn -= HeightStep;
			bufferPerLine -= Channels;
		}
	} else if (Channels == 1) {
		float prevOut = (lpRowInitial[0] * cprev);

		for (int x = 0; x < width; ++x) {
			prevOut = ((lpRowInitial[0] * (a0a1)) - (prevOut * (b1b2)));
			bufferPerLine[0] = prevOut;
			bufferPerLine += Channels;
			lpRowInitial += Channels;
		}
		lpRowInitial -= Channels;
		lpColumn += HeightStep * WidthSubOne;
		bufferPerLine -= Channels;

		prevOut = (lpRowInitial[0] * cnext);

		for (int x = WidthSubOne; x >= 0; --x) {
			prevOut = ((lpRowInitial[0] * a2a3) - (prevOut * b1b2));
			bufferPerLine[0] += prevOut;
			lpColumn[0] = bufferPerLine[0];
			lpRowInitial -= Channels;
			lpColumn -= HeightStep;
			bufferPerLine -= Channels;
		}
	}
}

static void gaussian_vertical(uint8_t *bufferPerLine,
		uint8_t *lpRowInitial, uint8_t *lpColInitial, int height,
		int width, int Channels, float a0a1, float a2a3, float b1b2,
		float cprev, float cnext) {
	int WidthStep = Channels * width;
	int HeightSubOne = height - 1;
	if (Channels == 3) {
		float prevOut[3];
		prevOut[0] = (lpRowInitial[0] * cprev);
		prevOut[1] = (lpRowInitial[1] * cprev);
		prevOut[2] = (lpRowInitial[2] * cprev);

		for (int y = 0; y < height; y++) {
			prevOut[0] = ((lpRowInitial[0] * a0a1) - (prevOut[0] * b1b2));
			prevOut[1] = ((lpRowInitial[1] * a0a1) - (prevOut[1] * b1b2));
			prevOut[2] = ((lpRowInitial[2] * a0a1) - (prevOut[2] * b1b2));
			bufferPerLine[0] = prevOut[0];
			bufferPerLine[1] = prevOut[1];
			bufferPerLine[2] = prevOut[2];
			bufferPerLine += Channels;
			lpRowInitial += Channels;
		}
		lpRowInitial -= Channels;
		bufferPerLine -= Channels;
		lpColInitial += WidthStep * HeightSubOne;
		prevOut[0] = (lpRowInitial[0] * cnext);
		prevOut[1] = (lpRowInitial[1] * cnext);
		prevOut[2] = (lpRowInitial[2] * cnext);
		for (int y = HeightSubOne; y >= 0; y--) {
			prevOut[0] = ((lpRowInitial[0] * a2a3) - (prevOut[0] * b1b2));
			prevOut[1] = ((lpRowInitial[1] * a2a3) - (prevOut[1] * b1b2));
			prevOut[2] = ((lpRowInitial[2] * a2a3) - (prevOut[2] * b1b2));
			bufferPerLine[0] += prevOut[0];
			bufferPerLine[1] += prevOut[1];
			bufferPerLine[2] += prevOut[2];
			lpColInitial[0] = bufferPerLine[0];
			lpColInitial[1] = bufferPerLine[1];
			lpColInitial[2] = bufferPerLine[2];
			lpRowInitial -= Channels;
			lpColInitial -= WidthStep;
			bufferPerLine -= Channels;
		}
	} else if (Channels == 4) {
		float prevOut[4];

		prevOut[0] = (lpRowInitial[0] * cprev);
		prevOut[1] = (lpRowInitial[1] * cprev);
		prevOut[2] = (lpRowInitial[2] * cprev);
		prevOut[3] = (lpRowInitial[3] * cprev);

		for (int y = 0; y < height; y++) {
			prevOut[0] = ((lpRowInitial[0] * a0a1) - (prevOut[0] * b1b2));
			prevOut[1] = ((lpRowInitial[1] * a0a1) - (prevOut[1] * b1b2));
			prevOut[2] = ((lpRowInitial[2] * a0a1) - (prevOut[2] * b1b2));
			prevOut[3] = ((lpRowInitial[3] * a0a1) - (prevOut[3] * b1b2));
			bufferPerLine[0] = prevOut[0];
			bufferPerLine[1] = prevOut[1];
			bufferPerLine[2] = prevOut[2];
			bufferPerLine[3] = prevOut[3];
			bufferPerLine += Channels;
			lpRowInitial += Channels;
		}
		lpRowInitial -= Channels;
		bufferPerLine -= Channels;
		lpColInitial += WidthStep * HeightSubOne;
		prevOut[0] = (lpRowInitial[0] * cnext);
		prevOut[1] = (lpRowInitial[1] * cnext);
		prevOut[2] = (lpRowInitial[2] * cnext);
		prevOut[3] = (lpRowInitial[3] * cnext);
		for (int y = HeightSubOne; y >= 0; y--) {
			prevOut[0] = ((lpRowInitial[0] * a2a3) - (prevOut[0] * b1b2));
			prevOut[1] = ((lpRowInitial[1] * a2a3) - (prevOut[1] * b1b2));
			prevOut[2] = ((lpRowInitial[2] * a2a3) - (prevOut[2] * b1b2));
			prevOut[3] = ((lpRowInitial[3] * a2a3) - (prevOut[3] * b1b2));
			bufferPerLine[0] += prevOut[0];
			bufferPerLine[1] += prevOut[1];
			bufferPerLine[2] += prevOut[2];
			bufferPerLine[3] += prevOut[3];
			lpColInitial[0] = bufferPerLine[0];
			lpColInitial[1] = bufferPerLine[1];
			lpColInitial[2] = bufferPerLine[2];
			lpColInitial[3] = bufferPerLine[3];
			lpRowInitial -= Channels;
			lpColInitial -= WidthStep;
			bufferPerLine -= Channels;
		}
	} else if (Channels == 1) {
		float prevOut = 0;
		prevOut = (lpRowInitial[0] * cprev);
		for (int y = 0; y < height; y++) {
			prevOut = ((lpRowInitial[0] * a0a1) - (prevOut * b1b2));
			bufferPerLine[0] = prevOut;
			bufferPerLine += Channels;
			lpRowInitial += Channels;
		}
		lpRowInitial -= Channels;
		bufferPerLine -= Channels;
		lpColInitial += WidthStep * HeightSubOne;
		prevOut = (lpRowInitial[0] * cnext);
		for (int y = HeightSubOne; y >= 0; y--) {
			prevOut = ((lpRowInitial[0] * a2a3) - (prevOut * b1b2));
			bufferPerLine[0] += prevOut;
			lpColInitial[0] = bufferPerLine[0];
			lpRowInitial -= Channels;
			lpColInitial -= WidthStep;
			bufferPerLine -= Channels;
		}
	}
}

bool bitmap_create(bitmap_t &bmp, uint32_t w, uint32_t h, uint8_t bpp) {
	memset(&bmp, 0, sizeof(bitmap_t));
	bmp.width = w;
	bmp.height = h;
	bmp.bytes = bpp;
	bmp.pitch = w * bpp;
	bmp.data = (uint8_t *) calloc(1, bmp.pitch * bmp.height);
	return bmp.data != NULL;
}

void bitmap_destroy(bitmap_t &bmp) {
	if (bmp.data) {
		free(bmp.data);
		bmp.data = NULL;
	}
}

void bitmap_copy(bitmap_t &dst_bmp, const bitmap_t &src_bmp, const ui::Region *rgn) {
	if (dst_bmp.bytes != src_bmp.bytes) {
		return;
	}

	if ((dst_bmp.pitch == src_bmp.pitch) &&
		(dst_bmp.width == src_bmp.width) &&
		(dst_bmp.height == src_bmp.height) &&
		!rgn) {
		memcpy(dst_bmp.data, src_bmp.data, src_bmp.pitch * src_bmp.height);
		return;
	}

	uint32_t w = std::min(dst_bmp.width, src_bmp.width);
	uint32_t h = std::min(dst_bmp.height, src_bmp.height);
	uint8_t *src_line = src_bmp.data;
	uint8_t *dst_line = dst_bmp.data;
	uint32_t pitch = w * src_bmp.bytes;

	if (rgn) {
		uint32_t rw = REGION_WIDTH(*rgn);
		uint32_t rh = REGION_HEIGHT(*rgn);

		if (rw < w) {
			w = rw;
		}
		if (rh < h) {
			h = rh;
		}

		src_line += BITMAP_OFFSET(src_bmp, rgn->left, rgn->top);
		dst_line += BITMAP_OFFSET(dst_bmp, rgn->left, rgn->top);
		pitch = w * src_bmp.bytes;
	}

	for (uint32_t i = 0; i < h; ++i) {
		memcpy(dst_line, src_line, pitch);
		src_line += src_bmp.pitch;
		dst_line += dst_bmp.pitch;
	}
}

bool bitmap_cmp(const bitmap_t &dst_bmp, const bitmap_t &src_bmp) {
	if ((dst_bmp.bytes != src_bmp.bytes) ||
		(dst_bmp.width != src_bmp.width) ||
		(dst_bmp.height != src_bmp.height)) {
		return false;
	}

	if (dst_bmp.pitch == src_bmp.pitch) {
		return (memcmp(dst_bmp.data, src_bmp.data, src_bmp.pitch * src_bmp.height) == 0);
	}

	const uint8_t *src_line = src_bmp.data;
	const uint8_t *dst_line = dst_bmp.data;
	uint32_t pitch = src_bmp.width * src_bmp.bytes;

	for (uint32_t i = 0; i < src_bmp.height; ++i) {
		if (memcmp(dst_line, src_line, pitch) != 0) {
			return false;
		}

		src_line += src_bmp.pitch;
		dst_line += dst_bmp.pitch;
	}

	return true;
}

bool bitmap_copy_and_cmp(bitmap_t &dst_bmp, const bitmap_t &src_bmp) {
	bitmap_copy(dst_bmp, src_bmp);
	return bitmap_cmp(dst_bmp, src_bmp);
}

void bitmap_scale(bitmap_t &dst_bmp, const bitmap_t &src_bmp) {
	bitmap_scale(dst_bmp, src_bmp.width, src_bmp.height, src_bmp.pitch,
			[] (int y, uint8_t *buf, void *user_data) {
				const bitmap_t *bmp = (const bitmap_t *) user_data;
				memcpy(buf, bmp->data + bmp->pitch * y, bmp->width * bmp->bytes);
			}, (void *) &src_bmp);
}

void bitmap_scale(bitmap_t &dst_bmp, uint32_t src_w, uint32_t src_h, uint32_t src_pitch,
		void (*read_line)(int y, uint8_t *buf, void *user_data), void *user_data) {
	int sw = src_w - 1, sh = src_h - 1, dw = dst_bmp.width - 1, dh = dst_bmp.height - 1;
	uint8_t *dst_line = dst_bmp.data;

	if ((sw == dw) && (sh == dh)) {
		for (int y = 0; y <= dh; ++y, dst_line += dst_bmp.pitch) {
			read_line(y, dst_line, user_data);
		}
		return;
	}

	uint8_t *prev_line = (uint8_t *) malloc(src_pitch);
	uint8_t *next_line = (uint8_t *) malloc(src_pitch);

	defer {  // 解码库异常有jmp的情况，这里确保释放
		free(prev_line);
		free(next_line);
	};

	for (int i = 0, y = 0, sy, n; i <= dh;) {
		sy = i * sh / dh;
		n = dh - i * sh % dh;

		if (y - 2 != sy) {
			if (y - 1 == sy) {
				memcpy(prev_line, next_line, src_pitch);
			} else {
				read_line(y, prev_line, user_data);
				if (y++ != sy) {
					continue;
				}
			}

			if (n != dh) {
				read_line(y++, next_line, user_data);
			} else {
				memcpy(next_line, prev_line, src_pitch);
			}
		}

		scale_line(dst_line, prev_line, next_line, sw, dw, dh, dst_bmp.bytes, n);

		++i;
		dst_line += dst_bmp.pitch;
	}
}

void bitmap_rotate(bitmap_t &dst_bmp, const bitmap_t &src_bmp, ERot rot) {
	if (dst_bmp.bytes != src_bmp.bytes) {
		return;
	}

	int dw = dst_bmp.width;
	int dh = dst_bmp.height;
	int dp = dst_bmp.pitch;
	int sw = src_bmp.width;
	int sh = src_bmp.height;
	int sp = src_bmp.pitch;
	int bpp = src_bmp.bytes;

	switch (rot) {
	case E_ROT_0:
		bitmap_copy(dst_bmp, src_bmp);
		break;

	case E_ROT_90:
		if ((dw != sh) || (dh != sw)) {
			break;
		}

		if (bpp == 4) {
			uint32_t *src = (uint32_t *) src_bmp.data;
			uint32_t *dst = (uint32_t *) dst_bmp.data;

			for (int i = 0; i < sw; i++) {
				for (int j = sh - 1, k = 0; j >= 0; j--, k += sw) {
					dst[j] = src[k];
				}

				src++;
				dst += dw;
			}
		} else {
			uint8_t *src = src_bmp.data;
			uint8_t *dst = dst_bmp.data;

			for (int i = 0; i < sw; i++) {
				for (int j = (sh - 1) * bpp, k = 0; j >= 0; j -= bpp, k += sp) {
					for (int b = 0; b < bpp; b++) {
						dst[j + b] = src[k + b];
					}
				}

				src += bpp;
				dst += dp;
			}
		}
		break;

	case E_ROT_180:
		if ((dw != sw) || (dh != sh)) {
			break;
		}

		if (bpp == 4) {
			uint32_t *src = (uint32_t *) src_bmp.data;
			uint32_t *dst = (uint32_t *) dst_bmp.data;

			src += (sh - 1) * sw;
			for (int i = 0; i < sh; i++) {
				for (int j = 0; j < sw; j++) {
					dst[j] = src[sw - j - 1];
				}

				src -= sw;
				dst += dw;
			}
		} else {
			uint8_t *src = src_bmp.data;
			uint8_t *dst = dst_bmp.data;

			src += (sh - 1) * sp;
			for (int i = 0; i < sh; i++) {
				for (int j = 0, k = sp - bpp; j < sp; j += bpp, k -= bpp) {
					for (int b = 0; b < bpp; b++) {
						dst[j + b] = src[k + b];
					}
				}

				src -= sp;
				dst += dp;
			}
		}
		break;

	case E_ROT_270:
		if ((dw != sh) || (dh != sw)) {
			break;
		}

		if (bpp == 4) {
			uint32_t *src = (uint32_t *) src_bmp.data;
			uint32_t *dst = (uint32_t *) dst_bmp.data;

			src += sw - 1;
			for (int i = 0; i < sw; i++) {
				for (int j = 0, k = 0; j < sh; j++, k += sw) {
					dst[j] = src[k];
				}

				src--;
				dst += dw;
			}
		} else {
			uint8_t *src = src_bmp.data;
			uint8_t *dst = dst_bmp.data;

			src += (sw - 1) * bpp;
			for (int i = 0; i < sw; i++) {
				for (int j = 0, k = 0; j < dp; j += bpp, k += sp) {
					for (int b = 0; b < bpp; b++) {
						dst[j + b] = src[k + b];
					}
				}

				src -= bpp;
				dst += dp;
			}
		}
		break;
	}
}

void bitmap_gaussian_blur(bitmap_t &bmp, float sigma) {
	int width = bmp.width;
	int height = bmp.height;
	int channels = bmp.bytes;
	int pitch = bmp.pitch ? bmp.pitch : width * channels;
	uint8_t *data = bmp.data;

	float a0, a1, a2, a3, b1, b2, cprev, cnext;
	cal_gaussian_coeff(sigma, &a0, &a1, &a2, &a3, &b1, &b2, &cprev, &cnext);

	float a0a1 = (a0 + a1);
	float a2a3 = (a2 + a3);
	float b1b2 = (b1 + b2);

	int bufferSizePerThread = (width > height ? width : height) * channels;
	uint8_t *bufferPerLine = (uint8_t *) malloc(bufferSizePerThread);
	uint8_t *tempData = (uint8_t *) malloc(height * width * channels);
	if (!bufferPerLine || !tempData) {
		if (tempData) {
			free(tempData);
		}
		if (bufferPerLine) {
			free(bufferPerLine);
		}
		return;
	}

	for (int y = 0; y < height; ++y) {
		uint8_t *lpRowInitial = data + pitch * y;
		uint8_t *lpColInitial = tempData + y * channels;
		gaussian_horizontal(bufferPerLine, lpRowInitial, lpColInitial, width,
				height, channels, width, a0a1, a2a3, b1b2, cprev, cnext);
	}

	int heightStep = height * channels;
	for (int x = 0; x < width; ++x) {
		uint8_t *lpColInitial = data + x * channels;
		uint8_t *lpRowInitial = tempData + heightStep * x;
		gaussian_vertical(bufferPerLine, lpRowInitial, lpColInitial, height,
				width, channels, a0a1, a2a3, b1b2, cprev, cnext);
	}

	free(bufferPerLine);
	free(tempData);
}

void bitmap_fit(bitmap_t &dst_bmp, const bitmap_t &src_bmp, EImageBgMode mode, Region *fit_rgn) {
	uint32_t fit_w = src_bmp.width;
	uint32_t fit_h = src_bmp.height;
	fit_size(fit_w, fit_h, dst_bmp.width, dst_bmp.height);

	uint32_t fit_x = (dst_bmp.width - fit_w) / 2;
	uint32_t fit_y = (dst_bmp.height - fit_h) / 2;

	if (mode != E_IMAGE_BG_MODE_BLUR) {
		if ((fit_x > 0) || (fit_y > 0)) {
			memset(dst_bmp.data, (mode == E_IMAGE_BG_MODE_WHITE) ? 0xFF : 0, dst_bmp.pitch * dst_bmp.height);
		}
	}

	bitmap_t bmp = dst_bmp;
	bmp.width = fit_w;
	bmp.height = fit_h;
	bmp.data += BITMAP_OFFSET(dst_bmp, fit_x, fit_y);

	bitmap_scale(bmp, src_bmp);

	if (mode == E_IMAGE_BG_MODE_BLUR) {
		if ((fit_x > 0) || (fit_y > 0)) {
			float sigma = 8.f;
			uint32_t chunk_w, chunk_h;

			if (fit_x > 0) {
				float factor = (float) src_bmp.width / dst_bmp.width;
				chunk_w = factor * fit_x;
				chunk_h = factor * dst_bmp.height;
			} else {
				float factor = (float) src_bmp.height / dst_bmp.height;
				chunk_w = factor * dst_bmp.width;
				chunk_h = factor * fit_y;
			}
			if (chunk_w == 0) {
				chunk_w = 1;
			}
			if (chunk_h == 0) {
				chunk_h = 1;
			}

			bitmap_t chunk_bmp;
			bitmap_create(chunk_bmp, chunk_w, chunk_h, src_bmp.bytes);

			bitmap_t clip_bmp = src_bmp;
			clip_bmp.width = chunk_w;
			clip_bmp.height = chunk_h;

			if (fit_x > 0) {
				clip_bmp.data += (src_bmp.height - clip_bmp.height) / 2 * src_bmp.pitch;
				bmp.width = fit_x;
				bmp.height = dst_bmp.height;
				bmp.data = dst_bmp.data;
			} else {
				clip_bmp.data += (src_bmp.width - chunk_bmp.width) / 2 * src_bmp.bytes;
				bmp.width = dst_bmp.width;
				bmp.height = fit_y;
				bmp.data = dst_bmp.data;
			}

			bitmap_copy(chunk_bmp, clip_bmp);
			bitmap_gaussian_blur(chunk_bmp, sigma);
			bitmap_scale(bmp, chunk_bmp);

			if (fit_x > 0) {
				clip_bmp.data += (src_bmp.width - clip_bmp.width) * src_bmp.bytes;
				bmp.width = dst_bmp.width - (fit_x + fit_w);
				bmp.data += (fit_x + fit_w) * dst_bmp.bytes;
			} else {
				clip_bmp.data += (src_bmp.height - clip_bmp.height) * src_bmp.pitch;
				bmp.height = dst_bmp.height - (fit_y + fit_h);
				bmp.data += (fit_y + fit_h) * dst_bmp.pitch;
			}

			bitmap_copy(chunk_bmp, clip_bmp);
			bitmap_gaussian_blur(chunk_bmp, sigma);
			bitmap_scale(bmp, chunk_bmp);

			bitmap_destroy(chunk_bmp);
		}
	}

	if (fit_rgn) {
		REGION_SET(*fit_rgn, fit_x, fit_y, fit_x + fit_w, fit_y + fit_h);
	}
}

void bitmap_fill(bitmap_t &dst_bmp, const bitmap_t &src_bmp, const Region *rgn) {
	Region src_rgn = { 0, 0, (uint16_t) src_bmp.width, (uint16_t) src_bmp.height };
	if (rgn && !RegionIntersect(src_rgn, src_rgn, *rgn)) {
		return;
	}

	bitmap_t rgn_bmp = src_bmp;
	rgn_bmp.width = REGION_WIDTH(src_rgn);
	rgn_bmp.height = REGION_HEIGHT(src_rgn);
	rgn_bmp.data += BITMAP_OFFSET(rgn_bmp, src_rgn.left, src_rgn.top);

	bitmap_scale(dst_bmp, rgn_bmp);
}

}
