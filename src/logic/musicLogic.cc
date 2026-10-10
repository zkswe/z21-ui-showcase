#include <base/base.h>
#ifdef FUN_BUILD
#include GENERATED_UI_DEFINITIONS
INIT_UI_EVENT_BINDINGS
#endif // FUN_BUILD
#pragma once
#include "uart/ProtocolSender.h"
#include "ui/zk_ui_nav.h"
#include "zk/zk_page_anim.h"

/* music 页逻辑：演示数据 + 进度/歌词（音频输出待板子侧解决；见 memory 2026-10-09）
 * 已按官方 MusicDemo 口径试过 ZKMediaPlayer -> 本板 audio device enable error（音频设备起不来） */

static S_ACTIVITY_TIMEER REGISTER_ACTIVITY_TIMER_TAB[] = {
    {9, 16},   /* page anim */
    {0, 200},
};

static const char *kSong[][2] = {
    {"夜曲", "周杰伦 · 十一月的萧邦"},
    {"晴天", "周杰伦 · 叶惠美"},
    {"稻香", "周杰伦 · 魔杰座"},
};
static const char *kLyric[][3] = {
    {"为你弹奏萧邦的夜曲", "纪念我死去的爱情", "跟夜风一样的声音"},
    {"故事的小黄花", "从出生那年就飘着", "童年的荡秋千"},
    {"对这个世界如果你有太多的抱怨", "跌倒了就不敢继续往前走", "为什么人要这么的脆弱 堕落"},
};
static const int kDurMs = 246000;

static int  sIdx = 0;
static bool sPlaying = true;
static int  sMs = 42000;

static void muApplySong() {
    mSongNamePtr->setText(kSong[sIdx][0]);
    mSongArtistPtr->setText(kSong[sIdx][1]);
    mLyricPrevPtr->setText(kLyric[sIdx][0]);
    mLyricCurPtr->setText(kLyric[sIdx][1]);
    mLyricNextPtr->setText(kLyric[sIdx][2]);
}

static void muApplyProgress() {
    char buf[24];
    mMuProgressPtr->setProgress(sMs * 100 / kDurMs);
    snprintf(buf, sizeof(buf), "%02d:%02d", sMs / 60000, (sMs / 1000) % 60);
    mMuTimeNowPtr->setText(buf);
    snprintf(buf, sizeof(buf), "%02d:%02d", kDurMs / 60000, (kDurMs / 1000) % 60);
    mMuTimeAllPtr->setText(buf);
}

static void zkAnimInit() {
    zk::PageAnim::init(9, 16);        /* 与 REGISTER_ACTIVITY_TIMER_TAB 的 {9,16} 对应 */
    zk::PageAnim::setSlideAlphaEnable(false);   /* 本机 setAlpha 不重绘：只做位移 */
    zk::PageAnim::addSlideTop2Bottom(mMuBackPtr, 400, 0);
    zk::PageAnim::addSlideLeft2Right(mCoverPtr, 520, 60);
    zk::PageAnim::addSlideLeft2Right(mSongNamePtr, 460, 220);
    zk::PageAnim::addSlideLeft2Right(mSongArtistPtr, 460, 220);
    zk::PageAnim::addSlideLeft2Right(mLyricPrevPtr, 440, 300);
    zk::PageAnim::addSlideLeft2Right(mLyricCurPtr, 440, 300);
    zk::PageAnim::addSlideLeft2Right(mLyricNextPtr, 440, 300);
    zk::PageAnim::addSlideLeft2Right(mMuProgressPtr, 440, 380);
    zk::PageAnim::addSlideLeft2Right(mBtn_prevPtr, 420, 460);
    zk::PageAnim::addSlideLeft2Right(mBtn_playPtr, 420, 460);
    zk::PageAnim::addSlideLeft2Right(mBtn_nextPtr, 420, 460);
    zk::PageAnim::start();
}

static void onUI_init() {
    zkAnimInit();
  #ifdef FUN_BUILD
  INIT_UI_TIMERS
  #endif // FUN_BUILD
    muApplySong();
    muApplyProgress();
    mBtn_playPtr->setSelected(true);
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
        if (sPlaying) { sMs += 200; if (sMs >= kDurMs) sMs = 0; }
        muApplyProgress();
    }
    return true;
}

static bool onmusicActivityTouchEvent(const MotionEvent &ev) {
    return zkNavTouch(6, ev);
}

static bool onButtonClick_Btn_prev(ZKButton *pButton) {
    sIdx = (sIdx + 2) % 3; sMs = 0; muApplySong(); muApplyProgress(); return false;
}
static bool onButtonClick_Btn_play(ZKButton *pButton) {
    sPlaying = !sPlaying; pButton->setSelected(sPlaying); return false;
}
static bool onButtonClick_Btn_next(ZKButton *pButton) {
    sIdx = (sIdx + 1) % 3; sMs = 0; muApplySong(); muApplyProgress(); return false;
}
static bool onButtonClick_Btn_heart(ZKButton *pButton) {
    static bool fav = false; fav = !fav; pButton->setSelected(fav); return false;
}
static bool onButtonClick_Btn_volume(ZKButton *pButton) { return false; }

static void onProgressChanged_MuProgress(ZKSeekBar *pSeekBar, int progress) {
    sMs = progress * kDurMs / 100;
    muApplyProgress();
}static bool onButtonClick_NavBtn1(ZKButton* pButton) { return zkNavGo(0); }

static bool onButtonClick_NavBtn2(ZKButton* pButton) { return zkNavGo(1); }

static bool onButtonClick_NavBtn3(ZKButton* pButton) { return zkNavGo(2); }

static bool onButtonClick_NavBtn4(ZKButton* pButton) { return zkNavGo(3); }

static bool onButtonClick_NavBtn5(ZKButton* pButton) { return zkNavGo(4); }

static bool onButtonClick_NavBtn6(ZKButton* pButton) { return zkNavGo(5); }

static bool onButtonClick_NavBtn7(ZKButton* pButton) { return zkNavGo(6); }

static bool onButtonClick_NavBtn8(ZKButton* pButton) { return zkNavGo(7); }


