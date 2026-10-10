/***********************************************
/gen auto by zuitools
***********************************************/
#include "imageActivity.h"

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
static ZKWindow* mCrop3Ptr;
static ZKWindow* mCrop2Ptr;
static ZKWindow* mCrop1Ptr;
static ZKTextView* mCropLbPtr;
static ZKTextView* mToolTipPtr;
static ZKButton* mBtnSavePtr;
static ZKButton* mBtnClearPtr;
static ZKButton* mBtnUndoPtr;
static ZKButton* mW3Ptr;
static ZKPainter* mW3RadPtr;
static ZKButton* mW2Ptr;
static ZKPainter* mW2RadPtr;
static ZKButton* mW1Ptr;
static ZKPainter* mW1RadPtr;
static ZKTextView* mToolWidthLbPtr;
static ZKTextView* mSw5Ptr;
static ZKTextView* mSw4Ptr;
static ZKTextView* mSw3Ptr;
static ZKTextView* mSw2Ptr;
static ZKTextView* mSw1Ptr;
static ZKTextView* mToolColorLbPtr;
static ZKTextView* mToolTitlePtr;
static ZKWindow* mToolCardPtr;
static ZKTextView* mCanvasHintPtr;
static ZKPainter* mCanvasPtr;
static ZKWindow* mCanvasCardPtr;
static ZKTextView* mImageView3Ptr;
static ZKTextView* mImageView2Ptr;
static ZKTextView* mImageView1Ptr;
static ZKTextView* mTitleSubPtr;
static ZKTextView* mTitleTextPtr;
static imageActivity* mActivityPtr;

/*register activity*/
REGISTER_ACTIVITY(imageActivity);

typedef struct {
	int id; // 定时器ID ， 不能重复
	int time; // 定时器  时间间隔  单位 毫秒
}S_ACTIVITY_TIMEER;

#include "logic/imageLogic.cc"

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
    ID_IMAGE_NavBtn8, onButtonClick_NavBtn8,
    ID_IMAGE_NavBtn7, onButtonClick_NavBtn7,
    ID_IMAGE_NavBtn6, onButtonClick_NavBtn6,
    ID_IMAGE_NavBtn5, onButtonClick_NavBtn5,
    ID_IMAGE_NavBtn4, onButtonClick_NavBtn4,
    ID_IMAGE_NavBtn3, onButtonClick_NavBtn3,
    ID_IMAGE_NavBtn2, onButtonClick_NavBtn2,
    ID_IMAGE_NavBtn1, onButtonClick_NavBtn1,
    ID_IMAGE_BtnSave, onButtonClick_BtnSave,
    ID_IMAGE_BtnClear, onButtonClick_BtnClear,
    ID_IMAGE_BtnUndo, onButtonClick_BtnUndo,
    ID_IMAGE_W3, onButtonClick_W3,
    ID_IMAGE_W2, onButtonClick_W2,
    ID_IMAGE_W1, onButtonClick_W1,
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


imageActivity::imageActivity() {
	//todo add init code here
	mVideoLoopIndex = -1;
	mVideoLoopErrorCount = 0;
}

imageActivity::~imageActivity() {
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
    mCrop3Ptr = NULL;
    mCrop2Ptr = NULL;
    mCrop1Ptr = NULL;
    mCropLbPtr = NULL;
    mToolTipPtr = NULL;
    mBtnSavePtr = NULL;
    mBtnClearPtr = NULL;
    mBtnUndoPtr = NULL;
    mW3Ptr = NULL;
    mW3RadPtr = NULL;
    mW2Ptr = NULL;
    mW2RadPtr = NULL;
    mW1Ptr = NULL;
    mW1RadPtr = NULL;
    mToolWidthLbPtr = NULL;
    mSw5Ptr = NULL;
    mSw4Ptr = NULL;
    mSw3Ptr = NULL;
    mSw2Ptr = NULL;
    mSw1Ptr = NULL;
    mToolColorLbPtr = NULL;
    mToolTitlePtr = NULL;
    mToolCardPtr = NULL;
    mCanvasHintPtr = NULL;
    mCanvasPtr = NULL;
    mCanvasCardPtr = NULL;
    mImageView3Ptr = NULL;
    mImageView2Ptr = NULL;
    mImageView1Ptr = NULL;
    mTitleSubPtr = NULL;
    mTitleTextPtr = NULL;
}

