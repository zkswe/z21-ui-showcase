/*
 * Region.h
 *
 *  Created on: 2025年10月24日
 *      Author: ZKSWE Develop Team
 */

#ifndef _UI_REGION_H_
#define _UI_REGION_H_

#include <memory>
#include <stdint.h>

namespace ui {

struct Region {
	uint16_t left;
	uint16_t top;
	uint16_t right;
	uint16_t bottom;
};

#define REGION_WIDTH(rgn)                   ((rgn).right - (rgn).left)
#define REGION_HEIGHT(rgn)                  ((rgn).bottom - (rgn).top)

#define REGION_SET(rgn, l, t, r, b)         \
    do {                                    \
        (rgn).left = l;                     \
        (rgn).top = t;                      \
        (rgn).right = r;                    \
        (rgn).bottom = b;                   \
    } while (0)

#define REGION_RESET(rgn)                   REGION_SET(rgn, 0, 0, 0, 0)

#define REGION_IS_EMPTY(rgn)                \
    (((rgn).left == (rgn).right) ||         \
     ((rgn).top == (rgn).bottom))

#define REGION_IS_EQUAL(rgn1, rgn2)         \
    (((rgn1).left == (rgn2).left) &&        \
     ((rgn1).top == (rgn2).top) &&          \
     ((rgn1).right == (rgn2).right) &&      \
     ((rgn1).bottom == (rgn2).bottom))

#define REGION_OFFSET(rgn, x, y)            \
    do {                                    \
        (rgn).left += x;                    \
        (rgn).right += x;                   \
        (rgn).top += y;                     \
        (rgn).bottom += y;                  \
    } while (0)

#define REGION_CONTAINS(rgn1, rgn2)         \
    (((rgn1).left <= (rgn2).left) &&        \
     ((rgn1).right >= (rgn2).right) &&      \
     ((rgn1).top <= (rgn2).top) &&          \
     ((rgn1).bottom >= (rgn2).bottom))

#define REGION_DOES_INTERSECT(rgn1, rgn2)   \
    !(((rgn1).right <= (rgn2).left) ||      \
      ((rgn1).left >= (rgn2).right) ||      \
      ((rgn1).bottom <= (rgn2).top) ||      \
      ((rgn1).top >= (rgn2).bottom))

static inline bool RegionIntersect(Region &rgn, const Region &rgn1, const Region &rgn2) {
	if (!REGION_DOES_INTERSECT(rgn1, rgn2)) {
		return false;
	}

	REGION_SET(rgn, std::max(rgn1.left, rgn2.left), std::max(rgn1.top, rgn2.top),
			std::min(rgn1.right, rgn2.right), std::min(rgn1.bottom, rgn2.bottom));

	return true;
}

static inline void RegionBound(Region &rgn, const Region &rgn1, const Region &rgn2) {
	if (REGION_IS_EMPTY(rgn1)) {
		rgn = rgn2;
	} else if (REGION_IS_EMPTY(rgn2)) {
		rgn = rgn1;
	} else {
		REGION_SET(rgn, std::min(rgn1.left, rgn2.left), std::min(rgn1.top, rgn2.top),
				std::max(rgn1.right, rgn2.right), std::max(rgn1.bottom, rgn2.bottom));
	}
}

}

#endif /* _UI_REGION_H_ */
