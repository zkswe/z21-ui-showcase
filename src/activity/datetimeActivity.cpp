/***********************************************
/gen auto by zuitools
***********************************************/
#include "datetimeActivity.h"

/*TAG:GlobalVariable全局变量*/
static ZKButton* mNavBtn8Ptr;
static ZKButton* mNavBtn7Ptr;
static ZKButton* mNavBtn6Ptr;
static ZKButton* mNavBtn5Ptr;
static ZKButton* mNavBtn4Ptr;
static ZKButton* mNavBtn3Ptr;
static ZKButton* mNavBtn2Ptr;
static ZKButton* mNavBtn1Ptr;
static ZKTextView* mNavLabel8Ptr;
static ZKTextView* mNavIcon8Ptr;
static ZKTextView* mNavLabel7Ptr;
static ZKTextView* mNavIcon7Ptr;
static ZKTextView* mNavLabel6Ptr;
static ZKTextView* mNavIcon6Ptr;
static ZKTextView* mNavLabel5Ptr;
static ZKTextView* mNavIcon5Ptr;
static ZKTextView* mNavLabel4Ptr;
static ZKTextView* mNavIcon4Ptr;
static ZKTextView* mNavLabel3Ptr;
static ZKTextView* mNavIcon3Ptr;
static ZKTextView* mNavLabel2Ptr;
static ZKTextView* mNavIcon2Ptr;
static ZKTextView* mNavLabel1Ptr;
static ZKTextView* mNavIcon1Ptr;
static ZKWindow* mNavBarPtr;
static ZKTextView* mPk33Ptr;
static ZKTextView* mPk32Ptr;
static ZKTextView* mPk31Ptr;
static ZKTextView* mPk23Ptr;
static ZKTextView* mPk22Ptr;
static ZKTextView* mPk21Ptr;
static ZKTextView* mPk13Ptr;
static ZKTextView* mPk12Ptr;
static ZKTextView* mPk11Ptr;
static ZKTextView* mPkBandPtr;
static ZKTextView* mPkColH3Ptr;
static ZKTextView* mPkColH2Ptr;
static ZKTextView* mPkColH1Ptr;
static ZKTextView* mPickerTitlePtr;
static ZKWindow* mPickerCardPtr;
static ZKTextView* mNeedleHubPtr;
static ZKTextView* mNeedlePtr;
static ZKTextView* mDialH12Ptr;
static ZKTextView* mDialH11Ptr;
static ZKTextView* mDialH10Ptr;
static ZKTextView* mDialH9Ptr;
static ZKTextView* mDialH8Ptr;
static ZKTextView* mDialH7Ptr;
static ZKTextView* mDialH6Ptr;
static ZKTextView* mDialH5Ptr;
static ZKTextView* mDialH4Ptr;
static ZKTextView* mDialH3Ptr;
static ZKTextView* mDialH2Ptr;
static ZKTextView* mDialH1Ptr;
static ZKWindow* mDialPtr;
static ZKTextView* mImageView6Ptr;
static ZKTextView* mClockDatePtr;
static ZKTextView* mClockBigPtr;
static ZKWindow* mClockCardPtr;
static ZKTextView* mCalD31Ptr;
static ZKTextView* mCalD30Ptr;
static ZKTextView* mCalD29Ptr;
static ZKTextView* mCalD28Ptr;
static ZKTextView* mCalD27Ptr;
static ZKTextView* mCalD26Ptr;
static ZKTextView* mCalD25Ptr;
static ZKTextView* mCalD24Ptr;
static ZKTextView* mCalD23Ptr;
static ZKTextView* mCalD22Ptr;
static ZKTextView* mCalD21Ptr;
static ZKTextView* mCalD20Ptr;
static ZKTextView* mCalD19Ptr;
static ZKTextView* mCalD18Ptr;
static ZKTextView* mCalD17Ptr;
static ZKTextView* mCalD16Ptr;
static ZKTextView* mCalD15Ptr;
static ZKTextView* mCalD14Ptr;
static ZKTextView* mCalD13Ptr;
static ZKTextView* mCalD12Ptr;
static ZKTextView* mCalD11Ptr;
static ZKTextView* mCalD10Ptr;
static ZKTextView* mCalD9Ptr;
static ZKTextView* mCalD8Ptr;
static ZKTextView* mCalD7Ptr;
static ZKTextView* mCalD6Ptr;
static ZKTextView* mCalD5Ptr;
static ZKTextView* mCalD4Ptr;
static ZKTextView* mCalD3Ptr;
static ZKTextView* mCalD2Ptr;
static ZKTextView* mCalD1Ptr;
static ZKTextView* mCalW7Ptr;
static ZKTextView* mCalW6Ptr;
static ZKTextView* mCalW5Ptr;
static ZKTextView* mCalW4Ptr;
static ZKTextView* mCalW3Ptr;
static ZKTextView* mCalW2Ptr;
static ZKTextView* mCalW1Ptr;
static ZKTextView* mImageView5Ptr;
static ZKTextView* mImageView4Ptr;
static ZKTextView* mCalTitlePtr;
static ZKWindow* mCalCardPtr;
static ZKTextView* mImageView3Ptr;
static ZKTextView* mImageView2Ptr;
static ZKTextView* mImageView1Ptr;
static ZKTextView* mTitleSubPtr;
static ZKTextView* mTitleTextPtr;
static datetimeActivity* mActivityPtr;

