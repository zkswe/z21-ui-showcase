#include <base/base.h>
#ifdef FUN_BUILD
#include GENERATED_UI_DEFINITIONS
INIT_UI_EVENT_BINDINGS
#endif // FUN_BUILD
#pragma once
#include "uart/ProtocolSender.h"
#include "ui/zk_ui_nav.h"
#include "zk/zk_page_anim.h"
#include "media/ZKMediaPlayer.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>

/* ============================================================================
 * music 页：真实音频播放 + 黑胶唱片旋转
 *
 * 播放：ZKMediaPlayer(E_MEDIA_TYPE_AUDIO)。Z21 是 SigmaStar 平台，音频输出走
 *       MI_AO 通道（设备 /dev/mi_ao，由 /lib/libzkmedia.so 驱动 libmad 软解 MP3），
 *       **不是** ALSA —— 所以不要按"ALSA 声卡"的思路排查。
 *
 * 音频素材：resources/music/*.mp3 随工程资源一起部署到设备
 *          （资源目录 → /mnt/extsd/ui/，故实际路径 /mnt/extsd/ui/music/*.mp3）。
 *          也会依次回退到 /mnt/extsd/music/、/mnt/usb1/ 找同名文件。
 *          一个都找不到时退化为"演示进度"（界面照常可用），并在日志里提示。
 *
 * 唱片：pointer 控件（pointerPic = 黑胶图），调用方定时器驱动 setTargetAngle
 *      持续累加角度 → 匀速旋转；暂停即停转。三首歌各一张唱片，按可见性切换。
 * ========================================================================== */

static ZKMediaPlayer *sPlayer = nullptr;
static int   sIdx = 0;            /* 当前曲目 */
static bool  sPlaying = false;    /* UI 层"正在播放" */
static bool  sUserPaused = false; /* 用户主动暂停（离开页面回来不自动续播） */
static bool  sRealAudio = false;  /* 当前是否真在播放器上播（否则为演示进度） */
static int   sPosMs = 0;          /* 当前进度（真播取播放器位置） */
static int   sSimMs = 0;          /* 演示进度 */
static float sAngle = 0.0f;       /* 唱片角度 */
static float sVolume = 0.7f;
static bool  sMuted = false;

/* ---- 曲目表（素材为本仓库程序化合成的 demo 曲，无版权问题） ---- */
struct MuTrack {
    const char *title;
    const char *artist;
    const char *file;      /* 文件名，配合下面的目录候选解析 */
    int         lyrAt[4];  /* 每句歌词起始时间 ms（与素材时长对齐） */
    const char *lyr[4];
};

static const MuTrack kTracks[3] = {
    {"Morning Walk", "FlyThings UI Demo", "demo_01.mp3", {0, 12000, 26000, 38000},
     {"A quiet street wakes in the light", "Coffee steam and slow footsteps", "Every window starts to glow", "Walk with me, we are in no hurry"}},
    {"Pixel Run", "FlyThings UI Demo", "demo_02.mp3", {0, 10000, 22000, 33000},
     {"Press start, the screen comes alive", "Eight bit colors on the run", "Jump the gaps and keep the beat", "One more round before the sun"}},
    {"Night Drive", "FlyThings UI Demo", "demo_03.mp3", {0, 14000, 30000, 44000},
     {"City lights behind the glass", "Radio low, the road is long", "Neon rivers passing by", "Drive until the night runs out"}},
};

static const int kSimDurMs = 60000;   /* 找不到音频文件时的演示时长 */

static const char *kDirs[] = {
    "/mnt/extsd/ui/music/",   /* 随工程资源部署（resources/music/） */
    "/mnt/extsd/music/",      /* 用户自备 */
    "/mnt/usb1/music/",
    "/mnt/usb1/",
};

static char sPath[3][128];
static bool sHaveFile[3];
static int  sDurMs[3];

static S_ACTIVITY_TIMEER REGISTER_ACTIVITY_TIMER_TAB[] = {
    {9, 16},   /* page anim */
    {1, 40},   /* 唱片旋转 */
    {0, 200},  /* 进度 / 歌词 */
};

/* ------------------------------ 界面刷新 ------------------------------ */

