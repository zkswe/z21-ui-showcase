#include <base/base.h>
#ifdef FUN_BUILD
#include GENERATED_UI_DEFINITIONS
INIT_UI_EVENT_BINDINGS
#endif // FUN_BUILD
#pragma once
#include "uart/ProtocolSender.h"
#include "ui/zk_ui_nav.h"
#include "zk/zk_page_anim.h"

/* charts 页：实时"心电图"波形（单点 addData 追加，逐点流动）
 * 2026-10-09 需求方口径：addData = 每次追加一个点（setData 才是整体重设）；
 * 单点追加 + 高频 -> 才能做出心电图那样"每次更新一个数据"的效果。*/

static S_ACTIVITY_TIMEER REGISTER_ACTIVITY_TIMER_TAB[] = {
    {9, 16},   /* page anim */
    {0, 10},    /* 每 10ms 追加 1 点（≈100 点/秒） */
    {1, 150},   /* 环/仪表 */
};

/* 心电图一个周期（64 点，0..100）：基线 + P波 + QRS + T波 */
static const int kEcg[64] = {
    50, 50, 50, 50, 50, 50, 50, 50,   /* 基线 */
    50, 51, 53, 56, 58, 56, 53, 51,   /* P 波 */
    50, 50, 49, 47, 44, 41,           /* 回到基线 + Q 下凹 */
    96,                               /* R 尖峰 */
    30, 44,                              /* S 下凹 */
    49, 50, 51, 53, 56, 58, 60, 62, 60, 58, 56, 53, 51, 50,  /* T 波 */
    50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50,
};

static int sWave = 0;
static int sWavePrimed = 0;   /* 首帧预铺标记（等入场动画结束再画） */

static void zkAnimInit() {
    zk::PageAnim::init(9, 16);        /* 与 REGISTER_ACTIVITY_TIMER_TAB 的 {9,16} 对应 */
    zk::PageAnim::setSlideAlphaEnable(false);   /* 本机 setAlpha 不重绘：只做位移 */
    zk::PageAnim::addSlideTop2Bottom(mTitleTextPtr, 420, 0);
    zk::PageAnim::addSlideTop2Bottom(mTitleSubPtr, 420, 60);
    zk::PageAnim::addSlideBottom2Top(mNavBarPtr, 460, 40);
    zk::PageAnim::addSlideLeft2Right(mCardLinePtr, 460, 140);
    zk::PageAnim::addSlideRight2Left(mCardRingPtr, 460, 190);
    zk::PageAnim::addSlideLeft2Right(mCardGaugePtr, 460, 240);
    zk::PageAnim::addSlideRight2Left(mCardBarPtr, 460, 290);
    /* 圆弧推进等卡片落位后再跑（避免底图没到位就开始动） */
    zk::PageAnim::addCircleBarProgress(mRingBarPtr, 71, 1000, 900);
    zk::PageAnim::addCircleBarProgress(mGaugeBarPtr, 33, 1000, 980);
    zk::PageAnim::start();
}

static void onUI_init() {
    zkAnimInit();
  #ifdef FUN_BUILD
  INIT_UI_TIMERS
  #endif // FUN_BUILD
}

static void onUI_intent(const Intent *intentPtr) {}
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

static void onUI_hide() {}
static void onUI_quit() { zk::PageAnim::release();}
static void onProtocolDataUpdate(const SProtocolData &data) {}

static bool onUI_Timer(int id) {
    zk::PageAnim::onTimer(id);
    if (id == 0) {
        if (zk::PageAnim::isRunning()) return true;   /* 等入场动画结束（卡片/底图到位）再画波形 */
        if (!sWavePrimed) {                           /* 首帧预铺一屏，避免从空白开始 */
            sWavePrimed = 1;
            for (int i = 0; i < 110; ++i) {
                mWaveChartPtr->addData(0, (float) kEcg[sWave % 64]);
                ++sWave;
            }
            return true;
        }
        mWaveChartPtr->addData(0, (float) kEcg[sWave % 64]);   /* 单点追加 */
        ++sWave;
    }
    /* id==1 的圆弧推进已交给 PageAnim（addCircleBarProgress），这里不再驱动 */
    return true;
}

static bool onchartsActivityTouchEvent(const MotionEvent &ev) {
    return zkNavTouch(2, ev);
}static bool onButtonClick_NavBtn1(ZKButton* pButton) { return zkNavGo(0); }

static bool onButtonClick_NavBtn2(ZKButton* pButton) { return zkNavGo(1); }

static bool onButtonClick_NavBtn3(ZKButton* pButton) { return zkNavGo(2); }

static bool onButtonClick_NavBtn4(ZKButton* pButton) { return zkNavGo(3); }

static bool onButtonClick_NavBtn5(ZKButton* pButton) { return zkNavGo(4); }

static bool onButtonClick_NavBtn6(ZKButton* pButton) { return zkNavGo(5); }

static bool onButtonClick_NavBtn7(ZKButton* pButton) { return zkNavGo(6); }

static bool onButtonClick_NavBtn8(ZKButton* pButton) { return zkNavGo(7); }


