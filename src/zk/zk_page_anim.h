/*
 * zk_page_anim.h —— 页面「载入 / 退出」动画模块（FlyThings / EasyUI）
 *
 * 来历：本模块 = 早期 `AnimationFramework`（Z21 全控件演示的载入动效）的**封装升级版**：
 *       · **原版 API 100% 源码兼容**（类名 AnimationFramework、枚举 EEaseType/ESlideDirection/
 *         EAnimType、方法名与默认参数全部保留，旧代码不改一行就能编）；
 *       · 原版行为保留：屏外滑入(+50px 余量) + 滑入自带淡入 + 4 种缓动 + 按时长夹取 +
 *         数值变化才刷；
 *       · 叠加：**退出（退场）动画**、计数单位 suffix、屏幕尺寸可配、滑出距离可覆盖。
 *       原版留档：../reference/AnimationFramework.original.{h,cpp}
 *
 * 零依赖：只用 EasyUI 控件 API（setPosition / setAlpha / setText / setProgress）。
 *   ⚠️ **不调用** ZKBase::invalidate()：部分设备 libeasyui 未导出该符号，
 *      会在加载期 undefined symbol（详见 ../platforms.md）。
 *
 * 最小用法（页面 logic）：
 *   #define PAGE_ANIM_TIMER_ID 9999
 *   static S_ACTIVITY_TIMEER REGISTER_ACTIVITY_TIMER_TAB[] = { { PAGE_ANIM_TIMER_ID, 16 } };
 *
 *   static void onUI_init() {
 *       zk::PageAnim::init(PAGE_ANIM_TIMER_ID, 16);
 *       zk::PageAnim::addSlideTop2Bottom(mTvTitlePtr, 800, 0);
 *       zk::PageAnim::addFadeIn(mBtnBackPtr, 700, 100);
 *       zk::PageAnim::addTextCounter(mTvNumPtr, 9527, 1500, 700);
 *       zk::PageAnim::start();
 *   }
 *   static void onUI_show()  { zk::PageAnim::start(); }      // 再次进入自动重播
 *   static void onUI_quit()  { zk::PageAnim::release(); }
 *   static bool onUI_Timer(int id) { zk::PageAnim::onTimer(id); return true; }
 *
 * 退出动画（先播退场，播完再返回）：
 *   // zk::PageAnim::setExitCallback([](){ EASYUICONTEXT->goBack(); });
 *   // 返回键里： stop() → addFadeOut(...) / addSlideOutBottom(...) → startExit();
 *
 * 平台与实测：../platforms.md   例程：../example/
 */
#ifndef _ZK_PAGE_ANIM_H_
#define _ZK_PAGE_ANIM_H_

#include "control/ZKBase.h"
#include "control/ZKTextView.h"
#include "control/ZKSeekBar.h"
#include "control/ZKCircleBar.h"

#include <functional>

/* ============================================================
 * 原版兼容面（与 AnimationFramework.h 同名同值，别改）
 * ============================================================ */

/// 缓动曲线（E_EASE_DEFAULT = 用全局默认，见 setEasingCurve）
typedef enum {
    E_EASE_DEFAULT = -1,
    E_EASE_LINEAR = 0,
    E_EASE_OUT_CUBIC,      /* 先快后慢，滑入默认 */
    E_EASE_OUT_QUART,
    E_EASE_IN_OUT_SINE
} EEaseType;

/// 滑入方向
typedef enum {
    E_DIR_LEFT_2_RIGHT = 0,
    E_DIR_RIGHT_2_LEFT,
    E_DIR_TOP_2_BOTTOM,
    E_DIR_BOTTOM_2_TOP,
    E_DIR_FADE_ONLY        /* 仅淡入，不移动 */
} ESlideDirection;

/// 动画类型（内部用；保留供旧代码引用）
typedef enum {
    E_ANIM_SLIDE = 0,
    E_ANIM_TEXT_COUNTER,
    E_ANIM_TEXT_CHARSET_COUNTER,
    E_ANIM_SEEKBAR_PROGRESS,
    E_ANIM_CIRCLEBAR_PROGRESS
} EAnimType;

namespace zk {

class PageAnim {
public:
    // ============================ 生命周期 ============================

    /// 绑定页面周期定时器（页面注册 {timerId, tickMs}）。tickMs 建议 16，夹取 8~50；
    /// **会清空上次登记**。
    static void init(int timerId, int tickMs = 16);

    /// 清空所有已登记动画并停止（页面 onUI_quit 调用）。
    static void release();

    /// 开始播放已登记的「载入」动画（先把控件复位到起始状态，再逐帧推进）。
    static void start();

    /// 开始播放已登记的「退出」动画；播完调用 setExitCallback 设的回调。
    static void startExit();

    /// 停止推进（保留已登记项，便于再次 start()）。
    static void stop();

    /// 页面定时器转发：在 onUI_Timer(int id) 里调用。
    static void onTimer(int id);

    /// 是否正在播放动画。
    static bool isRunning();

    // ======================= 注册动画（原版面） =======================

    /// 通用滑入（含 Alpha 淡入）。travelPx > 0 时用指定位移，否则按"屏外 +50px"算。
    static void addSlide(ZKBase *pCtrl, int durationMs, int delayMs,
                         ESlideDirection dir, EEaseType ease = E_EASE_DEFAULT,
                         int travelPx = -1);