static void muApplyDisc() {
    if (mDisc1Ptr) mDisc1Ptr->setVisible(sIdx == 0);
    if (mDisc2Ptr) mDisc2Ptr->setVisible(sIdx == 1);
    if (mDisc3Ptr) mDisc3Ptr->setVisible(sIdx == 2);
}

static void muApplyAngle() {
    ZKPointer *p = (sIdx == 0) ? mDisc1Ptr : (sIdx == 1) ? mDisc2Ptr : mDisc3Ptr;
    if (p) p->setTargetAngle(sAngle);
}

static void muApplyTrackText() {
    const MuTrack &t = kTracks[sIdx];
    if (mSongNamePtr)   mSongNamePtr->setText(t.title);
    if (mSongArtistPtr) mSongArtistPtr->setText(t.artist);
    if (mBtn_playPtr)   mBtn_playPtr->setSelected(sPlaying);
}

static void muApplyLyric() {
    const MuTrack &t = kTracks[sIdx];
    int cur = 0;
    for (int i = 0; i < 4; ++i) {
        if (sPosMs >= t.lyrAt[i]) cur = i;
    }
    if (mLyricPrevPtr) mLyricPrevPtr->setText(cur > 0 ? t.lyr[cur - 1] : "");
    if (mLyricCurPtr)  mLyricCurPtr->setText(t.lyr[cur]);
    if (mLyricNextPtr) mLyricNextPtr->setText(cur < 3 ? t.lyr[cur + 1] : "");
}

static void muApplyProgress() {
    int dur = sDurMs[sIdx] > 0 ? sDurMs[sIdx] : kSimDurMs;
    int pos = sPosMs > dur ? dur : sPosMs;
    char buf[24];
    if (mMuProgressPtr) mMuProgressPtr->setProgress(pos * 100 / dur);
    snprintf(buf, sizeof(buf), "%02d:%02d", pos / 60000, (pos / 1000) % 60);
    if (mMuTimeNowPtr) mMuTimeNowPtr->setText(buf);
    snprintf(buf, sizeof(buf), "%02d:%02d", dur / 60000, (dur / 1000) % 60);
    if (mMuTimeAllPtr) mMuTimeAllPtr->setText(buf);
}

/* ------------------------------ 播放控制 ------------------------------ */

static void muResolveFiles() {
    for (int i = 0; i < 3; ++i) {
        sHaveFile[i] = false;
        sPath[i][0] = '\0';
        for (unsigned d = 0; d < sizeof(kDirs) / sizeof(kDirs[0]); ++d) {
            snprintf(sPath[i], sizeof(sPath[i]), "%s%s", kDirs[d], kTracks[i].file);
            if (access(sPath[i], F_OK) == 0) { sHaveFile[i] = true; break; }
        }
        LOGD("music: %s -> %s", kTracks[i].file, sHaveFile[i] ? sPath[i] : "(not found, demo mode)");
    }
}

static void muPlayFrom(int idx, int fromMs) {
    sIdx = ((idx % 3) + 3) % 3;
    sPosMs = fromMs;
    sSimMs = fromMs;
    sRealAudio = false;
    sPlaying = true;
    sUserPaused = false;

    muApplyDisc();
    muApplyTrackText();
    muApplyProgress();
    muApplyLyric();

    if (sPlayer && sHaveFile[sIdx]) {
        sPlayer->play(sPath[sIdx], fromMs);   /* 真正开始播放 */
    } else if (sPlayer) {
        sPlayer->stop();                      /* 无素材：退化为演示进度 */
    }
}

static void muNext(int step) {
    muPlayFrom(sIdx + step, 0);
}

