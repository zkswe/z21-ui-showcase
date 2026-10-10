/***********************************************
/gen auto by zuitools
***********************************************/
#include "settingsActivity.h"

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
static ZKButton* mBtnResetPtr;
static ZKButton* mBtnSavePtr;
static ZKTextView* mDiag6VPtr;
static ZKTextView* mDiag6KPtr;
static ZKTextView* mDiag5VPtr;
static ZKTextView* mDiag5KPtr;
static ZKTextView* mDiag4VPtr;
static ZKTextView* mDiag4KPtr;
static ZKTextView* mDiag3VPtr;
static ZKTextView* mDiag3KPtr;
static ZKTextView* mDiag2VPtr;
static ZKTextView* mDiag2KPtr;
static ZKTextView* mDiag1VPtr;
static ZKTextView* mDiag1KPtr;
static ZKWindow* mDiagCardPtr;
static ZKTextView* mLb2096Ptr;
static ZKTextView* mTg4KnobPtr;
static ZKTextView* mTg4Ptr;
static ZKTextView* mTg4NamePtr;
static ZKTextView* mTg3KnobPtr;
static ZKTextView* mTg3Ptr;
static ZKTextView* mTg3NamePtr;
static ZKTextView* mTg2KnobPtr;
static ZKTextView* mTg2Ptr;
static ZKTextView* mTg2NamePtr;
static ZKTextView* mTg1KnobPtr;
static ZKTextView* mTg1Ptr;
static ZKTextView* mTg1NamePtr;
static ZKTextView* mLb2990Ptr;
static ZKButton* mSegLightPtr;
static ZKButton* mSegDarkPtr;
static ZKTextView* mSegTrackPtr;
static ZKTextView* mLb2896Ptr;
static ZKTextView* mImageView3Ptr;
static ZKTextView* mImageView2Ptr;
static ZKTextView* mImageView1Ptr;
static ZKTextView* mTitleSubPtr;
static ZKTextView* mTitleTextPtr;
static settingsActivity* mActivityPtr;

/*register activity*/
REGISTER_ACTIVITY(settingsActivity);

typedef struct {
	int id; // 定时器ID ， 不能重复
	int time; // 定时器  时间间隔  单位 毫秒
}S_ACTIVITY_TIMEER;

#include "logic/settingsLogic.cc"

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
    ID_SETTINGS_NavBtn8, onButtonClick_NavBtn8,
    ID_SETTINGS_NavBtn7, onButtonClick_NavBtn7,
    ID_SETTINGS_NavBtn6, onButtonClick_NavBtn6,
    ID_SETTINGS_NavBtn5, onButtonClick_NavBtn5,
    ID_SETTINGS_NavBtn4, onButtonClick_NavBtn4,
    ID_SETTINGS_NavBtn3, onButtonClick_NavBtn3,
    ID_SETTINGS_NavBtn2, onButtonClick_NavBtn2,
    ID_SETTINGS_NavBtn1, onButtonClick_NavBtn1,
    ID_SETTINGS_BtnReset, onButtonClick_BtnReset,
    ID_SETTINGS_BtnSave, onButtonClick_BtnSave,
    ID_SETTINGS_SegLight, onButtonClick_SegLight,
    ID_SETTINGS_SegDark, onButtonClick_SegDark,
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


settingsActivity::settingsActivity() {
	//todo add init code here
	mVideoLoopIndex = -1;
	mVideoLoopErrorCount = 0;
}

settingsActivity::~settingsActivity() {
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
    mBtnResetPtr = NULL;
    mBtnSavePtr = NULL;
    mDiag6VPtr = NULL;
    mDiag6KPtr = NULL;
    mDiag5VPtr = NULL;
    mDiag5KPtr = NULL;
    mDiag4VPtr = NULL;
    mDiag4KPtr = NULL;
    mDiag3VPtr = NULL;
    mDiag3KPtr = NULL;
    mDiag2VPtr = NULL;
    mDiag2KPtr = NULL;
    mDiag1VPtr = NULL;
    mDiag1KPtr = NULL;
    mDiagCardPtr = NULL;
    mLb2096Ptr = NULL;
    mTg4KnobPtr = NULL;
    mTg4Ptr = NULL;
    mTg4NamePtr = NULL;
    mTg3KnobPtr = NULL;
    mTg3Ptr = NULL;
    mTg3NamePtr = NULL;
    mTg2KnobPtr = NULL;
    mTg2Ptr = NULL;
    mTg2NamePtr = NULL;
    mTg1KnobPtr = NULL;
    mTg1Ptr = NULL;
    mTg1NamePtr = NULL;
    mLb2990Ptr = NULL;
    mSegLightPtr = NULL;
    mSegDarkPtr = NULL;
    mSegTrackPtr = NULL;
    mLb2896Ptr = NULL;
    mImageView3Ptr = NULL;
    mImageView2Ptr = NULL;
    mImageView1Ptr = NULL;
    mTitleSubPtr = NULL;
    mTitleTextPtr = NULL;
}

