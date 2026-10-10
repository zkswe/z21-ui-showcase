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
    {0, 1000}, /* 走时/日期（真实系统时间） */
    {1, 16},   /* 滚轮中心行回读 */
};

/* 走时：数字钟由 digitalclock 控件自动走；日期文本与指针角度每秒刷一次 */
static const char *kWk[] = {"日", "一", "二", "三", "四", "五", "六"};
static void dtApply() {
    time_t t = time(NULL);
    struct tm *lt = localtime(&t);
    char b[48];
    snprintf(b, sizeof(b), "%04d-%02d-%02d 星期%s",
             lt->tm_year + 1900, lt->tm_mon + 1, lt->tm_mday, kWk[lt->tm_wday]);
    mClockDatePtr->setText(b);
    mDialMinPtr->setTargetAngle(lt->tm_min * 6.0);
    mDialHourPtr->setTargetAngle((lt->tm_hour % 12) * 30.0 + lt->tm_min * 0.5);
}

/* ---- 时间选择器：3 列 listview 滚轮（口径 knowledge/uicontrols/listview-wheel-picker） ---- */
static const int PK_MAX[3] = {24, 60, 60};
static const int PK_ROT = 1;                  /* rows=3 -> 正中行 = fi + 1 */
static int sPkShift[3] = {9, 41, 0};          /* 初值（数据平移，令其落正中行） */
static int sPkSel[3] = {9, 41, 0};

static ZKListView* pkLv(int c) {
    return c == 0 ? mPkHourPtr : (c == 1 ? mPkMinPtr : mPkSecPtr);
}
static int pkCenter(int c) {
    ZKListView* lv = pkLv(c);
    if (!lv) return PK_ROT;
    int fi = lv->getFirstVisibleItemIndex();
    int off = lv->getFirstVisibleItemOffset();
    int ih = (int)lv->getItemHeight();
    int h = lv->getPosition().mHeight;
    if (ih <= 0) return fi + PK_ROT;
    return fi + (h / 2 - off) / ih;
}
static int pkValAt(int c, int idx) {
    int n = PK_MAX[c];
    return ((idx - PK_ROT + sPkShift[c]) % n + n) % n;
}
static void pkObtain(int c, ZKListView::ZKListItem *pListItem, int index) {
    int n = PK_MAX[c];
    int v = ((index - PK_ROT + sPkShift[c]) % n + n) % n;
    char b[16];
    snprintf(b, sizeof(b), "%02d", v);
    pListItem->setText(b);
    pListItem->setTextStatusColor(0, index == pkCenter(c) ? 0xEC4141 : 0x8A8F98);
}
static int getListItemCount_PkHour(const ZKListView *pListView) { (void)pListView; return PK_MAX[0] * 2; }
static int getListItemCount_PkMin(const ZKListView *pListView)  { (void)pListView; return PK_MAX[1] * 2; }
static int getListItemCount_PkSec(const ZKListView *pListView)  { (void)pListView; return PK_MAX[2] * 2; }
static void obtainListItemData_PkHour(ZKListView *pListView, ZKListView::ZKListItem *pListItem, int index) { (void)pListView; pkObtain(0, pListItem, index); }
static void obtainListItemData_PkMin(ZKListView *pListView, ZKListView::ZKListItem *pListItem, int index)  { (void)pListView; pkObtain(1, pListItem, index); }
static void obtainListItemData_PkSec(ZKListView *pListView, ZKListView::ZKListItem *pListItem, int index)  { (void)pListView; pkObtain(2, pListItem, index); }
static void pkClick(int c, ZKListView *pListView, int index) { sPkShift[c] = pkValAt(c, index); pListView->refreshListView(); }
static void onListItemClick_PkHour(ZKListView *pListView, int index, int id) { (void)id; pkClick(0, pListView, index); }
static void onListItemClick_PkMin(ZKListView *pListView, int index, int id)  { (void)id; pkClick(1, pListView, index); }
static void onListItemClick_PkSec(ZKListView *pListView, int index, int id)  { (void)id; pkClick(2, pListView, index); }
static void pkPoll() {
    for (int c = 0; c < 3; c++) {
        ZKListView* lv = pkLv(c);
        if (!lv) continue;
        int v = pkValAt(c, pkCenter(c));
        if (v != sPkSel[c]) { sPkSel[c] = v; lv->refreshListView(); }
    }
}

