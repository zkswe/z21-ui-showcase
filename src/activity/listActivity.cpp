/***********************************************
/gen auto by zuitools
***********************************************/
#include "listActivity.h"

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
static ZKTextView* mCov6TxtPtr;
static ZKWindow* mCov6Ptr;
static ZKTextView* mCov5TxtPtr;
static ZKWindow* mCov5Ptr;
static ZKTextView* mCov4TxtPtr;
static ZKWindow* mCov4Ptr;
static ZKTextView* mCov3TxtPtr;
static ZKWindow* mCov3Ptr;
static ZKTextView* mCov2TxtPtr;
static ZKWindow* mCov2Ptr;
static ZKTextView* mCov1TxtPtr;
static ZKWindow* mCov1Ptr;
static ZKTextView* mGridTitlePtr;
static ZKWindow* mGridCardPtr;
static ZKTextView* mImageView11Ptr;
static ZKTextView* mRow4SubPtr;
static ZKTextView* mRow4NamePtr;
static ZKTextView* mImageView10Ptr;
static ZKWindow* mRow4CoverPtr;
static ZKWindow* mRow4Ptr;
static ZKTextView* mImageView9Ptr;
static ZKTextView* mRow3SubPtr;
static ZKTextView* mRow3NamePtr;
static ZKTextView* mImageView8Ptr;
static ZKWindow* mRow3CoverPtr;
static ZKWindow* mRow3Ptr;
static ZKTextView* mImageView7Ptr;
static ZKTextView* mRow2SubPtr;
static ZKTextView* mRow2NamePtr;
static ZKTextView* mImageView6Ptr;
static ZKWindow* mRow2CoverPtr;
static ZKWindow* mRow2Ptr;
static ZKTextView* mImageView5Ptr;
static ZKTextView* mRow1SubPtr;
static ZKTextView* mRow1NamePtr;
static ZKTextView* mImageView4Ptr;
static ZKWindow* mRow1CoverPtr;
static ZKWindow* mRow1Ptr;
static ZKTextView* mListTitlePtr;
static ZKWindow* mListCardPtr;
static ZKTextView* mImageView3Ptr;
static ZKTextView* mImageView2Ptr;
static ZKTextView* mImageView1Ptr;
static ZKTextView* mTitleSubPtr;
static ZKTextView* mTitleTextPtr;
static listActivity* mActivityPtr;

/*register activity*/
REGISTER_ACTIVITY(listActivity);

typedef struct {
	int id; // 定时器ID ， 不能重复
	int time; // 定时器  时间间隔  单位 毫秒
}S_ACTIVITY_TIMEER;

#include "logic/listLogic.cc"

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
    ID_LIST_NavBtn8, onButtonClick_NavBtn8,
    ID_LIST_NavBtn7, onButtonClick_NavBtn7,
    ID_LIST_NavBtn6, onButtonClick_NavBtn6,
    ID_LIST_NavBtn5, onButtonClick_NavBtn5,
    ID_LIST_NavBtn4, onButtonClick_NavBtn4,
    ID_LIST_NavBtn3, onButtonClick_NavBtn3,
    ID_LIST_NavBtn2, onButtonClick_NavBtn2,
    ID_LIST_NavBtn1, onButtonClick_NavBtn1,
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


listActivity::listActivity() {
	//todo add init code here
	mVideoLoopIndex = -1;
	mVideoLoopErrorCount = 0;
}

listActivity::~listActivity() {
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
    mCov6TxtPtr = NULL;
    mCov6Ptr = NULL;
    mCov5TxtPtr = NULL;
    mCov5Ptr = NULL;
    mCov4TxtPtr = NULL;
    mCov4Ptr = NULL;
    mCov3TxtPtr = NULL;
    mCov3Ptr = NULL;
    mCov2TxtPtr = NULL;
    mCov2Ptr = NULL;
    mCov1TxtPtr = NULL;
    mCov1Ptr = NULL;
    mGridTitlePtr = NULL;
    mGridCardPtr = NULL;
    mImageView11Ptr = NULL;
    mRow4SubPtr = NULL;
    mRow4NamePtr = NULL;
    mImageView10Ptr = NULL;
    mRow4CoverPtr = NULL;
    mRow4Ptr = NULL;
    mImageView9Ptr = NULL;
    mRow3SubPtr = NULL;
    mRow3NamePtr = NULL;
    mImageView8Ptr = NULL;
    mRow3CoverPtr = NULL;
    mRow3Ptr = NULL;
    mImageView7Ptr = NULL;
    mRow2SubPtr = NULL;
    mRow2NamePtr = NULL;
    mImageView6Ptr = NULL;
    mRow2CoverPtr = NULL;
    mRow2Ptr = NULL;
    mImageView5Ptr = NULL;
    mRow1SubPtr = NULL;
    mRow1NamePtr = NULL;
    mImageView4Ptr = NULL;
    mRow1CoverPtr = NULL;
    mRow1Ptr = NULL;
    mListTitlePtr = NULL;
    mListCardPtr = NULL;
    mImageView3Ptr = NULL;
    mImageView2Ptr = NULL;
    mImageView1Ptr = NULL;
    mTitleSubPtr = NULL;
    mTitleTextPtr = NULL;
}

