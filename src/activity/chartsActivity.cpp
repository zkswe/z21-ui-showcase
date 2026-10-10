/***********************************************
/gen auto by zuitools
***********************************************/
#include "chartsActivity.h"

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
static ZKTextView* mB12bPtr;
static ZKTextView* mB12aPtr;
static ZKTextView* mB11bPtr;
static ZKTextView* mB11aPtr;
static ZKTextView* mB10bPtr;
static ZKTextView* mB10aPtr;
static ZKTextView* mB9bPtr;
static ZKTextView* mB9aPtr;
static ZKTextView* mB8bPtr;
static ZKTextView* mB8aPtr;
static ZKTextView* mB7bPtr;
static ZKTextView* mB7aPtr;
static ZKTextView* mB6bPtr;
static ZKTextView* mB6aPtr;
static ZKTextView* mB5bPtr;
static ZKTextView* mB5aPtr;
static ZKTextView* mB4bPtr;
static ZKTextView* mB4aPtr;
static ZKTextView* mB3bPtr;
static ZKTextView* mB3aPtr;
static ZKTextView* mB2bPtr;
static ZKTextView* mB2aPtr;
static ZKTextView* mB1bPtr;
static ZKTextView* mB1aPtr;
static ZKTextView* mBarTitlePtr;
static ZKWindow* mCardBarPtr;
static ZKCircleBar* mGaugeBarPtr;
static ZKTextView* mGaugeValuePtr;
static ZKTextView* mGaugeTitlePtr;
static ZKWindow* mCardGaugePtr;
static ZKCircleBar* mRingBarPtr;
static ZKTextView* mRingValuePtr;
static ZKTextView* mRingTitlePtr;
static ZKWindow* mCardRingPtr;
static ZKDiagram* mWaveChartPtr;
static ZKTextView* mLineValuePtr;
static ZKTextView* mLineTitlePtr;
static ZKWindow* mCardLinePtr;
static ZKTextView* mImageView3Ptr;
static ZKTextView* mImageView2Ptr;
static ZKTextView* mImageView1Ptr;
static ZKTextView* mTitleSubPtr;
static ZKTextView* mTitleTextPtr;
static chartsActivity* mActivityPtr;

/*register activity*/
REGISTER_ACTIVITY(chartsActivity);

typedef struct {
	int id; // 定时器ID ， 不能重复
	int time; // 定时器  时间间隔  单位 毫秒
}S_ACTIVITY_TIMEER;

#include "logic/chartsLogic.cc"

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
    ID_CHARTS_NavBtn8, onButtonClick_NavBtn8,
    ID_CHARTS_NavBtn7, onButtonClick_NavBtn7,
    ID_CHARTS_NavBtn6, onButtonClick_NavBtn6,
    ID_CHARTS_NavBtn5, onButtonClick_NavBtn5,
    ID_CHARTS_NavBtn4, onButtonClick_NavBtn4,
    ID_CHARTS_NavBtn3, onButtonClick_NavBtn3,
    ID_CHARTS_NavBtn2, onButtonClick_NavBtn2,
    ID_CHARTS_NavBtn1, onButtonClick_NavBtn1,
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


chartsActivity::chartsActivity() {
	//todo add init code here
	mVideoLoopIndex = -1;
	mVideoLoopErrorCount = 0;
}

chartsActivity::~chartsActivity() {
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
    mB12bPtr = NULL;
    mB12aPtr = NULL;
    mB11bPtr = NULL;
    mB11aPtr = NULL;
    mB10bPtr = NULL;
    mB10aPtr = NULL;
    mB9bPtr = NULL;
    mB9aPtr = NULL;
    mB8bPtr = NULL;
    mB8aPtr = NULL;
    mB7bPtr = NULL;
    mB7aPtr = NULL;
    mB6bPtr = NULL;
    mB6aPtr = NULL;
    mB5bPtr = NULL;
    mB5aPtr = NULL;
    mB4bPtr = NULL;
    mB4aPtr = NULL;
    mB3bPtr = NULL;
    mB3aPtr = NULL;
    mB2bPtr = NULL;
    mB2aPtr = NULL;
    mB1bPtr = NULL;
    mB1aPtr = NULL;
    mBarTitlePtr = NULL;
    mCardBarPtr = NULL;
    mGaugeBarPtr = NULL;
    mGaugeValuePtr = NULL;
    mGaugeTitlePtr = NULL;
    mCardGaugePtr = NULL;
    mRingBarPtr = NULL;
    mRingValuePtr = NULL;
    mRingTitlePtr = NULL;
    mCardRingPtr = NULL;
    mWaveChartPtr = NULL;
    mLineValuePtr = NULL;
    mLineTitlePtr = NULL;
    mCardLinePtr = NULL;
    mImageView3Ptr = NULL;
    mImageView2Ptr = NULL;
    mImageView1Ptr = NULL;
    mTitleSubPtr = NULL;
    mTitleTextPtr = NULL;
}

