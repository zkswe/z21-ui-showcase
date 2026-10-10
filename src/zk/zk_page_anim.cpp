/*
 * zk_page_anim.cpp —— 页面「载入 / 退出」动画模块实现（FlyThings / EasyUI）
 * 头文件：include/zk/zk_page_anim.h（对外唯一面）
 *
 * 行为口径 = 原版 AnimationFramework（留档 ../reference/AnimationFramework.original.{h,cpp}）：
 *   · 滑入从**屏幕外**进来（控件整块 + 50px 余量）
 *   · **滑入自带淡入**（alpha 0 → 255）
 *   · 4 种缓动（Linear / OutCubic / OutQuart / InOutSine），cos 用泰勒展开
 *   · 数值动画只在数值变化时才 setText / setProgress
 *   · 进度超过控件 max 自动夹取
 * 本模块叠加：
 *   · 退出（退场）动画：addSlideOut* / addFadeOut + setExitCallback + startExit()
 *   · 计数单位 suffix、滑出位移可覆盖、屏幕尺寸可配
 *   · 载入 / 退出分组互不干扰
 *
 * ⚠️ 全程**不调用** ZKBase::invalidate()：部分设备 libeasyui 未导出该符号
 *    （装载期 undefined symbol）；setPosition/setAlpha/setText/setProgress 自身会重绘。
 */
#include "zk/zk_page_anim.h"

#include <vector>
#include <string>
#include <functional>
#include <cstdio>
#include <algorithm>

namespace zk {

namespace {

enum Kind {
    K_SLIDE = 0,   // 滑入 / 滑出（带淡入淡出）
    K_FADE,        // 原地淡入 / 淡出
    K_COUNTER,     // 数字累加
    K_CHARSET,     // 定宽补零累加
    K_SEEK,        // 进度条
    K_CIRCLE       // 圆环
};

struct Item {
    ZKBase *w;
    Kind kind;
    LayoutPosition origin;     // add 时刻的声明位置（载入=终点 / 退出=起点）
    int dx, dy;                // 方向单位向量
    int offset;                // 屏外位移覆盖值（>0 生效）
    int fromA, toA;            // 透明度端点
    int fromV, toV, digits;    // 数值端点 / 补零位数
    std::string suffix;        // 数值单位
    int dur, delay;
    EEaseType ease;
    int lastV;                 // 上次已应用数值（-1 = 尚未应用）
    int lastA;                 // 上次已应用 alpha
    bool exitItem;