/*register activity*/
REGISTER_ACTIVITY(datetimeActivity);

typedef struct {
	int id; // 定时器ID ， 不能重复
	int time; // 定时器  时间间隔  单位 毫秒
}S_ACTIVITY_TIMEER;

#include "logic/datetimeLogic.cc"

/***********/
typedef struct {
    int id;
    const char *pApp;
} SAppInfo;

/**
 *点击跳转window
 */
static SAppInfo sAppInfoTab[] = {
//  { ID_MAIN_TEXT, "TextViewActivity" },
};

/***************/
typedef bool (*ButtonCallback)(ZKButton *pButton);
/**
 * button onclick表
 */
typedef struct {
    int id;
    ButtonCallback callback;
}S_ButtonCallback;

/*TAG:ButtonCallbackTab按键映射表*/
static S_ButtonCallback sButtonCallbackTab[] = {
    ID_DATETIME_NavBtn8, onButtonClick_NavBtn8,
    ID_DATETIME_NavBtn7, onButtonClick_NavBtn7,
    ID_DATETIME_NavBtn6, onButtonClick_NavBtn6,
    ID_DATETIME_NavBtn5, onButtonClick_NavBtn5,
    ID_DATETIME_NavBtn4, onButtonClick_NavBtn4,
    ID_DATETIME_NavBtn3, onButtonClick_NavBtn3,
    ID_DATETIME_NavBtn2, onButtonClick_NavBtn2,
    ID_DATETIME_NavBtn1, onButtonClick_NavBtn1,
};
/***************/


typedef void (*SeekBarCallback)(ZKSeekBar *pSeekBar, int progress);
typedef struct {
    int id;
    SeekBarCallback callback;
}S_ZKSeekBarCallback;
/*TAG:SeekBarCallbackTab*/
static S_ZKSeekBarCallback SZKSeekBarCallbackTab[] = {
};


typedef int (*ListViewGetItemCountCallback)(const ZKListView *pListView);
typedef void (*ListViewobtainListItemDataCallback)(ZKListView *pListView,ZKListView::ZKListItem *pListItem, int index);
typedef void (*ListViewonItemClickCallback)(ZKListView *pListView, int index, int id);
typedef struct {
    int id;
    ListViewGetItemCountCallback getListItemCountCallback;
    ListViewobtainListItemDataCallback obtainListItemDataCallback;
    ListViewonItemClickCallback onItemClickCallback;
}S_ListViewFunctionsCallback;
/*TAG:ListViewFunctionsCallback*/
static S_ListViewFunctionsCallback SListViewFunctionsCallbackTab[] = {
};


typedef void (*SlideWindowItemClickCallback)(ZKSlideWindow *pSlideWindow, int index);
typedef struct {
    int id;
    SlideWindowItemClickCallback onSlideItemClickCallback;
}S_SlideWindowItemClickCallback;
/*TAG:SlideWindowFunctionsCallbackTab*/
static S_SlideWindowItemClickCallback SSlideWindowItemClickCallbackTab[] = {
};


typedef void (*EditTextInputCallback)(const std::string &text);
typedef struct {
    int id;
    EditTextInputCallback onEditTextChangedCallback;
}S_EditTextInputCallback;
/*TAG:EditTextInputCallback*/
static S_EditTextInputCallback SEditTextInputCallbackTab[] = {
};

typedef void (*VideoViewCallback)(ZKVideoView *pVideoView, int msg);
typedef struct {
    int id; //VideoView ID
    bool loop; // 是否是轮播类型
    int defaultvolume;//轮播类型时,默认视频音量
    VideoViewCallback onVideoViewCallback;
}S_VideoViewCallback;
/*TAG:VideoViewCallback*/
static S_VideoViewCallback SVideoViewCallbackTab[] = {
};


