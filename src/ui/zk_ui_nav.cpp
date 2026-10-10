/*
 * zk_ui_nav.cpp —— 导航防抖实现（见 zk_ui_nav.h 的说明）
 *
 * 真机现象：一次点击连翻两页 → 奇数下标页（控件族 1 / 日期时间 3 / 画板 5）被跳过、
 *           表现为"进不去"。真因是 openActivity 后新页也会收到同一个 UP 事件。
 * 处置：进程级时间戳，350ms 内只放行一次导航。
 */
#include "ui/zk_ui_nav.h"

#include <sys/time.h>

static long long sZkNavLastMs = 0;

bool zkNavDebounce() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    long long now = (long long)tv.tv_sec * 1000LL + (long long)tv.tv_usec / 1000LL;
    if (now - sZkNavLastMs < 350) {
        return false;
    }
    sZkNavLastMs = now;
    return true;
}
