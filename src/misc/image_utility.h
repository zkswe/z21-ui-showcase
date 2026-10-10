/*
 * image_utility.h
 *
 *  Created on: 2025年12月08日
 *      Author: ZKSWE Develop Team
 */

#ifndef _MISC_IMAGE_UTILITY_H_
#define _MISC_IMAGE_UTILITY_H_

#include "bitmap_utility.h"

namespace misc {

bool image_load(const std::string &img, bitmap_t &bmp, ui::Size *src_size = NULL, const ui::Size *max_size = NULL, int flag = ui::E_IMAGE_FLAG_OPAQUE);
bool image_save(const std::string &img, const bitmap_t &bmp, ui::EImageType type = ui::E_IMAGE_TYPE_JPG);
bool image_resize(const std::string &src, const std::string &dst, const ui::Size &max_size);

bool image_fit_bitmap(const std::string &img, bitmap_t &bmp, ui::EImageBgMode mode, ui::Region *fit_rgn = NULL);
bool image_fill_bitmap(const std::string &img, bitmap_t &bmp, const ui::Region *rgn = NULL);

bool image_rotate(const std::string &dst_img, const std::string &src_img, ui::ERot rot);

typedef bool (*func_img_decode_hook)(const void *data, uint32_t size, const ui::Size &src_size, bitmap_t &bmp);
typedef bool (*func_img_encode_hook)(const bitmap_t &bmp, void *&data, uint32_t &size);

void image_set_decode_hook(ui::EImageType type, func_img_decode_hook decode);
void image_set_encode_hook(ui::EImageType type, func_img_encode_hook encode);

}

#endif /* _MISC_IMAGE_UTILITY_H_ */