/* 播放器消息（在播放器线程回调） */
class MuPlayerListener : public ZKMediaPlayer::IPlayerMessageListener {
public:
    virtual void onPlayerMessage(ZKMediaPlayer *pMediaPlayer, int msg, void *pMsgData) {
        switch (msg) {
        case ZKMediaPlayer::E_MSGTYPE_PLAY_STARTED:
            sRealAudio = true;
            if (pMediaPlayer) {
                int d = pMediaPlayer->getDuration();
                if (d > 0) sDurMs[sIdx] = d;   /* 真时长（不再写死） */
            }
            sPlaying = true;
            muApplyTrackText();
            muApplyProgress();
            muApplyLyric();
            LOGD("music: playing #%d (%s)", sIdx + 1, sPath[sIdx]);
            break;

        case ZKMediaPlayer::E_MSGTYPE_PLAY_COMPLETED:
            LOGD("music: completed -> next");
            muNext(1);
            break;

        case ZKMediaPlayer::E_MSGTYPE_ERROR_INVALID_FILEPATH:
        case ZKMediaPlayer::E_MSGTYPE_ERROR_MEDIA_ERROR:
        case ZKMediaPlayer::E_MSGTYPE_ERROR_PREPARE_FAIL:
        case ZKMediaPlayer::E_MSGTYPE_ERROR_PLAY_FAIL:
            LOGD("music: player error msg=%d, fallback to demo progress", msg);
            sRealAudio = false;
            sSimMs = 0;
            break;

        default:
            break;
        }
    }
};
static MuPlayerListener sPlayerListener;

/* 进度条拖动（拖动中只记录，松手才 seek） */
class MuSeekListener : public ZKSeekBar::ISeekBarChangeListener {
public:
    virtual void onProgressChanged(ZKSeekBar *pSeekBar, int progress) {
        if (sTracking) sSeekTo = progress;
    }
    virtual void onStartTrackingTouch(ZKSeekBar *pSeekBar) {
        sTracking = true;
        sSeekTo = -1;
    }
    virtual void onStopTrackingTouch(ZKSeekBar *pSeekBar) {
        sTracking = false;
        int dur = sDurMs[sIdx] > 0 ? sDurMs[sIdx] : kSimDurMs;
        if (sSeekTo >= 0) {
            int ms = sSeekTo * dur / 100;
            if (sRealAudio && sPlayer) sPlayer->seekTo(ms);
            sSimMs = ms;
            sPosMs = ms;
            muApplyProgress();
            muApplyLyric();
        }
        sSeekTo = -1;
    }
private:
    static bool sTracking;
    static int  sSeekTo;
};
bool MuSeekListener::sTracking = false;
int  MuSeekListener::sSeekTo = -1;
static MuSeekListener sSeekListener;

/* ------------------------------ 页面动画 ------------------------------ */

static void zkAnimInit() {
    zk::PageAnim::init(9, 16);        /* 与 REGISTER_ACTIVITY_TIMER_TAB 的 {9,16} 对应 */
    zk::PageAnim::setSlideAlphaEnable(false);   /* 本机 setAlpha 不重绘：只做位移 */
    zk::PageAnim::addSlideTop2Bottom(mMuBackPtr, 400, 0);
    zk::PageAnim::addSlideLeft2Right(mCoverPtr, 520, 60);
    zk::PageAnim::addSlideLeft2Right(mDisc1Ptr, 520, 60);
    zk::PageAnim::addSlideLeft2Right(mDisc2Ptr, 520, 60);
    zk::PageAnim::addSlideLeft2Right(mDisc3Ptr, 520, 60);
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

    muResolveFiles();

    if (!sPlayer) {
        sPlayer = new ZKMediaPlayer(ZKMediaPlayer::E_MEDIA_TYPE_AUDIO);
        sPlayer->setPlayerMessageListener(&sPlayerListener);
        sPlayer->setVolume(sVolume);
    }
    if (mMuProgressPtr) mMuProgressPtr->setSeekBarChangeListener(&sSeekListener);

    sAngle = 0.0f;
    muApplyDisc();
    muApplyTrackText();
    muApplyProgress();
    muApplyLyric();
}

static void onUI_intent(const Intent *intentPtr) {}

static bool sPageShown = false;   /* 首次显示标记：复用页才需要重挂定时器 */
static void onUI_show() {
    /* 复用页再进入：定时器**不会自动重挂** → 先重挂，否则动画推不动 */
    if (sPageShown) {
      #ifdef FUN_BUILD
      INIT_UI_TIMERS
      #endif // FUN_BUILD
    }
    sPageShown = true;
    if (zk::PageAnim::isRunning()) { zk::PageAnim::start(); } else { zkAnimInit(); }

    /* 回到本页继续播放（用户主动暂停过则不自动续播） */
    if (!sPlaying && !sUserPaused) {
        muPlayFrom(sIdx, sPosMs > 0 ? sPosMs : 0);
    } else if (sPlaying && sRealAudio && sPlayer && !sPlayer->isPlaying()) {
        sPlayer->resume();
    }
}