datetimeActivity::datetimeActivity() {
	//todo add init code here
	mVideoLoopIndex = -1;
	mVideoLoopErrorCount = 0;
}

datetimeActivity::~datetimeActivity() {
  //todo add init file here
  // 退出应用时需要反注册
    EASYUICONTEXT->unregisterGlobalTouchListener(this);
    unregisterProtocolDataUpdateListener(onProtocolDataUpdate);
    onUI_quit();
    mActivityPtr = NULL;
    mNavBtn8Ptr = NULL;
    mNavBtn7Ptr = NULL;
    mNavBtn6Ptr = NULL;
    mNavBtn5Ptr = NULL;
    mNavBtn4Ptr = NULL;
    mNavBtn3Ptr = NULL;
    mNavBtn2Ptr = NULL;
    mNavBtn1Ptr = NULL;
    mNavLabel8Ptr = NULL;
    mNavIcon8Ptr = NULL;
    mNavLabel7Ptr = NULL;
    mNavIcon7Ptr = NULL;
    mNavLabel6Ptr = NULL;
    mNavIcon6Ptr = NULL;
    mNavLabel5Ptr = NULL;
    mNavIcon5Ptr = NULL;
    mNavLabel4Ptr = NULL;
    mNavIcon4Ptr = NULL;
    mNavLabel3Ptr = NULL;
    mNavIcon3Ptr = NULL;
    mNavLabel2Ptr = NULL;
    mNavIcon2Ptr = NULL;
    mNavLabel1Ptr = NULL;
    mNavIcon1Ptr = NULL;
    mNavBarPtr = NULL;
    mPk33Ptr = NULL;
    mPk32Ptr = NULL;
    mPk31Ptr = NULL;
    mPk23Ptr = NULL;
    mPk22Ptr = NULL;
    mPk21Ptr = NULL;
    mPk13Ptr = NULL;
    mPk12Ptr = NULL;
    mPk11Ptr = NULL;
    mPkBandPtr = NULL;
    mPkColH3Ptr = NULL;
    mPkColH2Ptr = NULL;
    mPkColH1Ptr = NULL;
    mPickerTitlePtr = NULL;
    mPickerCardPtr = NULL;
    mNeedleHubPtr = NULL;
    mNeedlePtr = NULL;
    mDialH12Ptr = NULL;
    mDialH11Ptr = NULL;
    mDialH10Ptr = NULL;
    mDialH9Ptr = NULL;
    mDialH8Ptr = NULL;
    mDialH7Ptr = NULL;
    mDialH6Ptr = NULL;
    mDialH5Ptr = NULL;
    mDialH4Ptr = NULL;
    mDialH3Ptr = NULL;
    mDialH2Ptr = NULL;
    mDialH1Ptr = NULL;
    mDialPtr = NULL;
    mImageView6Ptr = NULL;
    mClockDatePtr = NULL;
    mClockBigPtr = NULL;
    mClockCardPtr = NULL;
    mCalD31Ptr = NULL;
    mCalD30Ptr = NULL;
    mCalD29Ptr = NULL;
    mCalD28Ptr = NULL;
    mCalD27Ptr = NULL;
    mCalD26Ptr = NULL;
    mCalD25Ptr = NULL;
    mCalD24Ptr = NULL;
    mCalD23Ptr = NULL;
    mCalD22Ptr = NULL;
    mCalD21Ptr = NULL;
    mCalD20Ptr = NULL;
    mCalD19Ptr = NULL;
    mCalD18Ptr = NULL;
    mCalD17Ptr = NULL;
    mCalD16Ptr = NULL;
    mCalD15Ptr = NULL;
    mCalD14Ptr = NULL;
    mCalD13Ptr = NULL;
    mCalD12Ptr = NULL;
    mCalD11Ptr = NULL;
    mCalD10Ptr = NULL;
    mCalD9Ptr = NULL;
    mCalD8Ptr = NULL;
    mCalD7Ptr = NULL;
    mCalD6Ptr = NULL;
    mCalD5Ptr = NULL;
    mCalD4Ptr = NULL;
    mCalD3Ptr = NULL;
    mCalD2Ptr = NULL;
    mCalD1Ptr = NULL;
    mCalW7Ptr = NULL;
    mCalW6Ptr = NULL;
    mCalW5Ptr = NULL;
    mCalW4Ptr = NULL;
    mCalW3Ptr = NULL;
    mCalW2Ptr = NULL;
    mCalW1Ptr = NULL;
    mImageView5Ptr = NULL;
    mImageView4Ptr = NULL;
    mCalTitlePtr = NULL;
    mCalCardPtr = NULL;
    mImageView3Ptr = NULL;
    mImageView2Ptr = NULL;
    mImageView1Ptr = NULL;
    mTitleSubPtr = NULL;
    mTitleTextPtr = NULL;
}

