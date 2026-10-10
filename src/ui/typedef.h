/*
 * typedef.h
 *
 *  Created on: 2025年10月24日
 *      Author: ZKSWE Develop Team
 */

#ifndef _UI_TYPEDEF_H_
#define _UI_TYPEDEF_H_

#include <string>
#include "Region.h"

namespace ui {

#define TOUCH_SLOP           5
#define TRIGGER_MIN_SPEED    0.0001

#define BAR_DEF_WIDTH        4
#define BAR_DEF_MARGIN       BAR_DEF_WIDTH
#define BAR_DEF_COLOR        0xFF9F9F9F

enum ERot {
	E_ROT_0,
	E_ROT_90,
	E_ROT_180,
	E_ROT_270,
};

enum EOrientation {
	E_ORIENTATION_HORIZONTAL,
	E_ORIENTATION_VERTICAL,
};

enum EDirection {
	E_DIRECTION_LEFT_TO_RIGHT,
	E_DIRECTION_RIGHT_TO_LEFT,
	E_DIRECTION_TOP_TO_BOTTOM,
	E_DIRECTION_BOTTOM_TO_TOP,
};

enum EMotionFilter {
	E_MOTION_FILTER_NONE,
	E_MOTION_FILTER_INTERCEPT,
	E_MOTION_FILTER_FOLD,
};

enum EImageType {
	E_IMAGE_TYPE_JPG,
	E_IMAGE_TYPE_PNG,
	E_IMAGE_TYPE_BMP,
};

enum EImageFlag {
	E_IMAGE_FLAG_OPAQUE = 0x01,
	E_IMAGE_FLAG_BGR = 0x02,
	E_IMAGE_FLAG_FIT = 0x04,
	E_IMAGE_FLAG_PACK = 0x08,
	E_IMAGE_FLAG_UP_TO_BOTTOM = 0x10,
};

enum EImageShowMode {
	E_IMAGE_SHOW_MODE_FIT,    // 适配
	E_IMAGE_SHOW_MODE_FILL,   // 填充
};

enum EImageBgMode {
	E_IMAGE_BG_MODE_BLUR,
	E_IMAGE_BG_MODE_WHITE,
	E_IMAGE_BG_MODE_BLACK,
};

enum EImageEditMode {
	E_IMAGE_EDIT_MODE_SHOW,
	E_IMAGE_EDIT_MODE_ROT,
};

enum EImageSwitchEffect {
	E_IMAGE_SWITCH_EFFECT_NONE,
	E_IMAGE_SWITCH_EFFECT_MOVE,
	E_IMAGE_SWITCH_EFFECT_ZOOM,
	E_IMAGE_SWITCH_EFFECT_SQUEEZE_BOX,
};

struct Size {
	uint32_t w;
	uint32_t h;
};

struct ImageParam {
	std::string img;
	int64_t last_modified_time;
	EImageEditMode edit_mode;
	EImageShowMode show_mode;
	EImageBgMode bg_mode;
	bool scalable;
	Region rgn;
	Region crop;

	ImageParam() {
		last_modified_time = 0;
		edit_mode = E_IMAGE_EDIT_MODE_SHOW;
		show_mode = E_IMAGE_SHOW_MODE_FIT;
		bg_mode = E_IMAGE_BG_MODE_BLUR;
		scalable = false;
		REGION_RESET(rgn);
		REGION_RESET(crop);
	}
};

}

#endif /* _UI_TYPEDEF_H_ */