const char* settingsActivity::getAppName() const{
	return "settings.ftu";
}

//TAG:onCreate
void settingsActivity::onCreate() {
	Activity::onCreate();
    mNavBtn8Ptr = (ZKButton*)findControlByID(ID_SETTINGS_NavBtn8);
    mNavBtn7Ptr = (ZKButton*)findControlByID(ID_SETTINGS_NavBtn7);
    mNavBtn6Ptr = (ZKButton*)findControlByID(ID_SETTINGS_NavBtn6);
    mNavBtn5Ptr = (ZKButton*)findControlByID(ID_SETTINGS_NavBtn5);
    mNavBtn4Ptr = (ZKButton*)findControlByID(ID_SETTINGS_NavBtn4);
    mNavBtn3Ptr = (ZKButton*)findControlByID(ID_SETTINGS_NavBtn3);
    mNavBtn2Ptr = (ZKButton*)findControlByID(ID_SETTINGS_NavBtn2);
    mNavBtn1Ptr = (ZKButton*)findControlByID(ID_SETTINGS_NavBtn1);
    mNavLabel8Ptr = (ZKTextView*)findControlByID(ID_SETTINGS_NavLabel8);
    mNavIcon8Ptr = (ZKTextView*)findControlByID(ID_SETTINGS_NavIcon8);
    mNavLabel7Ptr = (ZKTextView*)findControlByID(ID_SETTINGS_NavLabel7);
    mNavIcon7Ptr = (ZKTextView*)findControlByID(ID_SETTINGS_NavIcon7);
    mNavLabel6Ptr = (ZKTextView*)findControlByID(ID_SETTINGS_NavLabel6);
    mNavIcon6Ptr = (ZKTextView*)findControlByID(ID_SETTINGS_NavIcon6);
    mNavLabel5Ptr = (ZKTextView*)findControlByID(ID_SETTINGS_NavLabel5);
    mNavIcon5Ptr = (ZKTextView*)findControlByID(ID_SETTINGS_NavIcon5);
    mNavLabel4Ptr = (ZKTextView*)findControlByID(ID_SETTINGS_NavLabel4);
    mNavIcon4Ptr = (ZKTextView*)findControlByID(ID_SETTINGS_NavIcon4);
    mNavLabel3Ptr = (ZKTextView*)findControlByID(ID_SETTINGS_NavLabel3);
    mNavIcon3Ptr = (ZKTextView*)findControlByID(ID_SETTINGS_NavIcon3);
    mNavLabel2Ptr = (ZKTextView*)findControlByID(ID_SETTINGS_NavLabel2);
    mNavIcon2Ptr = (ZKTextView*)findControlByID(ID_SETTINGS_NavIcon2);
    mNavLabel1Ptr = (ZKTextView*)findControlByID(ID_SETTINGS_NavLabel1);
    mNavIcon1Ptr = (ZKTextView*)findControlByID(ID_SETTINGS_NavIcon1);
    mNavBarPtr = (ZKWindow*)findControlByID(ID_SETTINGS_NavBar);
    mBtnResetPtr = (ZKButton*)findControlByID(ID_SETTINGS_BtnReset);
    mBtnSavePtr = (ZKButton*)findControlByID(ID_SETTINGS_BtnSave);
    mDiag6VPtr = (ZKTextView*)findControlByID(ID_SETTINGS_Diag6V);
    mDiag6KPtr = (ZKTextView*)findControlByID(ID_SETTINGS_Diag6K);
    mDiag5VPtr = (ZKTextView*)findControlByID(ID_SETTINGS_Diag5V);
    mDiag5KPtr = (ZKTextView*)findControlByID(ID_SETTINGS_Diag5K);
    mDiag4VPtr = (ZKTextView*)findControlByID(ID_SETTINGS_Diag4V);
    mDiag4KPtr = (ZKTextView*)findControlByID(ID_SETTINGS_Diag4K);
    mDiag3VPtr = (ZKTextView*)findControlByID(ID_SETTINGS_Diag3V);
    mDiag3KPtr = (ZKTextView*)findControlByID(ID_SETTINGS_Diag3K);
    mDiag2VPtr = (ZKTextView*)findControlByID(ID_SETTINGS_Diag2V);
    mDiag2KPtr = (ZKTextView*)findControlByID(ID_SETTINGS_Diag2K);
    mDiag1VPtr = (ZKTextView*)findControlByID(ID_SETTINGS_Diag1V);
    mDiag1KPtr = (ZKTextView*)findControlByID(ID_SETTINGS_Diag1K);
    mDiagCardPtr = (ZKWindow*)findControlByID(ID_SETTINGS_DiagCard);
    mLb2096Ptr = (ZKTextView*)findControlByID(ID_SETTINGS_Lb2096);
    mTg4KnobPtr = (ZKTextView*)findControlByID(ID_SETTINGS_Tg4Knob);
    mTg4Ptr = (ZKTextView*)findControlByID(ID_SETTINGS_Tg4);
    mTg4NamePtr = (ZKTextView*)findControlByID(ID_SETTINGS_Tg4Name);
    mTg3KnobPtr = (ZKTextView*)findControlByID(ID_SETTINGS_Tg3Knob);
    mTg3Ptr = (ZKTextView*)findControlByID(ID_SETTINGS_Tg3);
    mTg3NamePtr = (ZKTextView*)findControlByID(ID_SETTINGS_Tg3Name);
    mTg2KnobPtr = (ZKTextView*)findControlByID(ID_SETTINGS_Tg2Knob);
    mTg2Ptr = (ZKTextView*)findControlByID(ID_SETTINGS_Tg2);
    mTg2NamePtr = (ZKTextView*)findControlByID(ID_SETTINGS_Tg2Name);
    mTg1KnobPtr = (ZKTextView*)findControlByID(ID_SETTINGS_Tg1Knob);
    mTg1Ptr = (ZKTextView*)findControlByID(ID_SETTINGS_Tg1);
    mTg1NamePtr = (ZKTextView*)findControlByID(ID_SETTINGS_Tg1Name);
    mLb2990Ptr = (ZKTextView*)findControlByID(ID_SETTINGS_Lb2990);
    mSegLightPtr = (ZKButton*)findControlByID(ID_SETTINGS_SegLight);
    mSegDarkPtr = (ZKButton*)findControlByID(ID_SETTINGS_SegDark);
    mSegTrackPtr = (ZKTextView*)findControlByID(ID_SETTINGS_SegTrack);
    mLb2896Ptr = (ZKTextView*)findControlByID(ID_SETTINGS_Lb2896);
    mImageView3Ptr = (ZKTextView*)findControlByID(ID_SETTINGS_ImageView3);
    mImageView2Ptr = (ZKTextView*)findControlByID(ID_SETTINGS_ImageView2);
    mImageView1Ptr = (ZKTextView*)findControlByID(ID_SETTINGS_ImageView1);
    mTitleSubPtr = (ZKTextView*)findControlByID(ID_SETTINGS_TitleSub);
    mTitleTextPtr = (ZKTextView*)findControlByID(ID_SETTINGS_TitleText);
	mActivityPtr = this;
	onUI_init();
  registerProtocolDataUpdateListener(onProtocolDataUpdate);
  rigesterActivityTimer();
}