const char* datetimeActivity::getAppName() const{
	return "datetime.ftu";
}

//TAG:onCreate
void datetimeActivity::onCreate() {
	Activity::onCreate();
    mNavBtn8Ptr = (ZKButton*)findControlByID(ID_DATETIME_NavBtn8);
    mNavBtn7Ptr = (ZKButton*)findControlByID(ID_DATETIME_NavBtn7);
    mNavBtn6Ptr = (ZKButton*)findControlByID(ID_DATETIME_NavBtn6);
    mNavBtn5Ptr = (ZKButton*)findControlByID(ID_DATETIME_NavBtn5);
    mNavBtn4Ptr = (ZKButton*)findControlByID(ID_DATETIME_NavBtn4);
    mNavBtn3Ptr = (ZKButton*)findControlByID(ID_DATETIME_NavBtn3);
    mNavBtn2Ptr = (ZKButton*)findControlByID(ID_DATETIME_NavBtn2);
    mNavBtn1Ptr = (ZKButton*)findControlByID(ID_DATETIME_NavBtn1);
    mNavLabel8Ptr = (ZKTextView*)findControlByID(ID_DATETIME_NavLabel8);
    mNavIcon8Ptr = (ZKTextView*)findControlByID(ID_DATETIME_NavIcon8);
    mNavLabel7Ptr = (ZKTextView*)findControlByID(ID_DATETIME_NavLabel7);
    mNavIcon7Ptr = (ZKTextView*)findControlByID(ID_DATETIME_NavIcon7);
    mNavLabel6Ptr = (ZKTextView*)findControlByID(ID_DATETIME_NavLabel6);
    mNavIcon6Ptr = (ZKTextView*)findControlByID(ID_DATETIME_NavIcon6);
    mNavLabel5Ptr = (ZKTextView*)findControlByID(ID_DATETIME_NavLabel5);
    mNavIcon5Ptr = (ZKTextView*)findControlByID(ID_DATETIME_NavIcon5);
    mNavLabel4Ptr = (ZKTextView*)findControlByID(ID_DATETIME_NavLabel4);
    mNavIcon4Ptr = (ZKTextView*)findControlByID(ID_DATETIME_NavIcon4);
    mNavLabel3Ptr = (ZKTextView*)findControlByID(ID_DATETIME_NavLabel3);
    mNavIcon3Ptr = (ZKTextView*)findControlByID(ID_DATETIME_NavIcon3);
    mNavLabel2Ptr = (ZKTextView*)findControlByID(ID_DATETIME_NavLabel2);
    mNavIcon2Ptr = (ZKTextView*)findControlByID(ID_DATETIME_NavIcon2);
    mNavLabel1Ptr = (ZKTextView*)findControlByID(ID_DATETIME_NavLabel1);
    mNavIcon1Ptr = (ZKTextView*)findControlByID(ID_DATETIME_NavIcon1);
    mNavBarPtr = (ZKWindow*)findControlByID(ID_DATETIME_NavBar);
    mPk33Ptr = (ZKTextView*)findControlByID(ID_DATETIME_Pk33);
    mPk32Ptr = (ZKTextView*)findControlByID(ID_DATETIME_Pk32);
    mPk31Ptr = (ZKTextView*)findControlByID(ID_DATETIME_Pk31);
    mPk23Ptr = (ZKTextView*)findControlByID(ID_DATETIME_Pk23);
    mPk22Ptr = (ZKTextView*)findControlByID(ID_DATETIME_Pk22);
    mPk21Ptr = (ZKTextView*)findControlByID(ID_DATETIME_Pk21);
    mPk13Ptr = (ZKTextView*)findControlByID(ID_DATETIME_Pk13);
    mPk12Ptr = (ZKTextView*)findControlByID(ID_DATETIME_Pk12);
    mPk11Ptr = (ZKTextView*)findControlByID(ID_DATETIME_Pk11);
    mPkBandPtr = (ZKTextView*)findControlByID(ID_DATETIME_PkBand);
    mPkColH3Ptr = (ZKTextView*)findControlByID(ID_DATETIME_PkColH3);
    mPkColH2Ptr = (ZKTextView*)findControlByID(ID_DATETIME_PkColH2);
    mPkColH1Ptr = (ZKTextView*)findControlByID(ID_DATETIME_PkColH1);
    mPickerTitlePtr = (ZKTextView*)findControlByID(ID_DATETIME_PickerTitle);
    mPickerCardPtr = (ZKWindow*)findControlByID(ID_DATETIME_PickerCard);
    mNeedleHubPtr = (ZKTextView*)findControlByID(ID_DATETIME_NeedleHub);
    mNeedlePtr = (ZKTextView*)findControlByID(ID_DATETIME_Needle);
    mDialH12Ptr = (ZKTextView*)findControlByID(ID_DATETIME_DialH12);
    mDialH11Ptr = (ZKTextView*)findControlByID(ID_DATETIME_DialH11);
    mDialH10Ptr = (ZKTextView*)findControlByID(ID_DATETIME_DialH10);
    mDialH9Ptr = (ZKTextView*)findControlByID(ID_DATETIME_DialH9);
    mDialH8Ptr = (ZKTextView*)findControlByID(ID_DATETIME_DialH8);
    mDialH7Ptr = (ZKTextView*)findControlByID(ID_DATETIME_DialH7);
    mDialH6Ptr = (ZKTextView*)findControlByID(ID_DATETIME_DialH6);
    mDialH5Ptr = (ZKTextView*)findControlByID(ID_DATETIME_DialH5);
    mDialH4Ptr = (ZKTextView*)findControlByID(ID_DATETIME_DialH4);
    mDialH3Ptr = (ZKTextView*)findControlByID(ID_DATETIME_DialH3);
    mDialH2Ptr = (ZKTextView*)findControlByID(ID_DATETIME_DialH2);
    mDialH1Ptr = (ZKTextView*)findControlByID(ID_DATETIME_DialH1);
    mDialPtr = (ZKWindow*)findControlByID(ID_DATETIME_Dial);
    mImageView6Ptr = (ZKTextView*)findControlByID(ID_DATETIME_ImageView6);
    mClockDatePtr = (ZKTextView*)findControlByID(ID_DATETIME_ClockDate);
    mClockBigPtr = (ZKTextView*)findControlByID(ID_DATETIME_ClockBig);
    mClockCardPtr = (ZKWindow*)findControlByID(ID_DATETIME_ClockCard);
    mCalD31Ptr = (ZKTextView*)findControlByID(ID_DATETIME_CalD31);
    mCalD30Ptr = (ZKTextView*)findControlByID(ID_DATETIME_CalD30);
    mCalD29Ptr = (ZKTextView*)findControlByID(ID_DATETIME_CalD29);
    mCalD28Ptr = (ZKTextView*)findControlByID(ID_DATETIME_CalD28);
    mCalD27Ptr = (ZKTextView*)findControlByID(ID_DATETIME_CalD27);
    mCalD26Ptr = (ZKTextView*)findControlByID(ID_DATETIME_CalD26);
    mCalD25Ptr = (ZKTextView*)findControlByID(ID_DATETIME_CalD25);
    mCalD24Ptr = (ZKTextView*)findControlByID(ID_DATETIME_CalD24);
    mCalD23Ptr = (ZKTextView*)findControlByID(ID_DATETIME_CalD23);
    mCalD22Ptr = (ZKTextView*)findControlByID(ID_DATETIME_CalD22);
    mCalD21Ptr = (ZKTextView*)findControlByID(ID_DATETIME_CalD21);
    mCalD20Ptr = (ZKTextView*)findControlByID(ID_DATETIME_CalD20);
    mCalD19Ptr = (ZKTextView*)findControlByID(ID_DATETIME_CalD19);
    mCalD18Ptr = (ZKTextView*)findControlByID(ID_DATETIME_CalD18);
    mCalD17Ptr = (ZKTextView*)findControlByID(ID_DATETIME_CalD17);
    mCalD16Ptr = (ZKTextView*)findControlByID(ID_DATETIME_CalD16);
    mCalD15Ptr = (ZKTextView*)findControlByID(ID_DATETIME_CalD15);
    mCalD14Ptr = (ZKTextView*)findControlByID(ID_DATETIME_CalD14);
    mCalD13Ptr = (ZKTextView*)findControlByID(ID_DATETIME_CalD13);
    mCalD12Ptr = (ZKTextView*)findControlByID(ID_DATETIME_CalD12);
    mCalD11Ptr = (ZKTextView*)findControlByID(ID_DATETIME_CalD11);
    mCalD10Ptr = (ZKTextView*)findControlByID(ID_DATETIME_CalD10);
    mCalD9Ptr = (ZKTextView*)findControlByID(ID_DATETIME_CalD9);
    mCalD8Ptr = (ZKTextView*)findControlByID(ID_DATETIME_CalD8);
    mCalD7Ptr = (ZKTextView*)findControlByID(ID_DATETIME_CalD7);
    mCalD6Ptr = (ZKTextView*)findControlByID(ID_DATETIME_CalD6);
    mCalD5Ptr = (ZKTextView*)findControlByID(ID_DATETIME_CalD5);
    mCalD4Ptr = (ZKTextView*)findControlByID(ID_DATETIME_CalD4);
    mCalD3Ptr = (ZKTextView*)findControlByID(ID_DATETIME_CalD3);
    mCalD2Ptr = (ZKTextView*)findControlByID(ID_DATETIME_CalD2);
    mCalD1Ptr = (ZKTextView*)findControlByID(ID_DATETIME_CalD1);
    mCalW7Ptr = (ZKTextView*)findControlByID(ID_DATETIME_CalW7);
    mCalW6Ptr = (ZKTextView*)findControlByID(ID_DATETIME_CalW6);
    mCalW5Ptr = (ZKTextView*)findControlByID(ID_DATETIME_CalW5);
    mCalW4Ptr = (ZKTextView*)findControlByID(ID_DATETIME_CalW4);
    mCalW3Ptr = (ZKTextView*)findControlByID(ID_DATETIME_CalW3);
    mCalW2Ptr = (ZKTextView*)findControlByID(ID_DATETIME_CalW2);
    mCalW1Ptr = (ZKTextView*)findControlByID(ID_DATETIME_CalW1);
    mImageView5Ptr = (ZKTextView*)findControlByID(ID_DATETIME_ImageView5);
    mImageView4Ptr = (ZKTextView*)findControlByID(ID_DATETIME_ImageView4);
    mCalTitlePtr = (ZKTextView*)findControlByID(ID_DATETIME_CalTitle);
    mCalCardPtr = (ZKWindow*)findControlByID(ID_DATETIME_CalCard);
    mImageView3Ptr = (ZKTextView*)findControlByID(ID_DATETIME_ImageView3);
    mImageView2Ptr = (ZKTextView*)findControlByID(ID_DATETIME_ImageView2);
    mImageView1Ptr = (ZKTextView*)findControlByID(ID_DATETIME_ImageView1);
    mTitleSubPtr = (ZKTextView*)findControlByID(ID_DATETIME_TitleSub);
    mTitleTextPtr = (ZKTextView*)findControlByID(ID_DATETIME_TitleText);
	mActivityPtr = this;
	onUI_init();
  registerProtocolDataUpdateListener(onProtocolDataUpdate);
  rigesterActivityTimer();
}

