#include <base/base.h>
#include <vector>
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

static void imgSelColor(int idx);   /* 画笔色卡互斥选中（定义见文件末） */
static void imgSelWidth(int idx);   /* 画笔粗细互斥选中（定义见文件末） */

/* ============================================================
 * 自定义画板（ZKPainter 手绘）
 * ------------------------------------------------------------
 * 2026-10-10 修复（钟工）：
 *   ② 手指绘制区域无效 —— 原 onimageActivityTouchEvent 只做翻页、没接画布；
 *      现接入全局触摸回调：命中 Canvas(painter__1) 区域时把触点折算成控件内坐标
 *      (画布左上 (44,112))→ 逐点落笔，抬起收笔。
 *   ③ 粗中细选择没有效果 —— 原 W1/W2/W3 回调空实现，粗细丢了 preview 又被按钮盖住；
 *      现 W 按钮走两态（选中红底/未选暗底，见 _fix_canvas_1010.py），
 *      选中即改笔宽并重绘。
 * 另补 BtnUndo/BtnClear（真清空 + 撤销一笔）。
 *
 * 口径：
 *   · 触摸回调 `ev.mX/mY` 是**屏幕绝对坐标**，画布在 (44,112) → 需减去控件原点；
 *   · 全局触摸回调只在「命中画布」时返回 true 拦截，其余交回 zkNavTouch（否则按钮/翻页全废）；
 *   · 撤销 = 维护笔画列表 app 侧重放（ZKPainter 无「擦一笔」API，只有整块 erase）。
 * ============================================================ */
static const int CANVAS_X = 44;    /* painter__1 位置（ui/image.json） */
static const int CANVAS_Y = 112;
static const int CANVAS_W = 668;
static const int CANVAS_H = 304;

static const int PEN_W[3] = { 2, 5, 9 };                       /* 细 / 中 / 粗 */
static const unsigned int PEN_COLOR[5] = { 0xEC4141, 0x2ECC71, 0xF5A623,
                                           0x4C8DFF, 0xFFFFFF }; /* 与色卡 Sw1..5 一致 */

struct PenStroke {
    unsigned int color;
    int width;
    std::vector<SZKPoint> pts;
};

static std::vector<PenStroke> sStrokes;   /* 笔画列表（撤销/重放用） */
static bool sDrawing = false;             /* 正在落笔 */
static int sPenWIdx = 0;                  /* 当前粗细下标 */
static int sPenColorIdx = 0;              /* 当前颜色下标 */
static int sPenW = PEN_W[0];
static unsigned int sPenColor = PEN_COLOR[0];
static bool sHintVisible = true;          /* 提示语是否露出（画即隐藏） */

static void penDot(int x, int y, unsigned int color, int w) {
    if (mCanvasPtr == NULL) return;
    if (w < 1) w = 1;
    int r = (w + 1) / 2;
    mCanvasPtr->setLineWidth(w);
    mCanvasPtr->setSourceColor(color);
    mCanvasPtr->fillArc(x, y, r, r, 0, 360);        /* 圆头笔点 */
}

static void penSegment(const SZKPoint &a, const SZKPoint &b, unsigned int color, int w) {
    if (mCanvasPtr == NULL) return;
    SZKPoint seg[2];
    seg[0] = a;
    seg[1] = b;
    mCanvasPtr->setLineWidth(w);
    mCanvasPtr->setSourceColor(color);
    mCanvasPtr->drawLines(seg, 2);
}

/* 擦干净画布后按笔画列表整段重放（撤销用） */
static void penRedrawAll() {
    if (mCanvasPtr == NULL) return;
    mCanvasPtr->erase(0, 0, CANVAS_W, CANVAS_H);
    for (size_t i = 0; i < sStrokes.size(); ++i) {
        const PenStroke &st = sStrokes[i];
        for (size_t j = 0; j + 1 < st.pts.size(); ++j) {
            penSegment(st.pts[j], st.pts[j + 1], st.color, st.width);
        }
        for (size_t j = 0; j < st.pts.size(); ++j) {
            penDot((int) st.pts[j].x, (int) st.pts[j].y, st.color, st.width);
        }
    }
}

static void penHintShow(bool show) {
    if (mCanvasHintPtr == NULL) return;
    mCanvasHintPtr->setVisible(show);
    sHintVisible = show;
}

static void zkAnimInit() {
    zk::PageAnim::init(9, 16);        /* 与 REGISTER_ACTIVITY_TIMER_TAB 的 {9,16} 对应 */
    zk::PageAnim::setSlideAlphaEnable(false);   /* 本机 setAlpha 不重绘：只做位移 */
    zk::PageAnim::addSlideTop2Bottom(mTitleTextPtr, 420, 0);
    zk::PageAnim::addSlideTop2Bottom(mTitleSubPtr, 420, 60);
    zk::PageAnim::addSlideBottom2Top(mNavBarPtr, 460, 40);
    zk::PageAnim::addSlideLeft2Right(mCanvasCardPtr, 460, 140);
    zk::PageAnim::addSlideRight2Left(mToolCardPtr, 460, 190);
    zk::PageAnim::start();
}