const char* listActivity::getAppName() const{
	return "list.ftu";
}

//TAG:onCreate
void listActivity::onCreate() {
	Activity::onCreate();
    mNavBtn8Ptr = (ZKButton*)findControlByID(ID_LIST_NavBtn8);
    mNavBtn7Ptr = (ZKButton*)findControlByID(ID_LIST_NavBtn7);
    mNavBtn6Ptr = (ZKButton*)findControlByID(ID_LIST_NavBtn6);
    mNavBtn5Ptr = (ZKButton*)findControlByID(ID_LIST_NavBtn5);
    mNavBtn4Ptr = (ZKButton*)findControlByID(ID_LIST_NavBtn4);
    mNavBtn3Ptr = (ZKButton*)findControlByID(ID_LIST_NavBtn3);
    mNavBtn2Ptr = (ZKButton*)findControlByID(ID_LIST_NavBtn2);
    mNavBtn1Ptr = (ZKButton*)findControlByID(ID_LIST_NavBtn1);
    mNavLabel8Ptr = (ZKTextView*)findControlByID(ID_LIST_NavLabel8);
    mNavIcon8Ptr = (ZKTextView*)findControlByID(ID_LIST_NavIcon8);
    mNavLabel7Ptr = (ZKTextView*)findControlByID(ID_LIST_NavLabel7);
    mNavIcon7Ptr = (ZKTextView*)findControlByID(ID_LIST_NavIcon7);
    mNavLabel6Ptr = (ZKTextView*)findControlByID(ID_LIST_NavLabel6);
    mNavIcon6Ptr = (ZKTextView*)findControlByID(ID_LIST_NavIcon6);
    mNavLabel5Ptr = (ZKTextView*)findControlByID(ID_LIST_NavLabel5);
    mNavIcon5Ptr = (ZKTextView*)findControlByID(ID_LIST_NavIcon5);
    mNavLabel4Ptr = (ZKTextView*)findControlByID(ID_LIST_NavLabel4);
    mNavIcon4Ptr = (ZKTextView*)findControlByID(ID_LIST_NavIcon4);
    mNavLabel3Ptr = (ZKTextView*)findControlByID(ID_LIST_NavLabel3);
    mNavIcon3Ptr = (ZKTextView*)findControlByID(ID_LIST_NavIcon3);
    mNavLabel2Ptr = (ZKTextView*)findControlByID(ID_LIST_NavLabel2);
    mNavIcon2Ptr = (ZKTextView*)findControlByID(ID_LIST_NavIcon2);
    mNavLabel1Ptr = (ZKTextView*)findControlByID(ID_LIST_NavLabel1);
    mNavIcon1Ptr = (ZKTextView*)findControlByID(ID_LIST_NavIcon1);
    mNavBarPtr = (ZKWindow*)findControlByID(ID_LIST_NavBar);
    mCov6TxtPtr = (ZKTextView*)findControlByID(ID_LIST_Cov6Txt);
    mCov6Ptr = (ZKWindow*)findControlByID(ID_LIST_Cov6);
    mCov5TxtPtr = (ZKTextView*)findControlByID(ID_LIST_Cov5Txt);
    mCov5Ptr = (ZKWindow*)findControlByID(ID_LIST_Cov5);
    mCov4TxtPtr = (ZKTextView*)findControlByID(ID_LIST_Cov4Txt);
    mCov4Ptr = (ZKWindow*)findControlByID(ID_LIST_Cov4);
    mCov3TxtPtr = (ZKTextView*)findControlByID(ID_LIST_Cov3Txt);
    mCov3Ptr = (ZKWindow*)findControlByID(ID_LIST_Cov3);
    mCov2TxtPtr = (ZKTextView*)findControlByID(ID_LIST_Cov2Txt);
    mCov2Ptr = (ZKWindow*)findControlByID(ID_LIST_Cov2);
    mCov1TxtPtr = (ZKTextView*)findControlByID(ID_LIST_Cov1Txt);
    mCov1Ptr = (ZKWindow*)findControlByID(ID_LIST_Cov1);
    mGridTitlePtr = (ZKTextView*)findControlByID(ID_LIST_GridTitle);
    mGridCardPtr = (ZKWindow*)findControlByID(ID_LIST_GridCard);
    mImageView11Ptr = (ZKTextView*)findControlByID(ID_LIST_ImageView11);
    mRow4SubPtr = (ZKTextView*)findControlByID(ID_LIST_Row4Sub);
    mRow4NamePtr = (ZKTextView*)findControlByID(ID_LIST_Row4Name);
    mImageView10Ptr = (ZKTextView*)findControlByID(ID_LIST_ImageView10);
    mRow4CoverPtr = (ZKWindow*)findControlByID(ID_LIST_Row4Cover);
    mRow4Ptr = (ZKWindow*)findControlByID(ID_LIST_Row4);
    mImageView9Ptr = (ZKTextView*)findControlByID(ID_LIST_ImageView9);
    mRow3SubPtr = (ZKTextView*)findControlByID(ID_LIST_Row3Sub);
    mRow3NamePtr = (ZKTextView*)findControlByID(ID_LIST_Row3Name);
    mImageView8Ptr = (ZKTextView*)findControlByID(ID_LIST_ImageView8);
    mRow3CoverPtr = (ZKWindow*)findControlByID(ID_LIST_Row3Cover);
    mRow3Ptr = (ZKWindow*)findControlByID(ID_LIST_Row3);
    mImageView7Ptr = (ZKTextView*)findControlByID(ID_LIST_ImageView7);
    mRow2SubPtr = (ZKTextView*)findControlByID(ID_LIST_Row2Sub);
    mRow2NamePtr = (ZKTextView*)findControlByID(ID_LIST_Row2Name);
    mImageView6Ptr = (ZKTextView*)findControlByID(ID_LIST_ImageView6);
    mRow2CoverPtr = (ZKWindow*)findControlByID(ID_LIST_Row2Cover);
    mRow2Ptr = (ZKWindow*)findControlByID(ID_LIST_Row2);
    mImageView5Ptr = (ZKTextView*)findControlByID(ID_LIST_ImageView5);
    mRow1SubPtr = (ZKTextView*)findControlByID(ID_LIST_Row1Sub);
    mRow1NamePtr = (ZKTextView*)findControlByID(ID_LIST_Row1Name);
    mImageView4Ptr = (ZKTextView*)findControlByID(ID_LIST_ImageView4);
    mRow1CoverPtr = (ZKWindow*)findControlByID(ID_LIST_Row1Cover);
    mRow1Ptr = (ZKWindow*)findControlByID(ID_LIST_Row1);
    mListTitlePtr = (ZKTextView*)findControlByID(ID_LIST_ListTitle);
    mListCardPtr = (ZKWindow*)findControlByID(ID_LIST_ListCard);
    mImageView3Ptr = (ZKTextView*)findControlByID(ID_LIST_ImageView3);
    mImageView2Ptr = (ZKTextView*)findControlByID(ID_LIST_ImageView2);
    mImageView1Ptr = (ZKTextView*)findControlByID(ID_LIST_ImageView1);
    mTitleSubPtr = (ZKTextView*)findControlByID(ID_LIST_TitleSub);
    mTitleTextPtr = (ZKTextView*)findControlByID(ID_LIST_TitleText);
	mActivityPtr = this;
	onUI_init();
  registerProtocolDataUpdateListener(onProtocolDataUpdate);
  rigesterActivityTimer();
}

