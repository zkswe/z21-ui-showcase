#pragma once
#include "entry/EasyUIContext.h"
#include "zk/zk_page_anim.h"
/* zk_ui_nav.h —— UIShowcase 演示页导航
 *
 * 设计口径（2026-10-10 定）：
 *   ① 底部 8 页签的点击由 **NavBar 容器内的透明按钮**（caption NavBtn1..8）承载；
 *      命中区域 = 按钮自身几何（在 json/设计稿里定义），框架分派到 onButtonClick_NavBtnN。
 *      —— 不再在代码里写死 x/y/段宽 等几何常量（原 zkNavHit 已删除）。
 *   ② 标题区(y<70) 仍保留"点标题循环切页"隐藏入口（走全局触摸回调 zkNavTouch）。
 * 页面顺序与 kDemoApps / gen_draft 的 PAGES 一致。 */

static const char *kDemoApps[] = {
    "mainActivity", "widgetsActivity", "chartsActivity", "datetimeActivity",
    "listActivity", "imageActivity", "musicActivity", "settingsActivity",
};
static const int kDemoAppCount = 8;

/* 跨页防抖（实现见 zk_ui_nav.cpp）：
 * 真机现象 —— 一次点击会**连翻两页**（奇数页被跳过，表现为"进不去"）。
 * 真因：FlyThings `openActivity` 会把**新打开的那页**也加进全局触摸监听表，
 *       同一个 UP 事件在新页上又被处理一次。处置：350ms 内只放行一次导航。 */
bool zkNavDebounce();

/* 跳转到第 i 页（页签按钮回调共用）。350ms 内重复触发忽略。
 * 各页 logic 里的 `onButtonClick_NavBtnN` 由 fsc 生成桩，实现为 `return zkNavGo(N-1);`
 *（顺序与 kDemoApps 一致：main/widgets/charts/datetime/list/image/music/settings）。 */
static bool zkNavGo(int i) {
    if (i < 0 || i >= kDemoAppCount) return false;
    if (!zkNavDebounce()) return true;
    EASYUICONTEXT->openActivity(kDemoApps[i]);
    return true;
}

/* 通用触摸处理：仅处理标题区(y<70) → 循环下一页；底部页签交给 NavBtn* 按钮。
 * selfIdx = 本页在 kDemoApps 里的下标 */
static bool zkNavTouch(int selfIdx, const MotionEvent &ev) {
    if (ev.mActionStatus != MotionEvent::E_ACTION_UP) return false;
    if (ev.mY < 70) {
        if (!zkNavDebounce()) return false;
        int n = (selfIdx + 1) % kDemoAppCount;
        EASYUICONTEXT->openActivity(kDemoApps[n]);
        return true;
    }
    return false;
}

/* 只解析"该去哪页"，不跳转（保留给"先播退场动画再跳"用）。
 * selfIdx = 本页在 kDemoApps 里的下标。 */
static const char *zkNavTarget(int selfIdx, const MotionEvent &ev) {
    (void)selfIdx; (void)ev;      /* 退场动画暂关；保留签名 */
    return NULL;
}