/* ---- 日历：按年月重排日格（可翻月 + 点选日期） ---- */
static int sCalY = 2026, sCalM = 10, sCalSel = 9;
static ZKTextView* calCell[31];
static void calInitCells() {
    ZKTextView* t[31] = {
        mCalD1Ptr, mCalD2Ptr, mCalD3Ptr, mCalD4Ptr, mCalD5Ptr, mCalD6Ptr, mCalD7Ptr, mCalD8Ptr,
        mCalD9Ptr, mCalD10Ptr, mCalD11Ptr, mCalD12Ptr, mCalD13Ptr, mCalD14Ptr, mCalD15Ptr, mCalD16Ptr,
        mCalD17Ptr, mCalD18Ptr, mCalD19Ptr, mCalD20Ptr, mCalD21Ptr, mCalD22Ptr, mCalD23Ptr, mCalD24Ptr,
        mCalD25Ptr, mCalD26Ptr, mCalD27Ptr, mCalD28Ptr, mCalD29Ptr, mCalD30Ptr, mCalD31Ptr
    };
    for (int i = 0; i < 31; i++) calCell[i] = t[i];
}
static int calDaysIn(int y, int m) {
    static const int dm[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (m == 2 && ((y % 4 == 0 && y % 100 != 0) || y % 400 == 0)) return 29;
    return dm[m - 1];
}
static int calFirstWd(int y, int m) {          /* 一=0 .. 日=6 */
    static const int t[] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};
    int yy = y - (m < 3);
    int w = (yy + yy / 4 - yy / 100 + yy / 400 + t[m - 1] + 1) % 7;   /* 0=Sun */
    return (w + 6) % 7;
}
static void calRender() {
    char b[32];
    snprintf(b, sizeof(b), "%d 年 %d 月", sCalY, sCalM);
    mCalTitlePtr->setText(b);
    int first = calFirstWd(sCalY, sCalM);
    int days = calDaysIn(sCalY, sCalM);
    for (int i = 0; i < 31; i++) {
        int d = i + 1;
        if (d > days) { calCell[i]->setVisible(false); continue; }
        int idx = first + (d - 1);
        calCell[i]->setPosition(LayoutPosition(24 + (idx % 7) * 60, 94 + (idx / 7) * 48, 56, 40));
        snprintf(b, sizeof(b), "%d", d);
        calCell[i]->setText(b);
        calCell[i]->setVisible(true);
        calCell[i]->setBackgroundPic(d == sCalSel ? "images/pill_CalD9.9.png" : "images/cal_none.png");
    }
}
static void calShiftMonth(int delta) {
    int m = sCalM + delta;
    while (m < 1) { m += 12; sCalY--; }
    while (m > 12) { m -= 12; sCalY++; }
    sCalM = m;
    int days = calDaysIn(sCalY, sCalM);
    if (sCalSel > days) sCalSel = days;
    calRender();
}

static void zkAnimInit() {
    zk::PageAnim::init(9, 16);        /* 与 REGISTER_ACTIVITY_TIMER_TAB 的 {9,16} 对应 */
    zk::PageAnim::setSlideAlphaEnable(false);   /* 本机 setAlpha 不重绘：只做位移 */
    zk::PageAnim::addSlideTop2Bottom(mTitleTextPtr, 420, 0);
    zk::PageAnim::addSlideTop2Bottom(mTitleSubPtr, 420, 60);
    zk::PageAnim::addSlideBottom2Top(mNavBarPtr, 460, 40);
    zk::PageAnim::addSlideLeft2Right(mCalCardPtr, 460, 140);
    zk::PageAnim::addSlideRight2Left(mClockCardPtr, 460, 190);
    zk::PageAnim::addSlideLeft2Right(mPickerCardPtr, 460, 240);
    zk::PageAnim::start();
}

static void onUI_init() {
    zkAnimInit();
  #ifdef FUN_BUILD
  INIT_UI_TIMERS
  #endif // FUN_BUILD
    dtApply();
    calInitCells();
    calRender();
}

static void onUI_intent(const Intent *intentPtr) {
}

static bool sPageShown = false;   /* 首次显示标记：复用页才需要重挂定时器 */
static void onUI_show() {
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
        case 0:
            dtApply();
            break;
        case 1:
            pkPoll();
            break;
        default:
            break;
    }
    return true;
}

static bool ondatetimeActivityTouchEvent(const MotionEvent &ev) {
    switch (ev.mActionStatus) {
        case MotionEvent::E_ACTION_DOWN:
            break;
        case MotionEvent::E_ACTION_MOVE:
            break;
        case MotionEvent::E_ACTION_UP: {
            /* 日历日格点选：网格绝对原点=(CalCard 28,96)+(24,94)，格 60x48 */
            int gx = ev.mX - 52, gy = ev.mY - 190;
            if (gx >= 0 && gy >= 0) {
                int col = gx / 60, row = gy / 48;
                if (col < 7 && row < 6) {
                    int d = row * 7 + col - calFirstWd(sCalY, sCalM) + 1;
                    if (d >= 1 && d <= calDaysIn(sCalY, sCalM)) { sCalSel = d; calRender(); return true; }
                }
            }
            break;
        }
        default:
            break;
    }
    return zkNavTouch(3, ev);
}

static bool onButtonClick_CalPrev(ZKButton* pButton) { (void)pButton; calShiftMonth(-1); return false; }

static bool onButtonClick_CalNext(ZKButton* pButton) { (void)pButton; calShiftMonth(1); return false; }

static bool onButtonClick_NavBtn1(ZKButton* pButton) { return zkNavGo(0); }

static bool onButtonClick_NavBtn2(ZKButton* pButton) { return zkNavGo(1); }

static bool onButtonClick_NavBtn3(ZKButton* pButton) { return zkNavGo(2); }

static bool onButtonClick_NavBtn4(ZKButton* pButton) { return zkNavGo(3); }

static bool onButtonClick_NavBtn5(ZKButton* pButton) { return zkNavGo(4); }

static bool onButtonClick_NavBtn6(ZKButton* pButton) { return zkNavGo(5); }

static bool onButtonClick_NavBtn7(ZKButton* pButton) { return zkNavGo(6); }

static bool onButtonClick_NavBtn8(ZKButton* pButton) { return zkNavGo(7); }