void settingsActivity::onClick(ZKBase *pBase) {
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

void settingsActivity::onResume() {
	Activity::onResume();
	EASYUICONTEXT->registerGlobalTouchListener(this);
	startVideoLoopPlayback();
	onUI_show();
}

void settingsActivity::onPause() {
	Activity::onPause();
	EASYUICONTEXT->unregisterGlobalTouchListener(this);
	stopVideoLoopPlayback();
	onUI_hide();
}

void settingsActivity::onIntent(const Intent *intentPtr) {
	Activity::onIntent(intentPtr);
	onUI_intent(intentPtr);
}

bool settingsActivity::onTimer(int id) {
	return onUI_Timer(id);
}

void settingsActivity::onProgressChanged(ZKSeekBar *pSeekBar, int progress){

    int seekBarTablen = sizeof(SZKSeekBarCallbackTab) / sizeof(S_ZKSeekBarCallback);
    for (int i = 0; i < seekBarTablen; ++i) {
        if (SZKSeekBarCallbackTab[i].id == pSeekBar->getID()) {
            SZKSeekBarCallbackTab[i].callback(pSeekBar, progress);
            break;
        }
    }
}

int settingsActivity::getListItemCount(const ZKListView *pListView) const{
    int tablen = sizeof(SListViewFunctionsCallbackTab) / sizeof(S_ListViewFunctionsCallback);
    for (int i = 0; i < tablen; ++i) {
        if (SListViewFunctionsCallbackTab[i].id == pListView->getID()) {
            return SListViewFunctionsCallbackTab[i].getListItemCountCallback(pListView);
            break;
        }
    }
    return 0;
}

void settingsActivity::obtainListItemData(ZKListView *pListView,ZKListView::ZKListItem *pListItem, int index){
    int tablen = sizeof(SListViewFunctionsCallbackTab) / sizeof(S_ListViewFunctionsCallback);
    for (int i = 0; i < tablen; ++i) {
        if (SListViewFunctionsCallbackTab[i].id == pListView->getID()) {
            SListViewFunctionsCallbackTab[i].obtainListItemDataCallback(pListView, pListItem, index);
            break;
        }
    }
}

void settingsActivity::onItemClick(ZKListView *pListView, int index, int id){
    int tablen = sizeof(SListViewFunctionsCallbackTab) / sizeof(S_ListViewFunctionsCallback);
    for (int i = 0; i < tablen; ++i) {
        if (SListViewFunctionsCallbackTab[i].id == pListView->getID()) {
            SListViewFunctionsCallbackTab[i].onItemClickCallback(pListView, index, id);
            break;
        }
    }
}

void settingsActivity::onSlideItemClick(ZKSlideWindow *pSlideWindow, int index) {
    int tablen = sizeof(SSlideWindowItemClickCallbackTab) / sizeof(S_SlideWindowItemClickCallback);
    for (int i = 0; i < tablen; ++i) {
        if (SSlideWindowItemClickCallbackTab[i].id == pSlideWindow->getID()) {
            SSlideWindowItemClickCallbackTab[i].onSlideItemClickCallback(pSlideWindow, index);
            break;
        }
    }
}

bool settingsActivity::onTouchEvent(const MotionEvent &ev) {
    return onsettingsActivityTouchEvent(ev);
}

void settingsActivity::onTextChanged(ZKTextView *pTextView, const std::string &text) {
    int tablen = sizeof(SEditTextInputCallbackTab) / sizeof(S_EditTextInputCallback);
    for (int i = 0; i < tablen; ++i) {
        if (SEditTextInputCallbackTab[i].id == pTextView->getID()) {
            SEditTextInputCallbackTab[i].onEditTextChangedCallback(text);
            break;
        }
    }
}

void settingsActivity::rigesterActivityTimer() {
    int tablen = sizeof(REGISTER_ACTIVITY_TIMER_TAB) / sizeof(S_ACTIVITY_TIMEER);
    for (int i = 0; i < tablen; ++i) {
        S_ACTIVITY_TIMEER temp = REGISTER_ACTIVITY_TIMER_TAB[i];
        registerTimer(temp.id, temp.time);
    }
}


void settingsActivity::onVideoPlayerMessage(ZKVideoView *pVideoView, int msg) {
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

void settingsActivity::videoLoopPlayback(ZKVideoView *pVideoView, int msg, size_t callbackTabIndex) {

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

void settingsActivity::startVideoLoopPlayback() {
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

void settingsActivity::stopVideoLoopPlayback() {
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

bool settingsActivity::parseVideoFileList(const char *pFileListPath, std::vector<string>& mediaFileList) {
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

int settingsActivity::removeCharFromString(string& nString, char c) {
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

void settingsActivity::registerUserTimer(int id, int time) {
	registerTimer(id, time);
}

void settingsActivity::unregisterUserTimer(int id) {
	unregisterTimer(id);
}

void settingsActivity::resetUserTimer(int id, int time) {
	resetTimer(id, time);
}