static void onUI_init() {
    zkAnimInit();
  #ifdef FUN_BUILD
  INIT_UI_TIMERS
  #endif // FUN_BUILD
    /* 画笔默认：Sw1 红 + 细（与设计稿一致） */
    imgSelColor(0);
    imgSelWidth(0);
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

/* 触摸：命中画布 → 落笔；否则交回翻页（标题区）*/
static bool onimageActivityTouchEvent(const MotionEvent &ev) {
    int cx = ev.mX - CANVAS_X;
    int cy = ev.mY - CANVAS_Y;
    switch (ev.mActionStatus) {
		case MotionEvent::E_ACTION_DOWN:
			if (mCanvasPtr != NULL && cx >= 0 && cx < CANVAS_W
			        && cy >= 0 && cy < CANVAS_H) {
				sDrawing = true;
				if (sHintVisible) penHintShow(false);
				PenStroke st;
				st.color = sPenColor;
				st.width = sPenW;
				SZKPoint p;
				p.x = (float) cx;
				p.y = (float) cy;
				st.pts.push_back(p);
				sStrokes.push_back(st);
				penDot(cx, cy, sPenColor, sPenW);
				return true;                 /* 拦截：画布内按下不传给其它控件 */
			}
			break;
		case MotionEvent::E_ACTION_MOVE:
			if (sDrawing && !sStrokes.empty()) {
				/* 出界则贴边（画布外部分不画） */
				int x = cx < 0 ? 0 : (cx >= CANVAS_W ? CANVAS_W - 1 : cx);
				int y = cy < 0 ? 0 : (cy >= CANVAS_H ? CANVAS_H - 1 : cy);
				PenStroke &st = sStrokes.back();
				SZKPoint last = st.pts.back();
				if ((int) last.x != x || (int) last.y != y) {
					SZKPoint p;
					p.x = (float) x;
					p.y = (float) y;
					penSegment(last, p, st.color, st.width);
					penDot(x, y, st.color, st.width);
					st.pts.push_back(p);
				}
				return true;
			}
			break;
		case MotionEvent::E_ACTION_UP:
			if (sDrawing) {
				sDrawing = false;
				return true;
			}
			break;
		default:
			break;
	}
	return zkNavTouch(5, ev);
}

/* 画笔颜色色卡：两态按钮互斥选中 */
static int sSwCol = 0;
static void imgSelColor(int idx) {
    if (idx < 0 || idx > 4) return;
    sSwCol = idx;
    sPenColorIdx = idx;
    sPenColor = PEN_COLOR[idx];
    ZKButton* bs[5] = {mSw1Ptr, mSw2Ptr, mSw3Ptr, mSw4Ptr, mSw5Ptr};
    for (int i = 0; i < 5; i++) if (bs[i] != NULL) bs[i]->setSelected(i == idx);
}
static bool onButtonClick_Sw1(ZKButton* pButton) { imgSelColor(0); return false; }
static bool onButtonClick_Sw2(ZKButton* pButton) { imgSelColor(1); return false; }
static bool onButtonClick_Sw3(ZKButton* pButton) { imgSelColor(2); return false; }
static bool onButtonClick_Sw4(ZKButton* pButton) { imgSelColor(3); return false; }
static bool onButtonClick_Sw5(ZKButton* pButton) { imgSelColor(4); return false; }

/* 画笔粗细：两态按钮互斥选中 */
static void imgSelWidth(int idx) {
    if (idx < 0 || idx > 2) return;
    sPenWIdx = idx;
    sPenW = PEN_W[idx];
    ZKButton* bs[3] = {mW1Ptr, mW2Ptr, mW3Ptr};
    for (int i = 0; i < 3; i++) if (bs[i] != NULL) bs[i]->setSelected(i == idx);
}

static bool onButtonClick_W1(ZKButton* pButton) {
	imgSelWidth(0);
	return false;
}

static bool onButtonClick_W2(ZKButton* pButton) {
	imgSelWidth(1);
	return false;
}

static bool onButtonClick_W3(ZKButton* pButton) {
	imgSelWidth(2);
	return false;
}

static bool onButtonClick_BtnUndo(ZKButton* pButton) {
	if (!sStrokes.empty()) {
		sStrokes.pop_back();
		penRedrawAll();
		if (sStrokes.empty()) penHintShow(true);
	}
	return false;
}

static bool onButtonClick_BtnClear(ZKButton* pButton) {
	sStrokes.clear();
	if (mCanvasPtr != NULL) mCanvasPtr->erase(0, 0, CANVAS_W, CANVAS_H);
	penHintShow(true);
	return false;
}

static bool onButtonClick_BtnSave(ZKButton* pButton) {
	return false;
}static bool onButtonClick_NavBtn1(ZKButton* pButton) { return zkNavGo(0); }

static bool onButtonClick_NavBtn2(ZKButton* pButton) { return zkNavGo(1); }

static bool onButtonClick_NavBtn3(ZKButton* pButton) { return zkNavGo(2); }

static bool onButtonClick_NavBtn4(ZKButton* pButton) { return zkNavGo(3); }

static bool onButtonClick_NavBtn5(ZKButton* pButton) { return zkNavGo(4); }

static bool onButtonClick_NavBtn6(ZKButton* pButton) { return zkNavGo(5); }

static bool onButtonClick_NavBtn7(ZKButton* pButton) { return zkNavGo(6); }

static bool onButtonClick_NavBtn8(ZKButton* pButton) { return zkNavGo(7); }