static void onUI_hide() {
    /* 离开本页停声（唱片停转）；回来时自动续播 */
    if (sPlayer && sRealAudio && sPlayer->isPlaying()) {
        sPlayer->pause();
    }
    sPlaying = false;
    muApplyTrackText();
}

static void onUI_quit() {
    zk::PageAnim::release();
    if (sPlayer) {
        sPlayer->setPlayerMessageListener(NULL);
        sPlayer->stop();
        delete sPlayer;
        sPlayer = nullptr;
    }
    if (mMuProgressPtr) mMuProgressPtr->setSeekBarChangeListener(NULL);
}

static void onProtocolDataUpdate(const SProtocolData &data) {}

static bool onUI_Timer(int id) {
    zk::PageAnim::onTimer(id);

    if (id == 1) {                      /* 唱片旋转：只在自己播时转 */
        if (sPlaying) {
            sAngle += 7.0f;             /* 40ms × 7° ≈ 2.0 s/圈 */
            if (sAngle >= 360.0f) sAngle -= 360.0f;
            muApplyAngle();
        }
    } else if (id == 0) {               /* 进度 / 歌词 */
        if (sPlaying) {
            if (sRealAudio && sPlayer) {
                int p = sPlayer->getCurrentPosition();
                if (p >= 0) sPosMs = p;
                int d = sPlayer->getDuration();
                if (d > 0) sDurMs[sIdx] = d;
            } else {
                sSimMs += 200;
                int dur = sDurMs[sIdx] > 0 ? sDurMs[sIdx] : kSimDurMs;
                if (sSimMs >= dur) sSimMs = 0;
                sPosMs = sSimMs;
            }
            muApplyProgress();
            muApplyLyric();
        }
    }
    return true;
}

static bool onmusicActivityTouchEvent(const MotionEvent &ev) {
    return zkNavTouch(6, ev);
}

/* ------------------------------ 事件回调 ------------------------------ */

static bool onButtonClick_Btn_prev(ZKButton *pButton) { muNext(-1); return false; }
static bool onButtonClick_Btn_next(ZKButton *pButton) { muNext(1);  return false; }

static bool onButtonClick_Btn_play(ZKButton *pButton) {
    if (sPlaying) {                      /* 暂停 */
        if (sPlayer && sRealAudio && sPlayer->isPlaying()) sPlayer->pause();
        sPlaying = false;
        sUserPaused = true;
        pButton->setSelected(false);
    } else {                             /* 继续 / 重新开始 */
        if (sPlayer && sRealAudio) {
            sPlayer->resume();
            sPlaying = true;
        } else {
            muPlayFrom(sIdx, sPosMs);
        }
        sUserPaused = false;
        pButton->setSelected(true);
    }
    return false;
}

static bool onButtonClick_Btn_heart(ZKButton *pButton) {
    static bool fav = false;
    fav = !fav;
    pButton->setSelected(fav);
    return false;
}

static bool onButtonClick_Btn_volume(ZKButton *pButton) {
    sMuted = !sMuted;                    /* 静音 / 恢复（真音量接口） */
    if (sPlayer) sPlayer->setVolume(sMuted ? 0.0f : sVolume);
    pButton->setSelected(sMuted);
    return false;
}

/* 保留（未装自定义监听时生效的默认进度回调） */
static void onProgressChanged_MuProgress(ZKSeekBar *pSeekBar, int progress) {}

static bool onButtonClick_NavBtn1(ZKButton* pButton) { return zkNavGo(0); }

static bool onButtonClick_NavBtn2(ZKButton* pButton) { return zkNavGo(1); }

static bool onButtonClick_NavBtn3(ZKButton* pButton) { return zkNavGo(2); }

static bool onButtonClick_NavBtn4(ZKButton* pButton) { return zkNavGo(3); }

static bool onButtonClick_NavBtn5(ZKButton* pButton) { return zkNavGo(4); }

static bool onButtonClick_NavBtn6(ZKButton* pButton) { return zkNavGo(5); }

static bool onButtonClick_NavBtn7(ZKButton* pButton) { return zkNavGo(6); }

static bool onButtonClick_NavBtn8(ZKButton* pButton) { return zkNavGo(7); }
