/***********************************************
/gen auto by zuitools
***********************************************/
#include "widgetsActivity.h"

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
static ZKTextView* mImageView6Ptr;
static ZKSeekBar* mPanelBarPtr;
static ZKTextView* mPanelBodyPtr;
static ZKTextView* mPanelTitlePtr;
static ZKWindow* mCardPanelPtr;
static ZKTextView* mBadgeNumPtr;
static ZKTextView* mBadgeDotPtr;
static ZKTextView* mImageView5Ptr;
static ZKWindow* mBadgeHostPtr;
static ZKTextView* mToastTxtPtr;
static ZKTextView* mImageView4Ptr;
static ZKWindow* mToastPtr;
static ZKTextView* mLb3166Ptr;
static ZKButton* mStepPlusPtr;
static ZKTextView* mStepValPtr;
static ZKButton* mStepMinusPtr;
static ZKTextView* mSkValPtr;
static ZKSeekBar* mSkDemoPtr;
static ZKTextView* mLb3084Ptr;
static ZKTextView* mRd1TxtPtr;
static ZKTextView* mRd1DotPtr;
static ZKTextView* mRd1Ptr;
static ZKTextView* mCbOffTxtPtr;
static ZKTextView* mCbOffPtr;
static ZKTextView* mCbOnTxtPtr;
static ZKTextView* mCbOnMarkPtr;
static ZKTextView* mCbOnPtr;
static ZKTextView* mSwOffKnobPtr;
static ZKTextView* mSwOffPtr;
static ZKTextView* mSwOnKnobPtr;
static ZKTextView* mSwOnPtr;
static ZKTextView* mLb2990Ptr;
static ZKButton* mBtnDisabledPtr;
static ZKButton* mBtnIconPtr;
static ZKButton* mBtnGhostPtr;
static ZKButton* mBtnOutlinePtr;
static ZKButton* mBtnPrimaryPtr;
static ZKTextView* mLb2896Ptr;
static ZKTextView* mImageView3Ptr;
static ZKTextView* mImageView2Ptr;
static ZKTextView* mImageView1Ptr;
static ZKTextView* mTitleSubPtr;
static ZKTextView* mTitleTextPtr;
static widgetsActivity* mActivityPtr;

/*register activity*/
REGISTER_ACTIVITY(widgetsActivity);

typedef struct {
	int id; // 定时器ID ， 不能重复
	int time; // 定时器  时间间隔  单位 毫秒
}S_ACTIVITY_TIMEER;

#include "logic/widgetsLogic.cc"

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
    ID_WIDGETS_NavBtn8, onButtonClick_NavBtn8,
    ID_WIDGETS_NavBtn7, onButtonClick_NavBtn7,
    ID_WIDGETS_NavBtn6, onButtonClick_NavBtn6,
    ID_WIDGETS_NavBtn5, onButtonClick_NavBtn5,
    ID_WIDGETS_NavBtn4, onButtonClick_NavBtn4,
    ID_WIDGETS_NavBtn3, onButtonClick_NavBtn3,
    ID_WIDGETS_NavBtn2, onButtonClick_NavBtn2,
    ID_WIDGETS_NavBtn1, onButtonClick_NavBtn1,
    ID_WIDGETS_StepPlus, onButtonClick_StepPlus,
    ID_WIDGETS_StepMinus, onButtonClick_StepMinus,
    ID_WIDGETS_BtnDisabled, onButtonClick_BtnDisabled,
    ID_WIDGETS_BtnIcon, onButtonClick_BtnIcon,
    ID_WIDGETS_BtnGhost, onButtonClick_BtnGhost,
    ID_WIDGETS_BtnOutline, onButtonClick_BtnOutline,
    ID_WIDGETS_BtnPrimary, onButtonClick_BtnPrimary,
};
/***************/


