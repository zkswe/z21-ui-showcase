/*
 * base_utility.h
 *
 *  Created on: 2026年1月31日
 *      Author: ZKSWE Develop Team
 */

#ifndef _INTERNAL_BASE_UTILITY_H_
#define _INTERNAL_BASE_UTILITY_H_

#define LOG_TAG "ext_widgets"

#include "base/file.h"
#include "manager/ConfigManager.h"
#include "utils/BitmapHelper.h"
#include "utils/Log.h"

static inline std::string get_res_path(const std::string &file) {
	return base::exists(file) ? file : CONFIGMANAGER->getResFilePath(file);
}

static inline void fit_size(uint32_t &target_w, uint32_t &target_h, uint32_t max_w, uint32_t max_h) {
	if ((float) target_w / max_w > (float) target_h / max_h) {
		target_h = max_w * target_h / target_w;
		target_w = max_w;
	} else {
		target_w = max_h * target_w / target_h;
		target_h = max_h;
	}
}

static inline void pack_size(uint32_t &target_w, uint32_t &target_h, uint32_t max_w, uint32_t max_h) {
	if ((float) target_w / max_w < (float) target_h / max_h) {
		target_h = max_w * target_h / target_w;
		target_w = max_w;
	} else {
		target_w = max_h * target_w / target_h;
		target_h = max_h;
	}
}

static inline void* load_file_data(const std::string &file, uint32_t &size) {
	FILE *pf = fopen(file.c_str(), "rb");
	if (!pf) {
		return NULL;
	}

	void *data = NULL;
	do {
		fseek(pf, 0, SEEK_END);
		size = ftell(pf);
		fseek(pf, 0, SEEK_SET);

		data = malloc(size);
		if (!data) {
			break;
		}

		if (fread(data, size, 1, pf) != 1) {
			free(data);
			data = NULL;
		}
	} while (0);
	fclose(pf);

	return data;
}

#endif /* _INTERNAL_BASE_UTILITY_H_ */