void datetimeActivity::onClick(ZKBase *pBase) {
	//TODO: add widget onClik code 
    int buttonTablen = sizeof(sButtonCallbackTab) / sizeof(S_ButtonCallback);
    for (int i = 0; i < buttonTablen; ++i) {
        if (sButtonCallbackTab[i].id == pBase->getID()) {
            if (sButtonCallbackTab[i].callback((ZKButton*)pBase)) {
            	return;
            }
            break;
        }
    }


    int len = sizeof(sAppInfoTab) / sizeof(sAppInfoTab[0]);
    for (int i = 0; i < len; ++i) {
        if (sAppInfoTab[i].id == pBase->getID()) {
            EASYUICONTEXT->openActivity(sAppInfoTab[i].pApp);
            return;
        }
    }

	Activity::onClick(pBase);
}

void datetimeActivity::onResume() {
	Activity::onResume();
	EASYUICONTEXT->registerGlobalTouchListener(this);
	startVideoLoopPlayback();
	onUI_show();
}

void datetimeActivity::onPause() {
	Activity::onPause();
	EASYUICONTEXT->unregisterGlobalTouchListener(this);
	stopVideoLoopPlayback();
	onUI_hide();
}

void datetimeActivity::onIntent(const Intent *intentPtr) {
	Activity::onIntent(intentPtr);
	onUI_intent(intentPtr);
}