const char* chartsActivity::getAppName() const{
	return "charts.ftu";
}

//TAG:onCreate
void chartsActivity::onCreate() {
	Activity::onCreate();
    mNavBtn8Ptr = (ZKButton*)findControlByID(ID_CHARTS_NavBtn8);
    mNavBtn7Ptr = (ZKButton*)findControlByID(ID_CHARTS_NavBtn7);
    mNavBtn6Ptr = (ZKButton*)findControlByID(ID_CHARTS_NavBtn6);
    mNavBtn5Ptr = (ZKButton*)findControlByID(ID_CHARTS_NavBtn5);
    mNavBtn4Ptr = (ZKButton*)findControlByID(ID_CHARTS_NavBtn4);
    mNavBtn3Ptr = (ZKButton*)findControlByID(ID_CHARTS_NavBtn3);
    mNavBtn2Ptr = (ZKButton*)findControlByID(ID_CHARTS_NavBtn2);
    mNavBtn1Ptr = (ZKButton*)findControlByID(ID_CHARTS_NavBtn1);
    mNavLabel8Ptr = (ZKTextView*)findControlByID(ID_CHARTS_NavLabel8);
    mNavIcon8Ptr = (ZKTextView*)findControlByID(ID_CHARTS_NavIcon8);
    mNavLabel7Ptr = (ZKTextView*)findControlByID(ID_CHARTS_NavLabel7);
    mNavIcon7Ptr = (ZKTextView*)findControlByID(ID_CHARTS_NavIcon7);
    mNavLabel6Ptr = (ZKTextView*)findControlByID(ID_CHARTS_NavLabel6);
    mNavIcon6Ptr = (ZKTextView*)findControlByID(ID_CHARTS_NavIcon6);
    mNavLabel5Ptr = (ZKTextView*)findControlByID(ID_CHARTS_NavLabel5);
    mNavIcon5Ptr = (ZKTextView*)findControlByID(ID_CHARTS_NavIcon5);
    mNavLabel4Ptr = (ZKTextView*)findControlByID(ID_CHARTS_NavLabel4);
    mNavIcon4Ptr = (ZKTextView*)findControlByID(ID_CHARTS_NavIcon4);
    mNavLabel3Ptr = (ZKTextView*)findControlByID(ID_CHARTS_NavLabel3);
    mNavIcon3Ptr = (ZKTextView*)findControlByID(ID_CHARTS_NavIcon3);
    mNavLabel2Ptr = (ZKTextView*)findControlByID(ID_CHARTS_NavLabel2);
    mNavIcon2Ptr = (ZKTextView*)findControlByID(ID_CHARTS_NavIcon2);
    mNavLabel1Ptr = (ZKTextView*)findControlByID(ID_CHARTS_NavLabel1);
    mNavIcon1Ptr = (ZKTextView*)findControlByID(ID_CHARTS_NavIcon1);
    mNavBarPtr = (ZKWindow*)findControlByID(ID_CHARTS_NavBar);
    mB12bPtr = (ZKTextView*)findControlByID(ID_CHARTS_B12b);
    mB12aPtr = (ZKTextView*)findControlByID(ID_CHARTS_B12a);
    mB11bPtr = (ZKTextView*)findControlByID(ID_CHARTS_B11b);
    mB11aPtr = (ZKTextView*)findControlByID(ID_CHARTS_B11a);
    mB10bPtr = (ZKTextView*)findControlByID(ID_CHARTS_B10b);
    mB10aPtr = (ZKTextView*)findControlByID(ID_CHARTS_B10a);
    mB9bPtr = (ZKTextView*)findControlByID(ID_CHARTS_B9b);
    mB9aPtr = (ZKTextView*)findControlByID(ID_CHARTS_B9a);
    mB8bPtr = (ZKTextView*)findControlByID(ID_CHARTS_B8b);
    mB8aPtr = (ZKTextView*)findControlByID(ID_CHARTS_B8a);
    mB7bPtr = (ZKTextView*)findControlByID(ID_CHARTS_B7b);
    mB7aPtr = (ZKTextView*)findControlByID(ID_CHARTS_B7a);
    mB6bPtr = (ZKTextView*)findControlByID(ID_CHARTS_B6b);
    mB6aPtr = (ZKTextView*)findControlByID(ID_CHARTS_B6a);
    mB5bPtr = (ZKTextView*)findControlByID(ID_CHARTS_B5b);
    mB5aPtr = (ZKTextView*)findControlByID(ID_CHARTS_B5a);
    mB4bPtr = (ZKTextView*)findControlByID(ID_CHARTS_B4b);
    mB4aPtr = (ZKTextView*)findControlByID(ID_CHARTS_B4a);
    mB3bPtr = (ZKTextView*)findControlByID(ID_CHARTS_B3b);
    mB3aPtr = (ZKTextView*)findControlByID(ID_CHARTS_B3a);
    mB2bPtr = (ZKTextView*)findControlByID(ID_CHARTS_B2b);
    mB2aPtr = (ZKTextView*)findControlByID(ID_CHARTS_B2a);
    mB1bPtr = (ZKTextView*)findControlByID(ID_CHARTS_B1b);
    mB1aPtr = (ZKTextView*)findControlByID(ID_CHARTS_B1a);
    mBarTitlePtr = (ZKTextView*)findControlByID(ID_CHARTS_BarTitle);
    mCardBarPtr = (ZKWindow*)findControlByID(ID_CHARTS_CardBar);
    mGaugeBarPtr = (ZKCircleBar*)findControlByID(ID_CHARTS_GaugeBar);
    mGaugeValuePtr = (ZKTextView*)findControlByID(ID_CHARTS_GaugeValue);
    mGaugeTitlePtr = (ZKTextView*)findControlByID(ID_CHARTS_GaugeTitle);
    mCardGaugePtr = (ZKWindow*)findControlByID(ID_CHARTS_CardGauge);
    mRingBarPtr = (ZKCircleBar*)findControlByID(ID_CHARTS_RingBar);
    mRingValuePtr = (ZKTextView*)findControlByID(ID_CHARTS_RingValue);
    mRingTitlePtr = (ZKTextView*)findControlByID(ID_CHARTS_RingTitle);
    mCardRingPtr = (ZKWindow*)findControlByID(ID_CHARTS_CardRing);
    mWaveChartPtr = (ZKDiagram*)findControlByID(ID_CHARTS_WaveChart);
    mLineValuePtr = (ZKTextView*)findControlByID(ID_CHARTS_LineValue);
    mLineTitlePtr = (ZKTextView*)findControlByID(ID_CHARTS_LineTitle);
    mCardLinePtr = (ZKWindow*)findControlByID(ID_CHARTS_CardLine);
    mImageView3Ptr = (ZKTextView*)findControlByID(ID_CHARTS_ImageView3);
    mImageView2Ptr = (ZKTextView*)findControlByID(ID_CHARTS_ImageView2);
    mImageView1Ptr = (ZKTextView*)findControlByID(ID_CHARTS_ImageView1);
    mTitleSubPtr = (ZKTextView*)findControlByID(ID_CHARTS_TitleSub);
    mTitleTextPtr = (ZKTextView*)findControlByID(ID_CHARTS_TitleText);
	mActivityPtr = this;
	onUI_init();
  registerProtocolDataUpdateListener(onProtocolDataUpdate);
  rigesterActivityTimer();
}