    Item()
        : w(0), kind(K_SLIDE), dx(0), dy(0), offset(0), fromA(255), toA(255),
          fromV(0), toV(0), digits(0), dur(0), delay(0), ease(E_EASE_DEFAULT),
          lastV(-1), lastA(-1), exitItem(false) {
    }
};

std::vector<Item> sItems;
int sTimerId = -1;
int sTickMs = 16;
int sElapsedMs = 0;
bool sRunning = false;
bool sExit = false;
EEaseType sCurve = E_EASE_OUT_CUBIC;
bool sSlideAlpha = true;   // 滑入是否自带淡入（原版行为；false = 只位移）
int sTravel = 0;           // 全局滑入/滑出位移（0 = 屏外算；>0 = 固定像素，失败安全）
bool sArmed = false;       // 已 start() 但还没跑到第一帧（首帧才落到起点——防"定时器不跑 → 控件停在屏外"）
int sScreenW = 1024;
int sScreenH = 600;
std::function<void()> sExitCb;

inline int clampInt(int v, int lo, int hi) {
    return (v < lo) ? lo : ((v > hi) ? hi : v);
}

// 内部用的线性插值（对外同名工具见 PageAnim::lerpInt）
inline int lerpInt(int a, int b, float t) {
    return (int)(a + (b - a) * t);
}

inline float cosT(float x) {
    // cos([0,PI]) 泰勒展开（原版口径）
    float x2 = x * x;
    float x4 = x2 * x2;
    float x6 = x4 * x2;
    return 1.0f - x2 / 2.0f + x4 / 24.0f - x6 / 720.0f;
}

inline EEaseType resolveEase(EEaseType e) {
    return (e == E_EASE_DEFAULT) ? sCurve : e;
}

// 屏外位移距离：控件整块滑出可见区 + 50px 余量（或全局固定位移）
int offscreenOffset(const Item &it) {
    if (it.offset > 0) return it.offset;                                // 单项显式覆盖
    if (sTravel > 0) return sTravel;                                    // 全局固定位移（失败安全）
    if (it.dx < 0) return it.origin.mLeft + it.origin.mWidth + 50;      // 从/往 左
    if (it.dx > 0) return (sScreenW - it.origin.mLeft) + 50;            // 从/往 右
    if (it.dy < 0) return it.origin.mTop + it.origin.mHeight + 50;      // 从/往 上
    return (sScreenH - it.origin.mTop) + 50;                            // 从/往 下
}

void applyItem(Item &it, float p) {
    if (!it.w) return;

    switch (it.kind) {
    case K_SLIDE: {
        int off = offscreenOffset(it);
        // 载入：屏外(p=0) → 声明位(p=1)；退出：声明位(0) → 屏外(1)
        float k = it.exitItem ? p : (1.0f - p);
        LayoutPosition np = it.origin;
        np.mLeft = it.origin.mLeft + (int)(it.dx * off * k);
        np.mTop = it.origin.mTop + (int)(it.dy * off * k);
        if (np != it.w->getPosition()) {
            it.w->setPosition(np);
        }
        if (sSlideAlpha) {
            int a = clampInt((int)(it.fromA + (it.toA - it.fromA) * p), 0, 255);
            if (a != it.lastA) {
                it.lastA = a;
                it.w->setAlpha((uint8_t)a);
            }
        }
        break;
    }

    case K_FADE: {
        int a = clampInt((int)(it.fromA + (it.toA - it.fromA) * p), 0, 255);
        if (a != it.lastA) {
            it.lastA = a;
            it.w->setAlpha((uint8_t)a);
        }
        break;
    }

    case K_COUNTER: {
        int v = lerpInt(it.fromV, it.toV, p);
        if (v != it.lastV) {
            it.lastV = v;
            if (it.suffix.empty()) {
                static_cast<ZKTextView *>(it.w)->setText(v);
            } else {
                char buf[64];
                snprintf(buf, sizeof(buf), "%d%s", v, it.suffix.c_str());
                static_cast<ZKTextView *>(it.w)->setText(buf);
            }
        }
        break;
    }

    case K_CHARSET: {
        int v = lerpInt(it.fromV, it.toV, p);
        if (v != it.lastV) {
            it.lastV = v;
            char buf[64];
            snprintf(buf, sizeof(buf), "%0*d%s", clampInt(it.digits, 1, 12), v,
                     it.suffix.c_str());
            static_cast<ZKTextView *>(it.w)->setText(buf);
        }
        break;
    }

    case K_SEEK: {
        int v = lerpInt(it.fromV, it.toV, p);
        if (v != it.lastV) {
            it.lastV = v;
            static_cast<ZKSeekBar *>(it.w)->setProgress(v);
        }
        break;
    }

    case K_CIRCLE: {
        int v = lerpInt(it.fromV, it.toV, p);
        if (v != it.lastV) {
            it.lastV = v;
            static_cast<ZKCircleBar *>(it.w)->setProgress(v);
        }
        break;
    }

    default:
        break;
    }
}

// 复位到起点：载入=屏外/0；退出=声明位/满值
void resetItem(Item &it) {
    it.lastA = -1;
    it.lastV = -1;
    applyItem(it, 0.0f);
}

// 退场结束后把控件复位回声明位（防止"页面留在页面栈里 → 控件停在屏外 → 下次入场基准跑掉"）
void restoreItem(Item &it) {
    if (!it.w) return;
    if (it.kind == K_SLIDE) {
        LayoutPosition cur = it.w->getPosition();
        if (cur != it.origin) {
            it.w->setPosition(it.origin);
        }
    }
    if (it.kind == K_SLIDE || it.kind == K_FADE) {
        it.w->setAlpha(255);
    }
    it.lastA = -1;
    it.lastV = -1;
}

void addSlideItem(ZKBase *w, ESlideDirection dir, int durationMs, int delayMs,
                  EEaseType ease, int travelPx, bool out) {
    if (!w) return;

    Item it;
    it.w = w;
    it.kind = K_SLIDE;
    it.dur = clampInt(durationMs, 100, 4000);
    it.delay = delayMs < 0 ? 0 : delayMs;
    it.ease = resolveEase(ease);
    it.offset = travelPx;
    it.exitItem = out;
    it.origin = w->getPosition();
    it.fromA = out ? 255 : 0;
    it.toA = out ? 0 : 255;        // 滑入自带淡入 / 滑出自带淡出

    switch (dir) {
    case E_DIR_LEFT_2_RIGHT: it.dx = -1; it.dy = 0; break;
    case E_DIR_RIGHT_2_LEFT: it.dx = 1;  it.dy = 0; break;
    case E_DIR_TOP_2_BOTTOM: it.dx = 0;  it.dy = -1; break;
    case E_DIR_BOTTOM_2_TOP: it.dx = 0;  it.dy = 1; break;
    case E_DIR_FADE_ONLY:
    default:                 it.dx = 0;  it.dy = 0; break;
    }

    if (it.dx == 0 && it.dy == 0) {
        // 只淡入/淡出，不改位置
        it.kind = K_FADE;
    }

    sItems.push_back(it);
}

void addValueItem(ZKBase *w, Kind kind, int target, int digits, int durationMs,
                  int delayMs, EEaseType ease, const char *suffix) {
    if (!w) return;

    Item it;
    it.w = w;
    it.kind = kind;
    it.dur = clampInt(durationMs, 100, 4000);
    it.delay = delayMs < 0 ? 0 : delayMs;
    it.ease = resolveEase(ease);
    it.fromV = 0;
    it.toV = target < 0 ? 0 : target;
    it.digits = digits;
    if (suffix) it.suffix = suffix;
    it.exitItem = false;

    // 进度类：目标超过控件 max 时夹取（原版口径）
    if (kind == K_SEEK) {
        int mx = static_cast<ZKSeekBar *>(w)->getMax();
        if (mx > 0 && it.toV > mx) it.toV = mx;
    } else if (kind == K_CIRCLE) {
        int mx = static_cast<ZKCircleBar *>(w)->getMax();
        if (mx > 0 && it.toV > mx) it.toV = mx;
    }

    sItems.push_back(it);
}

void beginGroup(bool exitGroup) {
    sElapsedMs = 0;
    sExit = exitGroup;

    int count = 0;
    for (size_t i = 0; i < sItems.size(); ++i) {
        Item &it = sItems[i];
        if (it.exitItem != exitGroup) continue;
        ++count;
    }

    sRunning = (count > 0);
    /* ⚠️ 关键：这里**不**把控件落到起点 —— 留给第一帧 onTimer 去做。
     * 理由：如果定时器没跑（未注册/被看护重启/页面没显示），控件就停在声明位（可见），
     * 而不是停在屏外 → 不会出现"黑屏/控件不见"。代价是第一帧前有一个 16ms 的原位帧，人眼无感。 */
    sArmed = sRunning;
    if (!sRunning && exitGroup && sExitCb) {
        sExitCb();
    }
}

} // namespace

// ========================= 工具（原版公开面） =========================

int PageAnim::lerpInt(int a, int b, float t) {
    return (int)(a + (b - a) * t);
}

float PageAnim::ease(EEaseType type, float t) {
    if (t < 0.0f) t = 0.0f;
    if (t > 1.0f) t = 1.0f;
    switch (type) {
    case E_EASE_OUT_CUBIC: {
        float inv = 1.0f - t;
        return 1.0f - inv * inv * inv;
    }
    case E_EASE_OUT_QUART: {
        float inv = 1.0f - t;
        return 1.0f - inv * inv * inv * inv;
    }
    case E_EASE_IN_OUT_SINE: {
        const float PI = 3.14159265358979f;
        return -(cosT(PI * t) - 1.0f) * 0.5f;
    }
    case E_EASE_LINEAR:
    default:
        return t;
    }
}

// ============================ 生命周期 ============================

void PageAnim::init(int timerId, int tickMs) {
    sItems.clear();
    sTimerId = timerId;
    sTickMs = clampInt(tickMs, 8, 50);
    sElapsedMs = 0;
    sRunning = false;
    sExit = false;
    sExitCb = NULL;
}

void PageAnim::release() {
    sItems.clear();
    sRunning = false;
    sExit = false;
    sExitCb = NULL;
    sTimerId = -1;
    sElapsedMs = 0;
}

void PageAnim::start() {
    beginGroup(false);
}

void PageAnim::startExit() {
    beginGroup(true);
}

void PageAnim::stop() {
    sRunning = false;
    sArmed = false;
}

bool PageAnim::isRunning() {
    return sRunning;
}

void PageAnim::onTimer(int id) {
    if (sTimerId >= 0 && id != sTimerId) return;
    if (!sRunning) return;

    if (sArmed) {
        sArmed = false;
        for (size_t i = 0; i < sItems.size(); ++i) {
            Item &it = sItems[i];
            if (it.exitItem != sExit) continue;
            resetItem(it);      /* 首帧才落到起点（见 beginGroup 注释） */
        }
    }

    sElapsedMs += sTickMs;

    int maxEnd = 0;
    for (size_t i = 0; i < sItems.size(); ++i) {
        Item &it = sItems[i];
        if (it.exitItem != sExit) continue;

        int end = it.delay + it.dur;
        if (end > maxEnd) maxEnd = end;

        if (sElapsedMs < it.delay) continue;

        int localT = sElapsedMs - it.delay;
        float p01 = (localT >= it.dur) ? 1.0f : (float)localT / (float)it.dur;
        applyItem(it, ease(it.ease, p01));
    }

    if (sElapsedMs >= maxEnd) {
        for (size_t i = 0; i < sItems.size(); ++i) {
            Item &it = sItems[i];
            if (it.exitItem != sExit) continue;
            applyItem(it, 1.0f);       // 最后一帧落实到位
        }
        sRunning = false;

        if (sExit) {
            /* ⚠️ 退场播完后必须把控件复位回原位：
             *    FlyThings 的 openActivity 会把当前页留在页面栈里，未必走 onUI_quit；
             *    若控件停在屏外，下次 onUI_show 再登记/重播入场时，"当前位置"就成了屏外，
             *    位移基准跑掉 → 切回该页时控件看不见。
             *    （本次帧已经在屏幕上，复位不会被人眼看到。） */
            for (size_t i = 0; i < sItems.size(); ++i) {
                Item &it = sItems[i];
                if (!it.exitItem) continue;
                restoreItem(it);
            }
            if (sExitCb) {
                sExitCb();
            }
        }
    }
}

// ======================= 注册动画（原版面） =======================

void PageAnim::addSlide(ZKBase *pCtrl, int durationMs, int delayMs,
                        ESlideDirection dir, EEaseType ease, int travelPx) {
    addSlideItem(pCtrl, dir, durationMs, delayMs, ease, travelPx, false);
}

void PageAnim::addSlideLeft2Right(ZKBase *pCtrl, int durationMs, int delayMs,
                                  EEaseType ease, int travelPx) {
    addSlideItem(pCtrl, E_DIR_LEFT_2_RIGHT, durationMs, delayMs, ease, travelPx, false);
}

void PageAnim::addSlideRight2Left(ZKBase *pCtrl, int durationMs, int delayMs,
                                  EEaseType ease, int travelPx) {
    addSlideItem(pCtrl, E_DIR_RIGHT_2_LEFT, durationMs, delayMs, ease, travelPx, false);
}

void PageAnim::addSlideTop2Bottom(ZKBase *pCtrl, int durationMs, int delayMs,
                                  EEaseType ease, int travelPx) {
    addSlideItem(pCtrl, E_DIR_TOP_2_BOTTOM, durationMs, delayMs, ease, travelPx, false);
}

void PageAnim::addSlideBottom2Top(ZKBase *pCtrl, int durationMs, int delayMs,
                                  EEaseType ease, int travelPx) {
    addSlideItem(pCtrl, E_DIR_BOTTOM_2_TOP, durationMs, delayMs, ease, travelPx, false);
}

void PageAnim::addFadeIn(ZKBase *pCtrl, int durationMs, int delayMs, EEaseType ease) {
    addSlideItem(pCtrl, E_DIR_FADE_ONLY, durationMs, delayMs, ease, -1, false);
}

void PageAnim::addTextCounter(ZKTextView *pTv, int targetVal, int durationMs, int delayMs,
                              EEaseType ease, const char *suffix) {
    addValueItem(pTv, K_COUNTER, targetVal, 0, durationMs, delayMs, ease, suffix);
}

void PageAnim::addCharsetCounter(ZKTextView *pTv, int targetVal, int digitCount, int durationMs,
                                 int delayMs, EEaseType ease, const char *suffix) {
    addValueItem(pTv, K_CHARSET, targetVal, digitCount, durationMs, delayMs, ease, suffix);
}

void PageAnim::addSeekBarProgress(ZKSeekBar *pSb, int targetProgress, int durationMs,
                                  int delayMs, EEaseType ease) {
    addValueItem(pSb, K_SEEK, targetProgress, 0, durationMs, delayMs, ease, NULL);
}

void PageAnim::addCircleBarProgress(ZKCircleBar *pCb, int targetProgress, int durationMs,
                                    int delayMs, EEaseType ease) {
    addValueItem(pCb, K_CIRCLE, targetProgress, 0, durationMs, delayMs, ease, NULL);
}

// ========================= 退出（退场，新增） =========================

void PageAnim::addSlideOutTop(ZKBase *pCtrl, int durationMs, int delayMs,
                              EEaseType ease, int travelPx) {
    addSlideItem(pCtrl, E_DIR_TOP_2_BOTTOM, durationMs, delayMs, ease, travelPx, true);
}

void PageAnim::addSlideOutBottom(ZKBase *pCtrl, int durationMs, int delayMs,
                                 EEaseType ease, int travelPx) {
    addSlideItem(pCtrl, E_DIR_BOTTOM_2_TOP, durationMs, delayMs, ease, travelPx, true);
}

void PageAnim::addSlideOutLeft(ZKBase *pCtrl, int durationMs, int delayMs,
                               EEaseType ease, int travelPx) {
    addSlideItem(pCtrl, E_DIR_LEFT_2_RIGHT, durationMs, delayMs, ease, travelPx, true);
}

void PageAnim::addSlideOutRight(ZKBase *pCtrl, int durationMs, int delayMs,
                                EEaseType ease, int travelPx) {
    addSlideItem(pCtrl, E_DIR_RIGHT_2_LEFT, durationMs, delayMs, ease, travelPx, true);
}

void PageAnim::addFadeOut(ZKBase *pCtrl, int durationMs, int delayMs, EEaseType ease) {
    addSlideItem(pCtrl, E_DIR_FADE_ONLY, durationMs, delayMs, ease, -1, true);
}

void PageAnim::setExitCallback(std::function<void()> cb) {
    sExitCb = cb;
}

// ============================ 调参 ============================

void PageAnim::setEasingCurve(EEaseType curve) {
    sCurve = curve;
}

void PageAnim::setEasing(bool easeOut) {
    sCurve = easeOut ? E_EASE_OUT_CUBIC : E_EASE_LINEAR;
}

void PageAnim::setSlideAlphaEnable(bool enable) {
    sSlideAlpha = enable;
}

void PageAnim::setSlideTravel(int px) {
    sTravel = (px > 0) ? px : 0;
}

void PageAnim::setScreenSize(int width, int height) {
    if (width > 0) sScreenW = width;
    if (height > 0) sScreenH = height;
}

} // namespace zk