bool datetimeActivity::onTimer(int id) {
	return onUI_Timer(id);
}

void datetimeActivity::onProgressChanged(ZKSeekBar *pSeekBar, int progress){

    int seekBarTablen = sizeof(SZKSeekBarCallbackTab) / sizeof(S_ZKSeekBarCallback);
    for (int i = 0; i < seekBarTablen; ++i) {
        if (SZKSeekBarCallbackTab[i].id == pSeekBar->getID()) {
            SZKSeekBarCallbackTab[i].callback(pSeekBar, progress);
            break;
        }
    }
}

int datetimeActivity::getListItemCount(const ZKListView *pListView) const{
    int tablen = sizeof(SListViewFunctionsCallbackTab) / sizeof(S_ListViewFunctionsCallback);
    for (int i = 0; i < tablen; ++i) {
        if (SListViewFunctionsCallbackTab[i].id == pListView->getID()) {
            return SListViewFunctionsCallbackTab[i].getListItemCountCallback(pListView);
            break;
        }
    }
    return 0;
}

void datetimeActivity::obtainListItemData(ZKListView *pListView,ZKListView::ZKListItem *pListItem, int index){
    int tablen = sizeof(SListViewFunctionsCallbackTab) / sizeof(S_ListViewFunctionsCallback);
    for (int i = 0; i < tablen; ++i) {
        if (SListViewFunctionsCallbackTab[i].id == pListView->getID()) {
            SListViewFunctionsCallbackTab[i].obtainListItemDataCallback(pListView, pListItem, index);
            break;
        }
    }
}