    /// 从左往右滑入
    static void addSlideLeft2Right(ZKBase *pCtrl, int durationMs, int delayMs = 0,
                                   EEaseType ease = E_EASE_DEFAULT, int travelPx = -1);
    /// 从右往左滑入
    static void addSlideRight2Left(ZKBase *pCtrl, int durationMs, int delayMs = 0,
                                   EEaseType ease = E_EASE_DEFAULT, int travelPx = -1);
    /// 从上往下滑入
    static void addSlideTop2Bottom(ZKBase *pCtrl, int durationMs, int delayMs = 0,
                                   EEaseType ease = E_EASE_DEFAULT, int travelPx = -1);
    /// 从下往上滑入
    static void addSlideBottom2Top(ZKBase *pCtrl, int durationMs, int delayMs = 0,
                                   EEaseType ease = E_EASE_DEFAULT, int travelPx = -1);

    /// 原地淡入（alpha 0 → 255，不移动）
    static void addFadeIn(ZKBase *pCtrl, int durationMs, int delayMs = 0,
                          EEaseType ease = E_EASE_DEFAULT);

    /// 数字累加：0 → targetVal（走 setText(int)）。suffix 追加单位（如 "%"），NULL 不带。
    static void addTextCounter(ZKTextView *pTv, int targetVal, int durationMs, int delayMs = 0,
                               EEaseType ease = E_EASE_DEFAULT, const char *suffix = NULL);

    /// 定宽补零数字累加：0 → targetVal，digitCount 位（如 5 → "86420"）；suffix 追加单位。
    static void addCharsetCounter(ZKTextView *pTv, int targetVal, int digitCount, int durationMs,
                                  int delayMs = 0, EEaseType ease = E_EASE_DEFAULT,
                                  const char *suffix = NULL);

    /// 进度条推进：0 → targetProgress（超过 max 自动夹取）
    static void addSeekBarProgress(ZKSeekBar *pSb, int targetProgress, int durationMs,
                                   int delayMs = 0, EEaseType ease = E_EASE_DEFAULT);

    /// 圆环进度推进：0 → targetProgress（超过 max 自动夹取）
    static void addCircleBarProgress(ZKCircleBar *pCb, int targetProgress, int durationMs,
                                     int delayMs = 0, EEaseType ease = E_EASE_DEFAULT);

    // ========================= 退出（退场，新增） =========================

    /// 向上滑出屏外（含淡出）
    static void addSlideOutTop(ZKBase *pCtrl, int durationMs, int delayMs = 0,
                               EEaseType ease = E_EASE_DEFAULT, int travelPx = -1);
    /// 向下滑出
    static void addSlideOutBottom(ZKBase *pCtrl, int durationMs, int delayMs = 0,
                                  EEaseType ease = E_EASE_DEFAULT, int travelPx = -1);
    /// 向左滑出
    static void addSlideOutLeft(ZKBase *pCtrl, int durationMs, int delayMs = 0,
                                EEaseType ease = E_EASE_DEFAULT, int travelPx = -1);
    /// 向右滑出
    static void addSlideOutRight(ZKBase *pCtrl, int durationMs, int delayMs = 0,
                                 EEaseType ease = E_EASE_DEFAULT, int travelPx = -1);

    /// 原地淡出（alpha 255 → 0）
    static void addFadeOut(ZKBase *pCtrl, int durationMs, int delayMs = 0,
                           EEaseType ease = E_EASE_DEFAULT);

    /// 退场全部播完的回调（一般在这里 goBack()/openActivity()）。传 nullptr 取消。
    static void setExitCallback(std::function<void()> cb);

    // ============================ 调参 ============================

    /// 全局默认缓动曲线（对应 E_EASE_DEFAULT 的那些动画）
    static void setEasingCurve(EEaseType curve);
    /// 兼容写法：true = 缓出，false = 匀速
    static void setEasing(bool easeOut);
    /// 滑入/滑出是否附带 Alpha 淡入淡出（原版行为，默认 true）。
    /// ⚠️ 若某些设备 `setAlpha` 后不自动重绘（控件会"看不到"、成对零件出现时序错位），
    /// 置 false 就只做位移（`setPosition` 一定重绘）。
    static void setSlideAlphaEnable(bool enable);

    /// 全局滑入/滑出位移（像素）。默认 0 = 按"屏幕外 +50px"算（原版行为）。
    /// ⚠️ **失败安全**：给成小位移（如 48）后，即使动画被中断/定时器没跑，
    /// 控件最多偏离原位几十像素，不会跑出屏幕（避免"黑屏/控件不见"）。
    static void setSlideTravel(int px);

    /// 屏幕尺寸（算滑入/滑出距离用），默认 1024x600
    static void setScreenSize(int width, int height);

    // ========================= 工具（原版公开面） =========================
    static int   lerpInt(int a, int b, float t);
    static float ease(EEaseType type, float t);
};

} // namespace zk

/// 原版类名别名：AnimationFramework::addSlideLeft2Right(...) ≡ zk::PageAnim::...
typedef zk::PageAnim AnimationFramework;

#endif /* _ZK_PAGE_ANIM_H_ */
