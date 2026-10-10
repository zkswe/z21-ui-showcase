/*
 * bitmap_utility.h
 *
 *  Created on: 2025年12月22日
 *      Author: ZKSWE Develop Team
 */

#ifndef _MISC_BITMAP_UTILITY_H_
#define _MISC_BITMAP_UTILITY_H_

#include <utils/BitmapHelper.h>
#include "ui/typedef.h"
#include "ui/Region.h"

namespace misc {

#define BITMAP_OFFSET(bmp, x, y)    ((y) * (bmp).pitch + (x) * (bmp).bytes)

bool bitmap_create(bitmap_t &bmp, uint32_t w, uint32_t h, uint8_t bpp);
void bitmap_destroy(bitmap_t &bmp);

void bitmap_copy(bitmap_t &dst_bmp, const bitmap_t &src_bmp, const ui::Region *rgn = NULL);
bool bitmap_cmp(const bitmap_t &dst_bmp, const bitmap_t &src_bmp);
bool bitmap_copy_and_cmp(bitmap_t &dst_bmp, const bitmap_t &src_bmp);

void bitmap_scale(bitmap_t &dst_bmp, const bitmap_t &src_bmp);
void bitmap_scale(bitmap_t &dst_bmp, uint32_t src_w, uint32_t src_h, uint32_t src_pitch,
		void (*read_line)(int y, uint8_t *buf, void *user_data), void *user_data);

void bitmap_rotate(bitmap_t &dst_bmp, const bitmap_t &src_bmp, ui::ERot rot);

void bitmap_gaussian_blur(bitmap_t &bmp, float sigma = 5.f);

void bitmap_fit(bitmap_t &dst_bmp, const bitmap_t &src_bmp, ui::EImageBgMode mode, ui::Region *fit_rgn = NULL);
void bitmap_fill(bitmap_t &dst_bmp, const bitmap_t &src_bmp, const ui::Region *rgn = NULL);

}

#endif /* _MISC_BITMAP_UTILITY_H_ */
