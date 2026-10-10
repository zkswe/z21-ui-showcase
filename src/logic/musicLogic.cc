#include <base/base.h>
#ifdef FUN_BUILD
#include GENERATED_UI_DEFINITIONS
INIT_UI_EVENT_BINDINGS
#endif // FUN_BUILD
#pragma once
#include "uart/ProtocolSender.h"
#include "ui/zk_ui_nav.h"
#include "zk/zk_page_anim.h"
#include "zk/zk_fft.h"
#include <media_player.h>
#include <audio_manager.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>

/* ============================================================================
 * music 页：真实音频播放 + 黑胶唱片旋转 + 实时频谱
 *
 * 播放：base::MediaPlayer（audio-utility 包）。Z21 是 SigmaStar 平台，输出走
 *       MI_AO 通道（不是 ALSA）；音量/静音走 base::AudioManager（Z21 取值 0~50）。
 *       ⚠️ audio-utility 内部解码用 ffmpeg：工程 Manifest 除了 audio-utility
 *          还必须显式加 ffmpeg（包自身没声明这个依赖），否则链接报 av_ 与 swr_ 系列未定义。
 *
 * 频谱：MediaPlayer 的 onAudio 回调**直接给解码后的 PCM** → 写环形缓冲 →
 *       定时器里做 1024 点 radix-2 FFT → 分 28 个对数频带 → 每带自适应归一化
 *       → painter 画「频带竖线 + 光晕 + 曲线」。**真·实时**，任意音乐都能出谱。
 *       （回调在库线程：只做拷贝，绝不做 FFT/绘制。）
 *
 * 音频素材：resources/music/*.mp3 随工程资源部署到 /mnt/extsd/ui/，
 *          即实际路径 /mnt/extsd/ui/music/*.mp3；另依次回退 /mnt/extsd/music/、/mnt/usb1/。
 *
 * 唱片：pointer 控件（pointerPic = 黑胶图），定时器驱动 setTargetAngle 匀速旋转，
 *       暂停即停；三首歌各一张唱片，按可见性切换。
 * ========================================================================== */

/* ------------------------------ 曲目表 ------------------------------ */

