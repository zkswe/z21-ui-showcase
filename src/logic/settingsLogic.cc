#include <base/base.h>
#ifdef FUN_BUILD
#include GENERATED_UI_DEFINITIONS
INIT_UI_EVENT_BINDINGS
#endif // FUN_BUILD
#pragma once
#include "uart/ProtocolSender.h"
#include "ui/zk_ui_nav.h"
#include "zk/zk_page_anim.h"
#include <net/NetManager.h>   /* WiFi：NETMANAGER->getWifiManager() */

/* 由设计稿装配生成（UIShowcase-Z21） */

static S_ACTIVITY_TIMEER REGISTER_ACTIVITY_TIMER_TAB[] = {
    {9, 16},   /* page anim */
    {1, 1000}, /* WiFi 状态刷新 */
};

static void zkAnimInit() {
    zk::PageAnim::init(9, 16);        /* 与 REGISTER_ACTIVITY_TIMER_TAB 的 {9,16} 对应 */
    zk::PageAnim::setSlideAlphaEnable(false);   /* 本机 setAlpha 不重绘：只做位移 */
    zk::PageAnim::addSlideTop2Bottom(mTitleTextPtr, 420, 0);
    zk::PageAnim::addSlideTop2Bottom(mTitleSubPtr, 420, 60);
    zk::PageAnim::addSlideBottom2Top(mNavBarPtr, 460, 40);
    /* 外观分段：轨道 + 深色/浅色 两个按钮 —— 同参同组 */
    zk::PageAnim::addSlideLeft2Right(mLb2896Ptr, 440, 140);
    zk::PageAnim::addSlideLeft2Right(mSegTrackPtr, 440, 140);
    zk::PageAnim::addSlideLeft2Right(mSegDarkPtr, 440, 140);
    zk::PageAnim::addSlideLeft2Right(mSegLightPtr, 440, 140);
    /* 开关行：每行 名称+两态开关 同参（2026-10-10：旋钮合并进两态底图） */
    zk::PageAnim::addSlideLeft2Right(mLb2990Ptr, 440, 210);
    zk::PageAnim::addSlideLeft2Right(mTg1NamePtr, 420, 250);
    zk::PageAnim::addSlideLeft2Right(mTg1Ptr, 420, 250);
    zk::PageAnim::addSlideLeft2Right(mTg2NamePtr, 420, 290);
    zk::PageAnim::addSlideLeft2Right(mTg2Ptr, 420, 290);
    zk::PageAnim::addSlideLeft2Right(mTg3NamePtr, 420, 330);
    zk::PageAnim::addSlideLeft2Right(mTg3Ptr, 420, 330);
    zk::PageAnim::addSlideLeft2Right(mTg4NamePtr, 420, 370);
    zk::PageAnim::addSlideLeft2Right(mTg4Ptr, 420, 370);
    zk::PageAnim::addSlideLeft2Right(mDiagCardPtr, 460, 430);
    zk::PageAnim::addSlideLeft2Right(mBtnSavePtr, 420, 520);
    zk::PageAnim::addSlideLeft2Right(mBtnResetPtr, 420, 520);
    zk::PageAnim::start();
}

static bool sTg1 = true, sTg2 = false, sTg3 = true, sTg4 = false;   /* 开关项初值 */
static bool sDark = true;                                          /* 主题：true=深色 */

/* WiFi：读真实状态刷新诊断「网络」行（硬开关已移除，只读展示；改网络走系统内置页） */
static void wifiRefresh() {
    WifiManager* wm = NETMANAGER->getWifiManager();
    if (!wm) return;
    bool sup = wm->isSupported();
    bool en = sup && wm->isWifiEnable();
    char b[64];
    if (!sup)                    snprintf(b, sizeof(b), "无 WiFi 模块");
    else if (!en)                snprintf(b, sizeof(b), "WiFi 已关闭");
    else if (wm->isConnected())  snprintf(b, sizeof(b), "已连接 %s", wm->getIp());
    else                         snprintf(b, sizeof(b), "WiFi 已开启（未连接）");
    mDiag6VPtr->setText(b);
}

static void onUI_init() {
    zkAnimInit();
  #ifdef FUN_BUILD
  INIT_UI_TIMERS
  #endif // FUN_BUILD
    mTg1Ptr->setSelected(sTg1);
    mTg2Ptr->setSelected(sTg2);
    mTg3Ptr->setSelected(sTg3);
    mTg4Ptr->setSelected(sTg4);
    mSegDarkPtr->setSelected(sDark);
    mSegLightPtr->setSelected(!sDark);
    wifiRefresh();
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
		case 1:
            wifiRefresh();
			break;
		default:
			break;
	}
	return true;
}

static bool onsettingsActivityTouchEvent(const MotionEvent &ev) {
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
	return zkNavTouch(7, ev);
}

/* 开关项：两态按钮切换 */
static bool onButtonClick_Tg1(ZKButton* pButton) { sTg1 = !sTg1; pButton->setSelected(sTg1); return false; }

static bool onButtonClick_Tg2(ZKButton* pButton) { sTg2 = !sTg2; pButton->setSelected(sTg2); return false; }

static bool onButtonClick_Tg3(ZKButton* pButton) { sTg3 = !sTg3; pButton->setSelected(sTg3); return false; }

static bool onButtonClick_Tg4(ZKButton* pButton) { sTg4 = !sTg4; pButton->setSelected(sTg4); return false; }

/* 点「无线网络」名字行 → 跳系统自带 WiFi 设置页 */
static bool onButtonClick_WifiEnter(ZKButton* pButton) {
    (void)pButton;
    EASYUICONTEXT->openActivity("WifiSettingActivity");
    return false;
}

/* 点「系统设置」 → 跳系统内置设置页（网络/WiFi/热点/语言/触摸校准/开发者选项 总入口） */
static bool onButtonClick_SysSet(ZKButton* pButton) {
    (void)pButton;
    EASYUICONTEXT->openActivity("ZKSettingActivity");
    return false;
}

/* 外观分段：深色/浅色 互斥 */
static bool onButtonClick_SegDark(ZKButton* pButton) {
    sDark = true; mSegDarkPtr->setSelected(true); mSegLightPtr->setSelected(false); return false;
}

static bool onButtonClick_SegLight(ZKButton* pButton) {
    sDark = false; mSegDarkPtr->setSelected(false); mSegLightPtr->setSelected(true); return false;
}

static bool onButtonClick_BtnSave(ZKButton* pButton) {
	return false;
}

static bool onButtonClick_BtnReset(ZKButton* pButton) {
	return false;
}static bool onButtonClick_NavBtn1(ZKButton* pButton) { return zkNavGo(0); }

static bool onButtonClick_NavBtn2(ZKButton* pButton) { return zkNavGo(1); }

static bool onButtonClick_NavBtn3(ZKButton* pButton) { return zkNavGo(2); }

static bool onButtonClick_NavBtn4(ZKButton* pButton) { return zkNavGo(3); }

static bool onButtonClick_NavBtn5(ZKButton* pButton) { return zkNavGo(4); }

static bool onButtonClick_NavBtn6(ZKButton* pButton) { return zkNavGo(5); }

static bool onButtonClick_NavBtn7(ZKButton* pButton) { return zkNavGo(6); }

static bool onButtonClick_NavBtn8(ZKButton* pButton) { return zkNavGo(7); }