typedef void (*SeekBarCallback)(ZKSeekBar *pSeekBar, int progress);
typedef struct {
    int id;
    SeekBarCallback callback;
}S_ZKSeekBarCallback;
/*TAG:SeekBarCallbackTab*/
static S_ZKSeekBarCallback SZKSeekBarCallbackTab[] = {
    ID_WIDGETS_PanelBar, onProgressChanged_PanelBar,
    ID_WIDGETS_SkDemo, onProgressChanged_SkDemo,
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


widgetsActivity::widgetsActivity() {
	//todo add init code here
	mVideoLoopIndex = -1;
	mVideoLoopErrorCount = 0;
}

widgetsActivity::~widgetsActivity() {
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
    mImageView6Ptr = NULL;
    mPanelBarPtr = NULL;
    mPanelBodyPtr = NULL;
    mPanelTitlePtr = NULL;
    mCardPanelPtr = NULL;
    mBadgeNumPtr = NULL;
    mBadgeDotPtr = NULL;
    mImageView5Ptr = NULL;
    mBadgeHostPtr = NULL;
    mToastTxtPtr = NULL;
    mImageView4Ptr = NULL;
    mToastPtr = NULL;
    mLb3166Ptr = NULL;
    mStepPlusPtr = NULL;
    mStepValPtr = NULL;
    mStepMinusPtr = NULL;
    mSkValPtr = NULL;
    mSkDemoPtr = NULL;
    mLb3084Ptr = NULL;
    mRd1TxtPtr = NULL;
    mRd1DotPtr = NULL;
    mRd1Ptr = NULL;
    mCbOffTxtPtr = NULL;
    mCbOffPtr = NULL;
    mCbOnTxtPtr = NULL;
    mCbOnMarkPtr = NULL;
    mCbOnPtr = NULL;
    mSwOffKnobPtr = NULL;
    mSwOffPtr = NULL;
    mSwOnKnobPtr = NULL;
    mSwOnPtr = NULL;
    mLb2990Ptr = NULL;
    mBtnDisabledPtr = NULL;
    mBtnIconPtr = NULL;
    mBtnGhostPtr = NULL;
    mBtnOutlinePtr = NULL;
    mBtnPrimaryPtr = NULL;
    mLb2896Ptr = NULL;
    mImageView3Ptr = NULL;
    mImageView2Ptr = NULL;
    mImageView1Ptr = NULL;
    mTitleSubPtr = NULL;
    mTitleTextPtr = NULL;
}

const char* widgetsActivity::getAppName() const{
	return "widgets.ftu";
}

//TAG:onCreate
void widgetsActivity::onCreate() {
	Activity::onCreate();
    mNavBtn8Ptr = (ZKButton*)findControlByID(ID_WIDGETS_NavBtn8);
    mNavBtn7Ptr = (ZKButton*)findControlByID(ID_WIDGETS_NavBtn7);
    mNavBtn6Ptr = (ZKButton*)findControlByID(ID_WIDGETS_NavBtn6);
    mNavBtn5Ptr = (ZKButton*)findControlByID(ID_WIDGETS_NavBtn5);
    mNavBtn4Ptr = (ZKButton*)findControlByID(ID_WIDGETS_NavBtn4);
    mNavBtn3Ptr = (ZKButton*)findControlByID(ID_WIDGETS_NavBtn3);
    mNavBtn2Ptr = (ZKButton*)findControlByID(ID_WIDGETS_NavBtn2);
    mNavBtn1Ptr = (ZKButton*)findControlByID(ID_WIDGETS_NavBtn1);
    mNavLabel8Ptr = (ZKTextView*)findControlByID(ID_WIDGETS_NavLabel8);
    mNavIcon8Ptr = (ZKTextView*)findControlByID(ID_WIDGETS_NavIcon8);
    mNavLabel7Ptr = (ZKTextView*)findControlByID(ID_WIDGETS_NavLabel7);
    mNavIcon7Ptr = (ZKTextView*)findControlByID(ID_WIDGETS_NavIcon7);
    mNavLabel6Ptr = (ZKTextView*)findControlByID(ID_WIDGETS_NavLabel6);
    mNavIcon6Ptr = (ZKTextView*)findControlByID(ID_WIDGETS_NavIcon6);
    mNavLabel5Ptr = (ZKTextView*)findControlByID(ID_WIDGETS_NavLabel5);
    mNavIcon5Ptr = (ZKTextView*)findControlByID(ID_WIDGETS_NavIcon5);
    mNavLabel4Ptr = (ZKTextView*)findControlByID(ID_WIDGETS_NavLabel4);
    mNavIcon4Ptr = (ZKTextView*)findControlByID(ID_WIDGETS_NavIcon4);
    mNavLabel3Ptr = (ZKTextView*)findControlByID(ID_WIDGETS_NavLabel3);
    mNavIcon3Ptr = (ZKTextView*)findControlByID(ID_WIDGETS_NavIcon3);
    mNavLabel2Ptr = (ZKTextView*)findControlByID(ID_WIDGETS_NavLabel2);
    mNavIcon2Ptr = (ZKTextView*)findControlByID(ID_WIDGETS_NavIcon2);
    mNavLabel1Ptr = (ZKTextView*)findControlByID(ID_WIDGETS_NavLabel1);
    mNavIcon1Ptr = (ZKTextView*)findControlByID(ID_WIDGETS_NavIcon1);
    mNavBarPtr = (ZKWindow*)findControlByID(ID_WIDGETS_NavBar);
    mImageView6Ptr = (ZKTextView*)findControlByID(ID_WIDGETS_ImageView6);
    mPanelBarPtr = (ZKSeekBar*)findControlByID(ID_WIDGETS_PanelBar);if(mPanelBarPtr!= NULL){mPanelBarPtr->setSeekBarChangeListener(this);}
    mPanelBodyPtr = (ZKTextView*)findControlByID(ID_WIDGETS_PanelBody);
    mPanelTitlePtr = (ZKTextView*)findControlByID(ID_WIDGETS_PanelTitle);
    mCardPanelPtr = (ZKWindow*)findControlByID(ID_WIDGETS_CardPanel);
    mBadgeNumPtr = (ZKTextView*)findControlByID(ID_WIDGETS_BadgeNum);
    mBadgeDotPtr = (ZKTextView*)findControlByID(ID_WIDGETS_BadgeDot);
    mImageView5Ptr = (ZKTextView*)findControlByID(ID_WIDGETS_ImageView5);
    mBadgeHostPtr = (ZKWindow*)findControlByID(ID_WIDGETS_BadgeHost);
    mToastTxtPtr = (ZKTextView*)findControlByID(ID_WIDGETS_ToastTxt);
    mImageView4Ptr = (ZKTextView*)findControlByID(ID_WIDGETS_ImageView4);
    mToastPtr = (ZKWindow*)findControlByID(ID_WIDGETS_Toast);
    mLb3166Ptr = (ZKTextView*)findControlByID(ID_WIDGETS_Lb3166);
    mStepPlusPtr = (ZKButton*)findControlByID(ID_WIDGETS_StepPlus);
    mStepValPtr = (ZKTextView*)findControlByID(ID_WIDGETS_StepVal);
    mStepMinusPtr = (ZKButton*)findControlByID(ID_WIDGETS_StepMinus);
    mSkValPtr = (ZKTextView*)findControlByID(ID_WIDGETS_SkVal);
    mSkDemoPtr = (ZKSeekBar*)findControlByID(ID_WIDGETS_SkDemo);if(mSkDemoPtr!= NULL){mSkDemoPtr->setSeekBarChangeListener(this);}
    mLb3084Ptr = (ZKTextView*)findControlByID(ID_WIDGETS_Lb3084);
    mRd1TxtPtr = (ZKTextView*)findControlByID(ID_WIDGETS_Rd1Txt);
    mRd1DotPtr = (ZKTextView*)findControlByID(ID_WIDGETS_Rd1Dot);
    mRd1Ptr = (ZKTextView*)findControlByID(ID_WIDGETS_Rd1);
    mCbOffTxtPtr = (ZKTextView*)findControlByID(ID_WIDGETS_CbOffTxt);
    mCbOffPtr = (ZKTextView*)findControlByID(ID_WIDGETS_CbOff);
    mCbOnTxtPtr = (ZKTextView*)findControlByID(ID_WIDGETS_CbOnTxt);
    mCbOnMarkPtr = (ZKTextView*)findControlByID(ID_WIDGETS_CbOnMark);
    mCbOnPtr = (ZKTextView*)findControlByID(ID_WIDGETS_CbOn);
    mSwOffKnobPtr = (ZKTextView*)findControlByID(ID_WIDGETS_SwOffKnob);
    mSwOffPtr = (ZKTextView*)findControlByID(ID_WIDGETS_SwOff);
    mSwOnKnobPtr = (ZKTextView*)findControlByID(ID_WIDGETS_SwOnKnob);
    mSwOnPtr = (ZKTextView*)findControlByID(ID_WIDGETS_SwOn);
    mLb2990Ptr = (ZKTextView*)findControlByID(ID_WIDGETS_Lb2990);
    mBtnDisabledPtr = (ZKButton*)findControlByID(ID_WIDGETS_BtnDisabled);
    mBtnIconPtr = (ZKButton*)findControlByID(ID_WIDGETS_BtnIcon);
    mBtnGhostPtr = (ZKButton*)findControlByID(ID_WIDGETS_BtnGhost);
    mBtnOutlinePtr = (ZKButton*)findControlByID(ID_WIDGETS_BtnOutline);
    mBtnPrimaryPtr = (ZKButton*)findControlByID(ID_WIDGETS_BtnPrimary);
    mLb2896Ptr = (ZKTextView*)findControlByID(ID_WIDGETS_Lb2896);
    mImageView3Ptr = (ZKTextView*)findControlByID(ID_WIDGETS_ImageView3);
    mImageView2Ptr = (ZKTextView*)findControlByID(ID_WIDGETS_ImageView2);
    mImageView1Ptr = (ZKTextView*)findControlByID(ID_WIDGETS_ImageView1);
    mTitleSubPtr = (ZKTextView*)findControlByID(ID_WIDGETS_TitleSub);
    mTitleTextPtr = (ZKTextView*)findControlByID(ID_WIDGETS_TitleText);
	mActivityPtr = this;
	onUI_init();
  registerProtocolDataUpdateListener(onProtocolDataUpdate);
  rigesterActivityTimer();
}

void widgetsActivity::onClick(ZKBase *pBase) {
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

void widgetsActivity::onResume() {
	Activity::onResume();
	EASYUICONTEXT->registerGlobalTouchListener(this);
	startVideoLoopPlayback();
	onUI_show();
}

void widgetsActivity::onPause() {
	Activity::onPause();
	EASYUICONTEXT->unregisterGlobalTouchListener(this);
	stopVideoLoopPlayback();
	onUI_hide();
}

void widgetsActivity::onIntent(const Intent *intentPtr) {
	Activity::onIntent(intentPtr);
	onUI_intent(intentPtr);
}

bool widgetsActivity::onTimer(int id) {
	return onUI_Timer(id);
}

void widgetsActivity::onProgressChanged(ZKSeekBar *pSeekBar, int progress){

    int seekBarTablen = sizeof(SZKSeekBarCallbackTab) / sizeof(S_ZKSeekBarCallback);
    for (int i = 0; i < seekBarTablen; ++i) {
        if (SZKSeekBarCallbackTab[i].id == pSeekBar->getID()) {
            SZKSeekBarCallbackTab[i].callback(pSeekBar, progress);
            break;
        }
    }
}

int widgetsActivity::getListItemCount(const ZKListView *pListView) const{
    int tablen = sizeof(SListViewFunctionsCallbackTab) / sizeof(S_ListViewFunctionsCallback);
    for (int i = 0; i < tablen; ++i) {
        if (SListViewFunctionsCallbackTab[i].id == pListView->getID()) {
            return SListViewFunctionsCallbackTab[i].getListItemCountCallback(pListView);
            break;
        }
    }
    return 0;
}

void widgetsActivity::obtainListItemData(ZKListView *pListView,ZKListView::ZKListItem *pListItem, int index){
    int tablen = sizeof(SListViewFunctionsCallbackTab) / sizeof(S_ListViewFunctionsCallback);
    for (int i = 0; i < tablen; ++i) {
        if (SListViewFunctionsCallbackTab[i].id == pListView->getID()) {
            SListViewFunctionsCallbackTab[i].obtainListItemDataCallback(pListView, pListItem, index);
            break;
        }
    }
}

void widgetsActivity::onItemClick(ZKListView *pListView, int index, int id){
    int tablen = sizeof(SListViewFunctionsCallbackTab) / sizeof(S_ListViewFunctionsCallback);
    for (int i = 0; i < tablen; ++i) {
        if (SListViewFunctionsCallbackTab[i].id == pListView->getID()) {
            SListViewFunctionsCallbackTab[i].onItemClickCallback(pListView, index, id);
            break;
        }
    }
}

void widgetsActivity::onSlideItemClick(ZKSlideWindow *pSlideWindow, int index) {
    int tablen = sizeof(SSlideWindowItemClickCallbackTab) / sizeof(S_SlideWindowItemClickCallback);
    for (int i = 0; i < tablen; ++i) {
        if (SSlideWindowItemClickCallbackTab[i].id == pSlideWindow->getID()) {
            SSlideWindowItemClickCallbackTab[i].onSlideItemClickCallback(pSlideWindow, index);
            break;
        }
    }
}

bool widgetsActivity::onTouchEvent(const MotionEvent &ev) {
    return onwidgetsActivityTouchEvent(ev);
}

void widgetsActivity::onTextChanged(ZKTextView *pTextView, const std::string &text) {
    int tablen = sizeof(SEditTextInputCallbackTab) / sizeof(S_EditTextInputCallback);
    for (int i = 0; i < tablen; ++i) {
        if (SEditTextInputCallbackTab[i].id == pTextView->getID()) {
            SEditTextInputCallbackTab[i].onEditTextChangedCallback(text);
            break;
        }
    }
}

void widgetsActivity::rigesterActivityTimer() {
    int tablen = sizeof(REGISTER_ACTIVITY_TIMER_TAB) / sizeof(S_ACTIVITY_TIMEER);
    for (int i = 0; i < tablen; ++i) {
        S_ACTIVITY_TIMEER temp = REGISTER_ACTIVITY_TIMER_TAB[i];
        registerTimer(temp.id, temp.time);
    }
}


void widgetsActivity::onVideoPlayerMessage(ZKVideoView *pVideoView, int msg) {
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

void widgetsActivity::videoLoopPlayback(ZKVideoView *pVideoView, int msg, size_t callbackTabIndex) {

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

void widgetsActivity::startVideoLoopPlayback() {
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

void widgetsActivity::stopVideoLoopPlayback() {
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

bool widgetsActivity::parseVideoFileList(const char *pFileListPath, std::vector<string>& mediaFileList) {
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

int widgetsActivity::removeCharFromString(string& nString, char c) {
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

void widgetsActivity::registerUserTimer(int id, int time) {
	registerTimer(id, time);
}

void widgetsActivity::unregisterUserTimer(int id) {
	unregisterTimer(id);
}

void widgetsActivity::resetUserTimer(int id, int time) {
	resetTimer(id, time);
}