struct MuTrack {
    const char *title;     /* 兜底显示名（文件带 ID3 时用 ID3） */
    const char *artist;
    const char *file;      /* 文件名，配合下面的目录候选解析 */
    int         lyrAt[4];  /* 每句歌词起始时间 ms */
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

static const int kSimDurMs = 60000;   /* 无法播放时的演示时长 */

static const char *kDirs[] = {
    "/mnt/extsd/ui/music/",   /* 随工程资源部署（resources/music/） */
    "/mnt/extsd/music/",      /* 用户自备 */
    "/mnt/usb1/music/",
    "/mnt/usb1/",
};

static char sPath[3][128];
static bool sHaveFile[3];

/* ------------------------------ 播放状态 ------------------------------ */

static base::MediaPlayer *sPlayer = nullptr;
static int   sIdx = 0;            /* 当前曲目 */
static bool  sPlaying = false;    /* UI 层“正在播放” */
static bool  sUserPaused = false; /* 用户主动暂停（离开页面回来不自动续播） */
static int   sPosMs = 0;
static int   sSimMs = 0;
static int   sDurMs[3];
static float sAngle = 0.0f;       /* 唱片角度 */
static bool  sMuted = false;
static int   sVolume = 38;        /* Z21: 0~50 */
static bool  sId3Done = false;    /* 本曲是否已取过 ID3 */

static S_ACTIVITY_TIMEER REGISTER_ACTIVITY_TIMER_TAB[] = {
    {9, 16},   /* page anim */
    {1, 40},   /* 唱片旋转 + 频谱 */
    {0, 200},  /* 进度 / 歌词 */
};

/* ------------------------------ PCM 环形缓冲 ------------------------------ */

#define kPcmRingBits 15
#define kPcmRing     (1 << kPcmRingBits)     /* 32768 个单声道样本 */
#define kFftN        1024

static float sPcmRing[kPcmRing];
static volatile int sPcmW = 0;            /* 写指针（样本数） */
static volatile long sPcmBytes = 0;       /* 累计收到字节（探测格式用） */
static volatile long sPcmCalls = 0;
static int   sPcmCh = 1;                  /* PCM 声道数：实测回调 2304B/帧 = 1152 采样 = 单声道 */
static int   sPcmRate = 44100;            /* PCM 采样率：实测约 87.5 KB/s = mono s16 @ 44.1k */

static ZkFft  sFft(kFftN);
static float  sHann[kFftN];
static float  sFftIn[kFftN];
static float  sMag[kFftN / 2 + 1];

static const int kSpecBands = 28;
static float sBandRef[32];                /* 每带自适应参考（dB，慢衰减） */
static float sSpecDisp[32];               /* 显示值（0~1，平滑） */
static const int kSpecW = 520;            /* 与 ui 里 painter__1 尺寸一致 */
static const int kSpecH = 44;

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

/* ------------------------------ 频谱 ------------------------------ */

static void muSpecReset() {
    for (int i = 0; i < 32; ++i) {
        sBandRef[i] = -100.0f;
        sSpecDisp[i] = 0.0f;
    }
    sPcmW = 0;
    sPcmBytes = 0;
    sPcmCalls = 0;
}

static void muDrawSpecIdle() {
    if (!mSpecPtr) return;
    SZKPoint base[2];
    base[0].x = 0.0f;                base[0].y = (float)(kSpecH - 1);
    base[1].x = (float)(kSpecW - 1); base[1].y = (float)(kSpecH - 1);
    mSpecPtr->erase(0, 0, kSpecW, kSpecH);
    mSpecPtr->setLineWidth(1);
    mSpecPtr->setSourceColor(0x5E2730);
    mSpecPtr->drawLines(base, 2);
}

static void muDrawSpec() {
    if (!mSpecPtr) return;
    const unsigned int kColBody = 0xEC4141;   /* 主色（同页面强调色） */
    const unsigned int kColBar  = 0x5E2730;   /* 频带竖线（暗） */
    const unsigned int kColGlow = 0x3A1519;   /* 曲线光晕 */

    SZKPoint pts[32];
    for (int i = 0; i < kSpecBands; ++i) {
        pts[i].x = (float)(i * (kSpecW - 1)) / (float)(kSpecBands - 1);
        pts[i].y = (float)(kSpecH - 2) - sSpecDisp[i] * (float)(kSpecH - 6);
    }

    mSpecPtr->erase(0, 0, kSpecW, kSpecH);
    mSpecPtr->setLineWidth(1);                    /* 1) 频带竖线 */
    mSpecPtr->setSourceColor(kColBar);
    for (int i = 0; i < kSpecBands; ++i) {
        SZKPoint v[2];
        v[0] = pts[i]; v[0].y = (float)(kSpecH - 1);
        v[1] = pts[i];
        mSpecPtr->drawLines(v, 2);
    }
    mSpecPtr->setSourceColor(kColGlow);           /* 2) 光晕 */
    mSpecPtr->setLineWidth(6);
    mSpecPtr->drawCurve(pts, kSpecBands);
    mSpecPtr->setSourceColor(kColBody);           /* 3) 曲线本体 */
    mSpecPtr->setLineWidth(2);
    mSpecPtr->drawCurve(pts, kSpecBands);
}

/* PCM 回调（库线程）：只做下混 + 入环形缓冲 */
static void muPushPcm(const uint8_t *data, int length) {
    sPcmBytes += length;
    sPcmCalls++;
    if (!data || length < 2) return;
    const short *s = (const short *) data;
    int n = length / 2;
    int w = sPcmW;
    if (sPcmCh >= 2) {
        for (int i = 0; i + 1 < n; i += 2) {
            float v = (float)(s[i] + s[i + 1]) * 0.5f;
            sPcmRing[(w++) & (kPcmRing - 1)] = v;
        }
    } else {
        for (int i = 0; i < n; ++i) {
            sPcmRing[(w++) & (kPcmRing - 1)] = (float) s[i];
        }
    }
    sPcmW = w;
}

/* 定时器里跑：取最近 kFftN 个样本 → FFT → 28 频带 → 自适应归一 */
static void muAnalyzeSpec() {
    if (sPcmW < kFftN) { muDrawSpecIdle(); return; }

    for (int i = 0; i < kFftN; ++i) {
        int idx = (sPcmW - kFftN + i) & (kPcmRing - 1);
        sFftIn[i] = sPcmRing[idx];
    }
    sFft.magnitude(sFftIn, sHann, sMag);

    const float rate = (float) sPcmRate;
    float fLo = 60.0f;
    float fHi = rate * 0.45f;
    if (fHi > 7000.0f) fHi = 7000.0f;
    const float ratio = fHi / fLo;

    for (int b = 0; b < kSpecBands; ++b) {
        float f0 = fLo * powf(ratio, (float) b / kSpecBands);
        float f1 = fLo * powf(ratio, (float)(b + 1) / kSpecBands);
        int i0 = (int)(f0 * kFftN / rate);
        int i1 = (int)(f1 * kFftN / rate);
        if (i0 < 1) i0 = 1;
        if (i1 <= i0) i1 = i0 + 1;
        if (i1 > kFftN / 2) i1 = kFftN / 2;

        float sum = 0.0f;
        float peak = 0.0f;
        for (int i = i0; i < i1; ++i) {
            sum += sMag[i];
            if (sMag[i] > peak) peak = sMag[i];
        }
        float m = 0.6f * peak + 0.4f * (sum / (float)(i1 - i0));   /* 峰值为主：形状更清脆 */
        float db = 20.0f * log10f(m + 1e-6f);
        db += (float)(kSpecBands - b) * 0.30f;                    /* 低频抬一点：保留音频自然斜率 */

        if (db > sBandRef[b]) sBandRef[b] = db;   /* 自适应参考：跟峰、快衰减（起伏更明显） */
        else sBandRef[b] -= 1.0f;

        float v = (db - (sBandRef[b] - 26.0f)) / 26.0f;
        if (v < 0.0f) v = 0.0f;
        if (v > 1.0f) v = 1.0f;
        v = powf(v, 0.85f);
        sSpecDisp[b] += (v - sSpecDisp[b]) * 0.42f;   /* 轻平滑 */
    }
    muDrawSpec();
}

/* ------------------------------ 播放控制 ------------------------------ */

static void muNext(int step);

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
    sPlaying = true;
    sUserPaused = false;
    sId3Done = false;