void datetimeActivity::onItemClick(ZKListView *pListView, int index, int id){
    int tablen = sizeof(SListViewFunctionsCallbackTab) / sizeof(S_ListViewFunctionsCallback);
    for (int i = 0; i < tablen; ++i) {
        if (SListViewFunctionsCallbackTab[i].id == pListView->getID()) {
            SListViewFunctionsCallbackTab[i].onItemClickCallback(pListView, index, id);
            break;
        }
    }
}

void datetimeActivity::onSlideItemClick(ZKSlideWindow *pSlideWindow, int index) {
    int tablen = sizeof(SSlideWindowItemClickCallbackTab) / sizeof(S_SlideWindowItemClickCallback);
    for (int i = 0; i < tablen; ++i) {
        if (SSlideWindowItemClickCallbackTab[i].id == pSlideWindow->getID()) {
            SSlideWindowItemClickCallbackTab[i].onSlideItemClickCallback(pSlideWindow, index);
            break;
        }
    }
}

bool datetimeActivity::onTouchEvent(const MotionEvent &ev) {
    return ondatetimeActivityTouchEvent(ev);
}

void datetimeActivity::onTextChanged(ZKTextView *pTextView, const std::string &text) {
    int tablen = sizeof(SEditTextInputCallbackTab) / sizeof(S_EditTextInputCallback);
    for (int i = 0; i < tablen; ++i) {
        if (SEditTextInputCallbackTab[i].id == pTextView->getID()) {
            SEditTextInputCallbackTab[i].onEditTextChangedCallback(text);
            break;
        }
    }
}

void datetimeActivity::rigesterActivityTimer() {
    int tablen = sizeof(REGISTER_ACTIVITY_TIMER_TAB) / sizeof(S_ACTIVITY_TIMEER);
    for (int i = 0; i < tablen; ++i) {
        S_ACTIVITY_TIMEER temp = REGISTER_ACTIVITY_TIMER_TAB[i];
        registerTimer(temp.id, temp.time);
    }
}


void datetimeActivity::onVideoPlayerMessage(ZKVideoView *pVideoView, int msg) {
    int tablen = sizeof(SVideoViewCallbackTab) / sizeof(S_VideoViewCallback);
    for (int i = 0; i < tablen; ++i) {
        if (SVideoViewCallbackTab[i].id == pVideoView->getID()) {
        	if (SVideoViewCallbackTab[i].loop) {
                //循环播放
        		videoLoopPlayback(pVideoView, msg, i);
        	} else if (SVideoViewCallbackTab[i].onVideoViewCallback != NULL){
        	    SVideoViewCallbackTab[i].onVideoViewCallback(pVideoView, msg);
        	}
            break;
        }
    }
}

