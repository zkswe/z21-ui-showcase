#include <base/base.h>
#ifdef FUN_BUILD
#include GENERATED_UI_DEFINITIONS
INIT_UI_EVENT_BINDINGS
#endif // FUN_BUILD
#pragma once
#include "uart/ProtocolSender.h"
#include "ui/zk_ui_nav.h"
#include "zk/zk_page_anim.h"

#include "zk/zk_imagecache.h"
#include "manager/ConfigManager.h"
#include <utils/BitmapHelper.h>
#include <base/log.h>
#include <string>
#include <vector>

/* 由设计稿装配生成（UIShowcase-Z21） */

/* ============================================================================
 * 封面墙：listview 动态数据 + imagecache（按路径缓存「已解码位图」）
 * 口径 knowledge/uicontrols/listview-image-cache.md 两件套：
 *   1) 取图尺寸 == 显示盒（本页出图 142x88 == SubCovCover 盒）
 *   2) fill 里 setBackgroundPic 之后紧跟 imagecache().cache(path)：回页/重建 item 不再重解
 * 硬约束：缓存键 = 路径，路径必须唯一（本页 8 张固定名不换内容，安全）
 * ============================================================================ */
namespace {
struct zkAlbumItem { const char *name; const char *file; };

/* 运行时构建的列表数据（listview 适配器从这里取；要加歌单/封面墙条目就往这里加） */
std::vector<zkAlbumItem> gAlbums;

void zkAlbumsBuild() {
    gAlbums.clear();
    const char *files[] = {"cov_album_01.png", "cov_album_02.png", "cov_album_03.png", "cov_album_04.png",
                           "cov_album_05.png", "cov_album_06.png", "cov_album_07.png", "cov_album_08.png"};
    const int n = (int)(sizeof(files) / sizeof(files[0]));
    for (int i = 0; i < n; ++i) {
        zkAlbumItem it;
        static char names[8][16];
        snprintf(names[i], sizeof(names[i]), "专辑 %d", i + 1);
        it.name = names[i];
        it.file = files[i];
        gAlbums.push_back(it);
    }
}

std::string zkCoverPath(int i) {
    return CONFIGMANAGER->getResFilePath(std::string("images/") + gAlbums[i].file);
}

/* imagecache 装载/释放 = 交给框架 BitmapHelper（解好的位图登记进框架资源表并持有） */
int zkIcLoad(const char *path, zk::Bitmap *out, void *user) {
    (void)user;
    bitmap_t *bmp = NULL;
    if (!BitmapHelper::loadBitmapFromFile(bmp, path) || bmp == NULL) return -1;
    *out = (zk::Bitmap)bmp;
    return 0;
}
void zkIcFree(zk::Bitmap bmp, void *user) {
    (void)user;
    bitmap_t *b = (bitmap_t *)bmp;   /* unloadBitmap 要 _bitmap_t*& 非常引用，必须给具名左值 */
    BitmapHelper::unloadBitmap(b);
}
void zkIcLog(int level, const char *msg, void *user) {
    (void)user;
    /* 命中/装载明细是 DEBUG（fill 每次都调，很吵），现场只留 WARN/ERROR；
       要查命中/解码次数看 imagecache 的 hits()/loads() 或临时把 DEBUG 也接出来 */
    if (level != ZK_IC_LOG_DEBUG) { LOGW_TRACE("%s", msg); }
}

void zkCoverCacheSetup() {
    zk::ImageCache::Config cfg;
    cfg.load = zkIcLoad;
    cfg.free = zkIcFree;
    cfg.user = NULL;
    cfg.capacity = 128;   /* 本页 8 张 142x88，远小于此；Z21 内存紧，别加大 */
    cfg.pathMaxLen = 256;
    zk::ImageCache::instance().setLogHook(zkIcLog, NULL);
    if (!zk::ImageCache::instance().configure(cfg)) {
        LOGW_TRACE("ImageCache configure 失败：%s", zk::ImageCache::instance().lastError());
    }
}
}  /* namespace */

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
    zk::PageAnim::addSlideLeft2Right(mListCardPtr, 460, 140);
    zk::PageAnim::addSlideRight2Left(mGridCardPtr, 460, 190);
    zk::PageAnim::start();
}