const char* imageActivity::getAppName() const{
	return "image.ftu";
}

//TAG:onCreate
void imageActivity::onCreate() {
	Activity::onCreate();
    mNavBtn8Ptr = (ZKButton*)findControlByID(ID_IMAGE_NavBtn8);
    mNavBtn7Ptr = (ZKButton*)findControlByID(ID_IMAGE_NavBtn7);
    mNavBtn6Ptr = (ZKButton*)findControlByID(ID_IMAGE_NavBtn6);
    mNavBtn5Ptr = (ZKButton*)findControlByID(ID_IMAGE_NavBtn5);
    mNavBtn4Ptr = (ZKButton*)findControlByID(ID_IMAGE_NavBtn4);
    mNavBtn3Ptr = (ZKButton*)findControlByID(ID_IMAGE_NavBtn3);
    mNavBtn2Ptr = (ZKButton*)findControlByID(ID_IMAGE_NavBtn2);
    mNavBtn1Ptr = (ZKButton*)findControlByID(ID_IMAGE_NavBtn1);
    mNavLabel8Ptr = (ZKTextView*)findControlByID(ID_IMAGE_NavLabel8);
    mNavIcon8Ptr = (ZKTextView*)findControlByID(ID_IMAGE_NavIcon8);
    mNavLabel7Ptr = (ZKTextView*)findControlByID(ID_IMAGE_NavLabel7);
    mNavIcon7Ptr = (ZKTextView*)findControlByID(ID_IMAGE_NavIcon7);
    mNavLabel6Ptr = (ZKTextView*)findControlByID(ID_IMAGE_NavLabel6);
    mNavIcon6Ptr = (ZKTextView*)findControlByID(ID_IMAGE_NavIcon6);
    mNavLabel5Ptr = (ZKTextView*)findControlByID(ID_IMAGE_NavLabel5);
    mNavIcon5Ptr = (ZKTextView*)findControlByID(ID_IMAGE_NavIcon5);
    mNavLabel4Ptr = (ZKTextView*)findControlByID(ID_IMAGE_NavLabel4);
    mNavIcon4Ptr = (ZKTextView*)findControlByID(ID_IMAGE_NavIcon4);
    mNavLabel3Ptr = (ZKTextView*)findControlByID(ID_IMAGE_NavLabel3);
    mNavIcon3Ptr = (ZKTextView*)findControlByID(ID_IMAGE_NavIcon3);
    mNavLabel2Ptr = (ZKTextView*)findControlByID(ID_IMAGE_NavLabel2);
    mNavIcon2Ptr = (ZKTextView*)findControlByID(ID_IMAGE_NavIcon2);
    mNavLabel1Ptr = (ZKTextView*)findControlByID(ID_IMAGE_NavLabel1);
    mNavIcon1Ptr = (ZKTextView*)findControlByID(ID_IMAGE_NavIcon1);
    mNavBarPtr = (ZKWindow*)findControlByID(ID_IMAGE_NavBar);
    mCrop3Ptr = (ZKWindow*)findControlByID(ID_IMAGE_Crop3);
    mCrop2Ptr = (ZKWindow*)findControlByID(ID_IMAGE_Crop2);
    mCrop1Ptr = (ZKWindow*)findControlByID(ID_IMAGE_Crop1);
    mCropLbPtr = (ZKTextView*)findControlByID(ID_IMAGE_CropLb);
    mToolTipPtr = (ZKTextView*)findControlByID(ID_IMAGE_ToolTip);
    mBtnSavePtr = (ZKButton*)findControlByID(ID_IMAGE_BtnSave);
    mBtnClearPtr = (ZKButton*)findControlByID(ID_IMAGE_BtnClear);
    mBtnUndoPtr = (ZKButton*)findControlByID(ID_IMAGE_BtnUndo);
    mW3Ptr = (ZKButton*)findControlByID(ID_IMAGE_W3);
    mW3RadPtr = (ZKPainter*)findControlByID(ID_IMAGE_W3Rad);
    mW2Ptr = (ZKButton*)findControlByID(ID_IMAGE_W2);
    mW2RadPtr = (ZKPainter*)findControlByID(ID_IMAGE_W2Rad);
    mW1Ptr = (ZKButton*)findControlByID(ID_IMAGE_W1);
    mW1RadPtr = (ZKPainter*)findControlByID(ID_IMAGE_W1Rad);
    mToolWidthLbPtr = (ZKTextView*)findControlByID(ID_IMAGE_ToolWidthLb);
    mSw5Ptr = (ZKTextView*)findControlByID(ID_IMAGE_Sw5);
    mSw4Ptr = (ZKTextView*)findControlByID(ID_IMAGE_Sw4);
    mSw3Ptr = (ZKTextView*)findControlByID(ID_IMAGE_Sw3);
    mSw2Ptr = (ZKTextView*)findControlByID(ID_IMAGE_Sw2);
    mSw1Ptr = (ZKTextView*)findControlByID(ID_IMAGE_Sw1);
    mToolColorLbPtr = (ZKTextView*)findControlByID(ID_IMAGE_ToolColorLb);
    mToolTitlePtr = (ZKTextView*)findControlByID(ID_IMAGE_ToolTitle);
    mToolCardPtr = (ZKWindow*)findControlByID(ID_IMAGE_ToolCard);
    mCanvasHintPtr = (ZKTextView*)findControlByID(ID_IMAGE_CanvasHint);
    mCanvasPtr = (ZKPainter*)findControlByID(ID_IMAGE_Canvas);
    mCanvasCardPtr = (ZKWindow*)findControlByID(ID_IMAGE_CanvasCard);
    mImageView3Ptr = (ZKTextView*)findControlByID(ID_IMAGE_ImageView3);
    mImageView2Ptr = (ZKTextView*)findControlByID(ID_IMAGE_ImageView2);
    mImageView1Ptr = (ZKTextView*)findControlByID(ID_IMAGE_ImageView1);
    mTitleSubPtr = (ZKTextView*)findControlByID(ID_IMAGE_TitleSub);
    mTitleTextPtr = (ZKTextView*)findControlByID(ID_IMAGE_TitleText);
	mActivityPtr = this;
	onUI_init();
  registerProtocolDataUpdateListener(onProtocolDataUpdate);
  rigesterActivityTimer();
}