void datetimeActivity::videoLoopPlayback(ZKVideoView *pVideoView, int msg, size_t callbackTabIndex) {

	switch (msg) {
	case ZKVideoView::E_MSGTYPE_VIDEO_PLAY_STARTED:
		LOGD("ZKVideoView::E_MSGTYPE_VIDEO_PLAY_STARTED\n");
    if (callbackTabIndex >= (sizeof(SVideoViewCallbackTab)/sizeof(S_VideoViewCallback))) {
      break;
    }
		pVideoView->setVolume(SVideoViewCallbackTab[callbackTabIndex].defaultvolume / 10.0);
		mVideoLoopErrorCount = 0;
		break;
	case ZKVideoView::E_MSGTYPE_VIDEO_PLAY_ERROR:
		/**错误处理 */
		++mVideoLoopErrorCount;
		if (mVideoLoopErrorCount > 100) {
			LOGD("video loop error counts > 100, quit loop playback !");
            break;
		} //不用break, 继续尝试播放下一个
	case ZKVideoView::E_MSGTYPE_VIDEO_PLAY_COMPLETED:
		LOGD("ZKVideoView::E_MSGTYPE_VIDEO_PLAY_COMPLETED\n");
        std::vector<std::string> videolist;
        std::string fileName(getAppName());
        if (fileName.size() < 4) {
             LOGD("getAppName size < 4, ignore!");
             break;
        }
        fileName = fileName.substr(0, fileName.length() - 4) + "_video_list.txt";
        fileName = "/mnt/extsd/" + fileName;
        if (!parseVideoFileList(fileName.c_str(), videolist)) {
            LOGD("parseVideoFileList failed !");
		    break;
        }
		if (pVideoView && !videolist.empty()) {
			mVideoLoopIndex = (mVideoLoopIndex + 1) % videolist.size();
			pVideoView->play(videolist[mVideoLoopIndex].c_str());
		}
		break;
	}
}

void datetimeActivity::startVideoLoopPlayback() {
    int tablen = sizeof(SVideoViewCallbackTab) / sizeof(S_VideoViewCallback);
    for (int i = 0; i < tablen; ++i) {
    	if (SVideoViewCallbackTab[i].loop) {
    		ZKVideoView* videoView = (ZKVideoView*)findControlByID(SVideoViewCallbackTab[i].id);
    		if (!videoView) {
    			return;
    		}
    		//循环播放
    		videoLoopPlayback(videoView, ZKVideoView::E_MSGTYPE_VIDEO_PLAY_COMPLETED, i);
    		return;
    	}
    }
}

void datetimeActivity::stopVideoLoopPlayback() {
    int tablen = sizeof(SVideoViewCallbackTab) / sizeof(S_VideoViewCallback);
    for (int i = 0; i < tablen; ++i) {
    	if (SVideoViewCallbackTab[i].loop) {
    		ZKVideoView* videoView = (ZKVideoView*)findControlByID(SVideoViewCallbackTab[i].id);
    		if (!videoView) {
    			return;
    		}
    		if (videoView->isPlaying()) {
    		    videoView->stop();
    		}
    		return;
    	}
    }
}

bool datetimeActivity::parseVideoFileList(const char *pFileListPath, std::vector<string>& mediaFileList) {
	mediaFileList.clear();
	if (NULL == pFileListPath || 0 == strlen(pFileListPath)) {
        LOGD("video file list is null!");
		return false;
	}

	ifstream is(pFileListPath, ios_base::in);
	if (!is.is_open()) {
		LOGD("cann't open file %s \n", pFileListPath);
		return false;
	}
	char tmp[1024] = {0};
	while (is.getline(tmp, sizeof(tmp))) {
		string str = tmp;
		removeCharFromString(str, '\"');
		removeCharFromString(str, '\r');
		removeCharFromString(str, '\n');
		if (str.size() > 1) {
     		mediaFileList.push_back(str.c_str());
		}
	}
  LOGD("(f:%s, l:%d) parse fileList[%s], get [%d]files", __FUNCTION__,
      __LINE__, pFileListPath, int(mediaFileList.size()));
  for (std::vector<string>::size_type i = 0; i < mediaFileList.size(); i++) {
    LOGD("file[%d]:[%s]", int(i), mediaFileList[i].c_str());
  }
	is.close();

	return true;
}

int datetimeActivity::removeCharFromString(string& nString, char c) {
    string::size_type   pos;
    while(1) {
        pos = nString.find(c);
        if(pos != string::npos) {
            nString.erase(pos, 1);
        } else {
            break;
        }
    }
    return (int)nString.size();
}

void datetimeActivity::registerUserTimer(int id, int time) {
	registerTimer(id, time);
}

void datetimeActivity::unregisterUserTimer(int id) {
	unregisterTimer(id);
}

void datetimeActivity::resetUserTimer(int id, int time) {
	resetTimer(id, time);
}