#include <base/base.h>
#ifdef FUN_BUILD
#include GENERATED_UI_DEFINITIONS
INIT_UI_EVENT_BINDINGS
#endif // FUN_BUILD
#pragma once
#include "uart/ProtocolSender.h"
#include "ui/zk_ui_nav.h"
#include "zk/zk_page_anim.h"

/* 由设计稿装配生成（UIShowcase-Z21） */

static S_ACTIVITY_TIMEER REGISTER_ACTIVITY_TIMER_TAB[] = {
    {9, 16},   /* page anim */
	//{0, 1000},
};

static void zkAnimInit() {
    zk::PageAnim::init(9, 16);        /* 与 REGISTER_ACTIVITY_TIMER_TAB 的 {9,16} 对应 */
    zk::PageAnim::setSlideAlphaEnable(false);   /* 本机 setAlpha 不重绘：只做位移 */
    zk::PageAnim::addSlideTop2Bottom(mTitleTextPtr, 420, 0);
    zk::PageAnim::addSlideTop2Bottom(mTitleSubPtr, 420, 60);
    zk::PageAnim::addSlideBottom2Top(mNavBarPtr, 460, 40);
    zk::PageAnim::addSlideLeft2Right(mHeroCardPtr, 460, 140);
    zk::PageAnim::addSlideLeft2Right(mCardStat1Ptr, 460, 200);
    zk::PageAnim::addSlideRight2Left(mCardStat2Ptr, 460, 200);
    zk::PageAnim::addSlideLeft2Right(mMetric1Ptr, 440, 260);
    zk::PageAnim::addSlideLeft2Right(mMetric2Ptr, 440, 300);
    zk::PageAnim::addSlideLeft2Right(mMetric3Ptr, 440, 340);
    zk::PageAnim::addSlideLeft2Right(mMetric4Ptr, 440, 380);
    zk::PageAnim::start();
}

static void onUI_init() {
    zkAnimInit();
  #ifdef FUN_BUILD
  INIT_UI_TIMERS
  #endif // FUN_BUILD
}

static void onUI_intent(const Intent *intentPtr) {
}

static bool sPageShown = false;   /* 首次显示标记：复用页才需要重挂定时器 */
static void onUI_show() {
    /* 复用页再进入：定时器**不会自动重挂** → 先重挂，否则动画推不动；
       空闲则重新登记（基准=原位）重播，正在播就重播现成的 */
    if (sPageShown) {
      #ifdef FUN_BUILD
      INIT_UI_TIMERS
      #endif // FUN_BUILD
    }
    sPageShown = true;
    if (zk::PageAnim::isRunning()) { zk::PageAnim::start(); } else { zkAnimInit(); }
}

static void onUI_hide() {
}

static void onUI_quit() { zk::PageAnim::release();
}

static void onProtocolDataUpdate(const SProtocolData &data) {
}

static bool onUI_Timer(int id) {
    zk::PageAnim::onTimer(id);
	switch (id) {
		default:
			break;
	}
	return true;
}

static bool onmainActivityTouchEvent(const MotionEvent &ev) {
    switch (ev.mActionStatus) {
		case MotionEvent::E_ACTION_DOWN:
			break;
		case MotionEvent::E_ACTION_MOVE:
			break;
		case MotionEvent::E_ACTION_UP:
			break;
		default:
			break;
	}
	return zkNavTouch(0, ev);
}

static bool onButtonClick_BtnHeroGo(ZKButton* pButton) {
	return false;
}

static void onProgressChanged_PerfBar(ZKSeekBar *pSeekBar, int progress) {
}static bool onButtonClick_NavBtn1(ZKButton* pButton) { return zkNavGo(0); }

static bool onButtonClick_NavBtn2(ZKButton* pButton) { return zkNavGo(1); }

static bool onButtonClick_NavBtn3(ZKButton* pButton) { return zkNavGo(2); }

static bool onButtonClick_NavBtn4(ZKButton* pButton) { return zkNavGo(3); }

static bool onButtonClick_NavBtn5(ZKButton* pButton) { return zkNavGo(4); }

static bool onButtonClick_NavBtn6(ZKButton* pButton) { return zkNavGo(5); }

static bool onButtonClick_NavBtn7(ZKButton* pButton) { return zkNavGo(6); }

static bool onButtonClick_NavBtn8(ZKButton* pButton) { return zkNavGo(7); }