void imageActivity::onClick(ZKBase *pBase) {
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

void imageActivity::onResume() {
	Activity::onResume();
	EASYUICONTEXT->registerGlobalTouchListener(this);
	startVideoLoopPlayback();
	onUI_show();
}

void imageActivity::onPause() {
	Activity::onPause();
	EASYUICONTEXT->unregisterGlobalTouchListener(this);
	stopVideoLoopPlayback();
	onUI_hide();
}

void imageActivity::onIntent(const Intent *intentPtr) {
	Activity::onIntent(intentPtr);
	onUI_intent(intentPtr);
}

bool imageActivity::onTimer(int id) {
	return onUI_Timer(id);
}

void imageActivity::onProgressChanged(ZKSeekBar *pSeekBar, int progress){

    int seekBarTablen = sizeof(SZKSeekBarCallbackTab) / sizeof(S_ZKSeekBarCallback);
    for (int i = 0; i < seekBarTablen; ++i) {
        if (SZKSeekBarCallbackTab[i].id == pSeekBar->getID()) {
            SZKSeekBarCallbackTab[i].callback(pSeekBar, progress);
            break;
        }
    }
}

int imageActivity::getListItemCount(const ZKListView *pListView) const{
    int tablen = sizeof(SListViewFunctionsCallbackTab) / sizeof(S_ListViewFunctionsCallback);
    for (int i = 0; i < tablen; ++i) {
        if (SListViewFunctionsCallbackTab[i].id == pListView->getID()) {
            return SListViewFunctionsCallbackTab[i].getListItemCountCallback(pListView);
            break;
        }
    }
    return 0;
}

void imageActivity::obtainListItemData(ZKListView *pListView,ZKListView::ZKListItem *pListItem, int index){
    int tablen = sizeof(SListViewFunctionsCallbackTab) / sizeof(S_ListViewFunctionsCallback);
    for (int i = 0; i < tablen; ++i) {
        if (SListViewFunctionsCallbackTab[i].id == pListView->getID()) {
            SListViewFunctionsCallbackTab[i].obtainListItemDataCallback(pListView, pListItem, index);
            break;
        }
    }
}

void imageActivity::onItemClick(ZKListView *pListView, int index, int id){
    int tablen = sizeof(SListViewFunctionsCallbackTab) / sizeof(S_ListViewFunctionsCallback);
    for (int i = 0; i < tablen; ++i) {
        if (SListViewFunctionsCallbackTab[i].id == pListView->getID()) {
            SListViewFunctionsCallbackTab[i].onItemClickCallback(pListView, index, id);
            break;
        }
    }
}

void imageActivity::onSlideItemClick(ZKSlideWindow *pSlideWindow, int index) {
    int tablen = sizeof(SSlideWindowItemClickCallbackTab) / sizeof(S_SlideWindowItemClickCallback);
    for (int i = 0; i < tablen; ++i) {
        if (SSlideWindowItemClickCallbackTab[i].id == pSlideWindow->getID()) {
            SSlideWindowItemClickCallbackTab[i].onSlideItemClickCallback(pSlideWindow, index);
            break;
        }
    }
}

bool imageActivity::onTouchEvent(const MotionEvent &ev) {
    return onimageActivityTouchEvent(ev);
}

void imageActivity::onTextChanged(ZKTextView *pTextView, const std::string &text) {
    int tablen = sizeof(SEditTextInputCallbackTab) / sizeof(S_EditTextInputCallback);
    for (int i = 0; i < tablen; ++i) {
        if (SEditTextInputCallbackTab[i].id == pTextView->getID()) {
            SEditTextInputCallbackTab[i].onEditTextChangedCallback(text);
            break;
        }
    }
}

void imageActivity::rigesterActivityTimer() {
    int tablen = sizeof(REGISTER_ACTIVITY_TIMER_TAB) / sizeof(S_ACTIVITY_TIMEER);
    for (int i = 0; i < tablen; ++i) {
        S_ACTIVITY_TIMEER temp = REGISTER_ACTIVITY_TIMER_TAB[i];
        registerTimer(temp.id, temp.time);
    }
}


void imageActivity::onVideoPlayerMessage(ZKVideoView *pVideoView, int msg) {
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

void imageActivity::videoLoopPlayback(ZKVideoView *pVideoView, int msg, size_t callbackTabIndex) {

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

void imageActivity::startVideoLoopPlayback() {
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

void imageActivity::stopVideoLoopPlayback() {
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

bool imageActivity::parseVideoFileList(const char *pFileListPath, std::vector<string>& mediaFileList) {
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

int imageActivity::removeCharFromString(string& nString, char c) {
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

void imageActivity::registerUserTimer(int id, int time) {
	registerTimer(id, time);
}

void imageActivity::unregisterUserTimer(int id) {
	unregisterTimer(id);
}

void imageActivity::resetUserTimer(int id, int time) {
	resetTimer(id, time);
}