void chartsActivity::onClick(ZKBase *pBase) {
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

void chartsActivity::onResume() {
	Activity::onResume();
	EASYUICONTEXT->registerGlobalTouchListener(this);
	startVideoLoopPlayback();
	onUI_show();
}

void chartsActivity::onPause() {
	Activity::onPause();
	EASYUICONTEXT->unregisterGlobalTouchListener(this);
	stopVideoLoopPlayback();
	onUI_hide();
}

void chartsActivity::onIntent(const Intent *intentPtr) {
	Activity::onIntent(intentPtr);
	onUI_intent(intentPtr);
}

bool chartsActivity::onTimer(int id) {
	return onUI_Timer(id);
}

void chartsActivity::onProgressChanged(ZKSeekBar *pSeekBar, int progress){

    int seekBarTablen = sizeof(SZKSeekBarCallbackTab) / sizeof(S_ZKSeekBarCallback);
    for (int i = 0; i < seekBarTablen; ++i) {
        if (SZKSeekBarCallbackTab[i].id == pSeekBar->getID()) {
            SZKSeekBarCallbackTab[i].callback(pSeekBar, progress);
            break;
        }
    }
}

int chartsActivity::getListItemCount(const ZKListView *pListView) const{
    int tablen = sizeof(SListViewFunctionsCallbackTab) / sizeof(S_ListViewFunctionsCallback);
    for (int i = 0; i < tablen; ++i) {
        if (SListViewFunctionsCallbackTab[i].id == pListView->getID()) {
            return SListViewFunctionsCallbackTab[i].getListItemCountCallback(pListView);
            break;
        }
    }
    return 0;
}

void chartsActivity::obtainListItemData(ZKListView *pListView,ZKListView::ZKListItem *pListItem, int index){
    int tablen = sizeof(SListViewFunctionsCallbackTab) / sizeof(S_ListViewFunctionsCallback);
    for (int i = 0; i < tablen; ++i) {
        if (SListViewFunctionsCallbackTab[i].id == pListView->getID()) {
            SListViewFunctionsCallbackTab[i].obtainListItemDataCallback(pListView, pListItem, index);
            break;
        }
    }
}

void chartsActivity::onItemClick(ZKListView *pListView, int index, int id){
    int tablen = sizeof(SListViewFunctionsCallbackTab) / sizeof(S_ListViewFunctionsCallback);
    for (int i = 0; i < tablen; ++i) {
        if (SListViewFunctionsCallbackTab[i].id == pListView->getID()) {
            SListViewFunctionsCallbackTab[i].onItemClickCallback(pListView, index, id);
            break;
        }
    }
}

void chartsActivity::onSlideItemClick(ZKSlideWindow *pSlideWindow, int index) {
    int tablen = sizeof(SSlideWindowItemClickCallbackTab) / sizeof(S_SlideWindowItemClickCallback);
    for (int i = 0; i < tablen; ++i) {
        if (SSlideWindowItemClickCallbackTab[i].id == pSlideWindow->getID()) {
            SSlideWindowItemClickCallbackTab[i].onSlideItemClickCallback(pSlideWindow, index);
            break;
        }
    }
}

bool chartsActivity::onTouchEvent(const MotionEvent &ev) {
    return onchartsActivityTouchEvent(ev);
}

void chartsActivity::onTextChanged(ZKTextView *pTextView, const std::string &text) {
    int tablen = sizeof(SEditTextInputCallbackTab) / sizeof(S_EditTextInputCallback);
    for (int i = 0; i < tablen; ++i) {
        if (SEditTextInputCallbackTab[i].id == pTextView->getID()) {
            SEditTextInputCallbackTab[i].onEditTextChangedCallback(text);
            break;
        }
    }
}

void chartsActivity::rigesterActivityTimer() {
    int tablen = sizeof(REGISTER_ACTIVITY_TIMER_TAB) / sizeof(S_ACTIVITY_TIMEER);
    for (int i = 0; i < tablen; ++i) {
        S_ACTIVITY_TIMEER temp = REGISTER_ACTIVITY_TIMER_TAB[i];
        registerTimer(temp.id, temp.time);
    }
}


void chartsActivity::onVideoPlayerMessage(ZKVideoView *pVideoView, int msg) {
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

void chartsActivity::videoLoopPlayback(ZKVideoView *pVideoView, int msg, size_t callbackTabIndex) {

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

void chartsActivity::startVideoLoopPlayback() {
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

void chartsActivity::stopVideoLoopPlayback() {
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

bool chartsActivity::parseVideoFileList(const char *pFileListPath, std::vector<string>& mediaFileList) {
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

int chartsActivity::removeCharFromString(string& nString, char c) {
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

void chartsActivity::registerUserTimer(int id, int time) {
	registerTimer(id, time);
}

void chartsActivity::unregisterUserTimer(int id) {
	unregisterTimer(id);
}

void chartsActivity::resetUserTimer(int id, int time) {
	resetTimer(id, time);
}