    muSpecReset();                   /* 换曲目：频谱从零起 */
    muApplyDisc();
    muApplyTrackText();
    muApplyProgress();
    muApplyLyric();
    muDrawSpecIdle();

    if (sPlayer && sHaveFile[sIdx]) {
        if (fromMs > 0) {
            sPlayer->play(sPath[sIdx]);
            sPlayer->seekTo(fromMs);
        } else {
            sPlayer->play(sPath[sIdx]);
        }
    } else if (sPlayer) {
        sPlayer->stop();             /* 无素材：退化为演示进度 */
    }
}

static void muNext(int step) {
    muPlayFrom(sIdx + step, 0);
}

static void muInitPlayer() {
    if (sPlayer) return;
    sPlayer = new base::MediaPlayer();
    sPlayer->onCompletion([]() {
        LOGD("music: completed -> next");
        muNext(1);
    });
    sPlayer->onError([](int what, const std::string &str) {
        LOGD("music: player error %#x %s", what, str.c_str());
    });
    sPlayer->onAudio([](uint8_t *data, int length) -> int {
        muPushPcm(data, length);     /* 库线程：只做拷贝 */
        return 0;
    });

    base::AudioManager &am = base::AudioManager::instance();
    am.setVolume(sVolume);           /* Z21: 0~50 */
    am.setIdleTimeout(3000);         /* 空闲关声，避免底噪 */
}