void listActivity::onClick(ZKBase *pBase) {
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

void listActivity::onResume() {
	Activity::onResume();
	EASYUICONTEXT->registerGlobalTouchListener(this);
	startVideoLoopPlayback();
	onUI_show();
}

void listActivity::onPause() {
	Activity::onPause();
	EASYUICONTEXT->unregisterGlobalTouchListener(this);
	stopVideoLoopPlayback();
	onUI_hide();
}

void listActivity::onIntent(const Intent *intentPtr) {
	Activity::onIntent(intentPtr);
	onUI_intent(intentPtr);
}

bool listActivity::onTimer(int id) {
	return onUI_Timer(id);
}

void listActivity::onProgressChanged(ZKSeekBar *pSeekBar, int progress){

    int seekBarTablen = sizeof(SZKSeekBarCallbackTab) / sizeof(S_ZKSeekBarCallback);
    for (int i = 0; i < seekBarTablen; ++i) {
        if (SZKSeekBarCallbackTab[i].id == pSeekBar->getID()) {
            SZKSeekBarCallbackTab[i].callback(pSeekBar, progress);
            break;
        }
    }
}

int listActivity::getListItemCount(const ZKListView *pListView) const{
    int tablen = sizeof(SListViewFunctionsCallbackTab) / sizeof(S_ListViewFunctionsCallback);
    for (int i = 0; i < tablen; ++i) {
        if (SListViewFunctionsCallbackTab[i].id == pListView->getID()) {
            return SListViewFunctionsCallbackTab[i].getListItemCountCallback(pListView);
            break;
        }
    }
    return 0;
}

void listActivity::obtainListItemData(ZKListView *pListView,ZKListView::ZKListItem *pListItem, int index){
    int tablen = sizeof(SListViewFunctionsCallbackTab) / sizeof(S_ListViewFunctionsCallback);
    for (int i = 0; i < tablen; ++i) {
        if (SListViewFunctionsCallbackTab[i].id == pListView->getID()) {
            SListViewFunctionsCallbackTab[i].obtainListItemDataCallback(pListView, pListItem, index);
            break;
        }
    }
}

void listActivity::onItemClick(ZKListView *pListView, int index, int id){
    int tablen = sizeof(SListViewFunctionsCallbackTab) / sizeof(S_ListViewFunctionsCallback);
    for (int i = 0; i < tablen; ++i) {
        if (SListViewFunctionsCallbackTab[i].id == pListView->getID()) {
            SListViewFunctionsCallbackTab[i].onItemClickCallback(pListView, index, id);
            break;
        }
    }
}

void listActivity::onSlideItemClick(ZKSlideWindow *pSlideWindow, int index) {
    int tablen = sizeof(SSlideWindowItemClickCallbackTab) / sizeof(S_SlideWindowItemClickCallback);
    for (int i = 0; i < tablen; ++i) {
        if (SSlideWindowItemClickCallbackTab[i].id == pSlideWindow->getID()) {
            SSlideWindowItemClickCallbackTab[i].onSlideItemClickCallback(pSlideWindow, index);
            break;
        }
    }
}

bool listActivity::onTouchEvent(const MotionEvent &ev) {
    return onlistActivityTouchEvent(ev);
}

void listActivity::onTextChanged(ZKTextView *pTextView, const std::string &text) {
    int tablen = sizeof(SEditTextInputCallbackTab) / sizeof(S_EditTextInputCallback);
    for (int i = 0; i < tablen; ++i) {
        if (SEditTextInputCallbackTab[i].id == pTextView->getID()) {
            SEditTextInputCallbackTab[i].onEditTextChangedCallback(text);
            break;
        }
    }
}

void listActivity::rigesterActivityTimer() {
    int tablen = sizeof(REGISTER_ACTIVITY_TIMER_TAB) / sizeof(S_ACTIVITY_TIMEER);
    for (int i = 0; i < tablen; ++i) {
        S_ACTIVITY_TIMEER temp = REGISTER_ACTIVITY_TIMER_TAB[i];
        registerTimer(temp.id, temp.time);
    }
}


void listActivity::onVideoPlayerMessage(ZKVideoView *pVideoView, int msg) {
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

void listActivity::videoLoopPlayback(ZKVideoView *pVideoView, int msg, size_t callbackTabIndex) {

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

void listActivity::startVideoLoopPlayback() {
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

void listActivity::stopVideoLoopPlayback() {
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

bool listActivity::parseVideoFileList(const char *pFileListPath, std::vector<string>& mediaFileList) {
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

int listActivity::removeCharFromString(string& nString, char c) {
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

void listActivity::registerUserTimer(int id, int time) {
	registerTimer(id, time);
}

void listActivity::unregisterUserTimer(int id) {
	unregisterTimer(id);
}

void listActivity::resetUserTimer(int id, int time) {
	resetTimer(id, time);
}