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
    zk::PageAnim::addSlideLeft2Right(mLb2896Ptr, 440, 140);
    zk::PageAnim::addSlideLeft2Right(mBtnPrimaryPtr, 460, 180);
    zk::PageAnim::addSlideLeft2Right(mBtnOutlinePtr, 460, 180);
    zk::PageAnim::addSlideLeft2Right(mBtnGhostPtr, 460, 180);
    zk::PageAnim::addSlideLeft2Right(mBtnIconPtr, 460, 180);
    zk::PageAnim::addSlideLeft2Right(mBtnDisabledPtr, 460, 180);
    zk::PageAnim::addSlideLeft2Right(mLb2990Ptr, 440, 250);
    /* 开关/复选/单选：2026-10-10 改为**两态 button**（原为静态 textview+旋钮叠加） */
    zk::PageAnim::addSlideLeft2Right(mSw1Ptr, 440, 290);
    zk::PageAnim::addSlideLeft2Right(mSw2Ptr, 440, 290);
    zk::PageAnim::addSlideLeft2Right(mCb1Ptr, 440, 340);
    zk::PageAnim::addSlideLeft2Right(mCbOnTxtPtr, 440, 340);
    zk::PageAnim::addSlideLeft2Right(mCb2Ptr, 440, 340);
    zk::PageAnim::addSlideLeft2Right(mCbOffTxtPtr, 440, 340);
    zk::PageAnim::addSlideLeft2Right(mRd1Ptr, 440, 340);
    zk::PageAnim::addSlideLeft2Right(mRd1TxtPtr, 440, 340);
    zk::PageAnim::addSlideLeft2Right(mLb3084Ptr, 440, 400);
    zk::PageAnim::addSlideLeft2Right(mSkDemoPtr, 440, 440);
    zk::PageAnim::addSlideLeft2Right(mSkValPtr, 440, 440);
    zk::PageAnim::addSlideLeft2Right(mStepMinusPtr, 440, 480);
    zk::PageAnim::addSlideLeft2Right(mStepValPtr, 440, 480);
    zk::PageAnim::addSlideLeft2Right(mStepPlusPtr, 440, 480);
    zk::PageAnim::addSlideLeft2Right(mLb3166Ptr, 440, 520);
    zk::PageAnim::addSlideLeft2Right(mToastPtr, 460, 560);
    zk::PageAnim::addSlideLeft2Right(mBadgeHostPtr, 460, 600);
    zk::PageAnim::addSlideLeft2Right(mCardPanelPtr, 460, 640);
    zk::PageAnim::start();
}

static bool sSw1 = true, sSw2 = false, sCb1 = true, sCb2 = false, sRd1 = true;
static int  sStep = 12;

static void stepApply() {
    char b[8];
    snprintf(b, sizeof(b), "%d", sStep);
    mStepValPtr->setText(b);
}

static void onUI_init() {
    zkAnimInit();
  #ifdef FUN_BUILD
  INIT_UI_TIMERS
  #endif // FUN_BUILD
    /* 两态按钮初值（与设计稿一致：Sw1/Cb1/Rd1 开、Sw2/Cb2 关） */
    mSw1Ptr->setSelected(sSw1);
    mSw2Ptr->setSelected(sSw2);
    mCb1Ptr->setSelected(sCb1);
    mCb2Ptr->setSelected(sCb2);
    mRd1Ptr->setSelected(sRd1);
    mSkDemoPtr->setProgress(62);
    mSkValPtr->setText("62%");
    stepApply();
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

static bool onwidgetsActivityTouchEvent(const MotionEvent &ev) {
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
	return zkNavTouch(1, ev);
}

static bool onButtonClick_BtnPrimary(ZKButton* pButton) {
	return false;
}

static bool onButtonClick_BtnOutline(ZKButton* pButton) {
	return false;
}

static bool onButtonClick_BtnGhost(ZKButton* pButton) {
	return false;
}

static bool onButtonClick_BtnIcon(ZKButton* pButton) {
	return false;
}

static bool onButtonClick_BtnDisabled(ZKButton* pButton) {
	return false;
}

/* 开关/复选/单选：两态按钮切换（pic0=关/未选, pic2=开/选中） */
static bool onButtonClick_Sw1(ZKButton* pButton) { sSw1 = !sSw1; pButton->setSelected(sSw1); return false; }

static bool onButtonClick_Sw2(ZKButton* pButton) { sSw2 = !sSw2; pButton->setSelected(sSw2); return false; }

static bool onButtonClick_Cb1(ZKButton* pButton) { sCb1 = !sCb1; pButton->setSelected(sCb1); return false; }

static bool onButtonClick_Cb2(ZKButton* pButton) { sCb2 = !sCb2; pButton->setSelected(sCb2); return false; }

static bool onButtonClick_Rd1(ZKButton* pButton) { sRd1 = !sRd1; pButton->setSelected(sRd1); return false; }

static bool onButtonClick_StepMinus(ZKButton* pButton) {
    if (sStep > 0) sStep--;
    stepApply();
    return false;
}

static bool onButtonClick_StepPlus(ZKButton* pButton) {
    if (sStep < 20) sStep++;
    stepApply();
    return false;
}

static void onProgressChanged_SkDemo(ZKSeekBar *pSeekBar, int progress) {
    char b[16];
    snprintf(b, sizeof(b), "%d%%", progress);
    mSkValPtr->setText(b);
}

/* 输入框演示：系统键盘输入后实时回显 */
static void onEditTextChanged_EdDemo(const std::string &text) {
    std::string s = "输入: ";
    s += text.empty() ? "-" : text;
    if (mEdEchoPtr) mEdEchoPtr->setText(s);
}

static void onProgressChanged_PanelBar(ZKSeekBar *pSeekBar, int progress) {
}static bool onButtonClick_NavBtn1(ZKButton* pButton) { return zkNavGo(0); }

static bool onButtonClick_NavBtn2(ZKButton* pButton) { return zkNavGo(1); }

static bool onButtonClick_NavBtn3(ZKButton* pButton) { return zkNavGo(2); }

static bool onButtonClick_NavBtn4(ZKButton* pButton) { return zkNavGo(3); }

static bool onButtonClick_NavBtn5(ZKButton* pButton) { return zkNavGo(4); }

static bool onButtonClick_NavBtn6(ZKButton* pButton) { return zkNavGo(5); }

static bool onButtonClick_NavBtn7(ZKButton* pButton) { return zkNavGo(6); }

static bool onButtonClick_NavBtn8(ZKButton* pButton) { return zkNavGo(7); }