static void onUI_init() {
    zkAnimInit();
    zkAlbumsBuild();                           /* 动态构建封面墙数据 */
    zkCoverCacheSetup();                       /* imagecache 配一次（幂等） */
    zk::ImageCache::instance().acquire();      /* 本页开始用封面缓存（与 onUI_quit 的 release 配对） */
  #ifdef FUN_BUILD
  INIT_UI_TIMERS
  #endif // FUN_BUILD
    /* 左侧设备列表：子控件不吞手势（touchable:false 的装饰件仍会挡住 scrollwindow 拖动）；
       封面墙已改 listview（自身 touchable，可拖动），不再逐个 setTouchPass */
    ZKBase* tps[] = { mListScrollCPtr,
        mRow1Ptr, mRow2Ptr, mRow3Ptr, mRow4Ptr, mRow5Ptr, mRow6Ptr, mRow7Ptr, mRow8Ptr,
        mRow1CoverPtr, mRow2CoverPtr, mRow3CoverPtr, mRow4CoverPtr,
        mRow5CoverPtr, mRow6CoverPtr, mRow7CoverPtr, mRow8CoverPtr,
        mRow1IconPtr, mRow2IconPtr, mRow3IconPtr, mRow4IconPtr,
        mRow5IconPtr, mRow6IconPtr, mRow7IconPtr, mRow8IconPtr,
        mRow1NamePtr, mRow2NamePtr, mRow3NamePtr, mRow4NamePtr,
        mRow5NamePtr, mRow6NamePtr, mRow7NamePtr, mRow8NamePtr,
        mRow1SubPtr, mRow2SubPtr, mRow3SubPtr, mRow4SubPtr,
        mRow5SubPtr, mRow6SubPtr, mRow7SubPtr, mRow8SubPtr,
        mRow1ArrowPtr, mRow2ArrowPtr, mRow3ArrowPtr, mRow4ArrowPtr,
        mRow5ArrowPtr, mRow6ArrowPtr, mRow7ArrowPtr, mRow8ArrowPtr };
    for (unsigned i = 0; i < sizeof(tps) / sizeof(tps[0]); ++i) {
        if (tps[i]) tps[i]->setTouchPass(true);
    }
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

static void onUI_quit() {
    zk::PageAnim::release();
    zk::ImageCache::instance().release();   /* 本页退出：最后一个使用者退出才真清（引用计数） */
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

static bool onlistActivityTouchEvent(const MotionEvent &ev) {
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
	return zkNavTouch(4, ev);
}
static bool onButtonClick_NavBtn1(ZKButton* pButton) { return zkNavGo(0); }

static bool onButtonClick_NavBtn2(ZKButton* pButton) { return zkNavGo(1); }

static bool onButtonClick_NavBtn3(ZKButton* pButton) { return zkNavGo(2); }

static bool onButtonClick_NavBtn4(ZKButton* pButton) { return zkNavGo(3); }

static bool onButtonClick_NavBtn5(ZKButton* pButton) { return zkNavGo(4); }

static bool onButtonClick_NavBtn6(ZKButton* pButton) { return zkNavGo(5); }

static bool onButtonClick_NavBtn7(ZKButton* pButton) { return zkNavGo(6); }

static bool onButtonClick_NavBtn8(ZKButton* pButton) { return zkNavGo(7); }

/* ---------- 封面墙 listview 三回调（动态数据 + imagecache） ---------- */
static int getListItemCount_ListCovers(const ZKListView *pListView) {
    (void)pListView;
    return (int)gAlbums.size();
}

static void obtainListItemData_ListCovers(ZKListView *pListView, ZKListView::ZKListItem *pListItem, int index) {
    if (pListView == NULL || pListItem == NULL || index < 0 || index >= (int)gAlbums.size()) return;
    const std::string path = zkCoverPath(index);
    ZKListView::ZKListSubItem *cover = pListItem->findSubItemByID(ID_LIST_SubCovCover);
    if (cover != NULL && !path.empty()) {
        cover->setBackgroundPic(path.c_str());            /* 首次：框架当场解码 */
        zk::ImageCache::instance().cache(path.c_str());   /* 登记已解码位图：再上屏不再解 */
    }
    ZKListView::ZKListSubItem *name = pListItem->findSubItemByID(ID_LIST_SubCovName);
    if (name != NULL) name->setText(gAlbums[index].name);
}

static void onListItemClick_ListCovers(ZKListView *pListView, int index, int id) {
    (void)pListView;
    (void)id;
    if (index < 0 || index >= (int)gAlbums.size()) return;
    LOGD("封面墙：点击 %s (index=%d)", gAlbums[index].name, index);
}