/* 进度条：拖动中记录，松手才 seek */
class MuSeekListener : public ZKSeekBar::ISeekBarChangeListener {
public:
    virtual void onProgressChanged(ZKSeekBar *pSeekBar, int progress) {
        if (sTracking) sTarget = progress;
    }
    virtual void onStartTrackingTouch(ZKSeekBar *pSeekBar) {
        sTracking = true;
        sTarget = -1;
    }
    virtual void onStopTrackingTouch(ZKSeekBar *pSeekBar) {
        sTracking = false;
        int dur = sDurMs[sIdx] > 0 ? sDurMs[sIdx] : kSimDurMs;
        if (sTarget >= 0) {
            int ms = sTarget * dur / 100;
            if (sPlayer && sHaveFile[sIdx]) sPlayer->seekTo(ms);
            sSimMs = ms;
            sPosMs = ms;
            muApplyProgress();
            muApplyLyric();
            muSpecReset();
            muDrawSpecIdle();
        }
        sTarget = -1;
    }
private:
    static bool sTracking;
    static int  sTarget;
};
bool MuSeekListener::sTracking = false;
int  MuSeekListener::sTarget = -1;
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
    zk::PageAnim::addSlideLeft2Right(mSpecPtr, 440, 380);
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
    muInitPlayer();
    zkHann(sHann, kFftN);
    muSpecReset();

    /* 进度条拖动 seek */
    if (mMuProgressPtr) mMuProgressPtr->setSeekBarChangeListener(&sSeekListener);

    sAngle = 0.0f;
    muApplyDisc();
    muApplyTrackText();
    muApplyProgress();
    muApplyLyric();
    muDrawSpecIdle();
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

    if (!sPlaying && !sUserPaused) {
        muPlayFrom(sIdx, 0);
    }
}

static void onUI_hide() {
    /* 离开本页停声（唱片停转）；回来时自动续播 */
    if (sPlayer && sPlaying) {
        sPlayer->pause();
    }
    sPlaying = false;
    muApplyTrackText();
}

static void onUI_quit() {
    zk::PageAnim::release();
    if (sPlayer) {
        sPlayer->stop();
        delete sPlayer;
        sPlayer = nullptr;
    }
}

static void onProtocolDataUpdate(const SProtocolData &data) {}

static bool onUI_Timer(int id) {
    zk::PageAnim::onTimer(id);

    if (id == 1) {                      /* 唱片旋转 + 频谱 */
        if (sPlaying) {
            sAngle += 7.0f;             /* 40ms × 7° ≈ 2.0 s/圈 */
            if (sAngle >= 360.0f) sAngle -= 360.0f;
            muApplyAngle();
            muAnalyzeSpec();
        }
    } else if (id == 0) {               /* 进度 / 歌词 */
        if (sPlaying) {
            if (sPlayer && sHaveFile[sIdx]) {
                double pos = sPlayer->currentPositon();
                double dur = sPlayer->duration();
                if (pos >= 0) sPosMs = (int)(pos * 1000.0);
                if (dur > 0) sDurMs[sIdx] = (int)(dur * 1000.0);
                if (!sId3Done) {        /* 取一次 ID3 信息 */
                    sId3Done = true;
                    base::MediaPlayer::AudioInfo info;
                    sPlayer->getAudioInfo(&info);
                    LOGD("music: id3 title='%s' artist='%s' album='%s'",
                         info.title.c_str(), info.artist.c_str(), info.album.c_str());
                }
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
        if (sPlayer) sPlayer->pause();
        sPlaying = false;
        sUserPaused = true;
        pButton->setSelected(false);
    } else {                             /* 继续（或从头开始） */
        if (sPlayer) {
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
    sMuted = !sMuted;                    /* 真音量：AudioManager */
    base::AudioManager::instance().setMute(sMuted);
    pButton->setSelected(sMuted);
    return false;
}

/* 进度条默认回调（已装自定义监听，这里保留空实现） */
static void onProgressChanged_MuProgress(ZKSeekBar *pSeekBar, int progress) {}

static bool onButtonClick_NavBtn1(ZKButton* pButton) { return zkNavGo(0); }

static bool onButtonClick_NavBtn2(ZKButton* pButton) { return zkNavGo(1); }

static bool onButtonClick_NavBtn3(ZKButton* pButton) { return zkNavGo(2); }

static bool onButtonClick_NavBtn4(ZKButton* pButton) { return zkNavGo(3); }

static bool onButtonClick_NavBtn5(ZKButton* pButton) { return zkNavGo(4); }

static bool onButtonClick_NavBtn6(ZKButton* pButton) { return zkNavGo(5); }

static bool onButtonClick_NavBtn7(ZKButton* pButton) { return zkNavGo(6); }

static bool onButtonClick_NavBtn8(ZKButton* pButton) { return zkNavGo(7); }
