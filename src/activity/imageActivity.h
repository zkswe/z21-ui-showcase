/***********************************************
/gen auto by zuitools
***********************************************/
#ifndef __IMAGEACTIVITY_H__
#define __IMAGEACTIVITY_H__


#include "app/Activity.h"
#include "entry/EasyUIContext.h"

#include "uart/ProtocolData.h"
#include "uart/ProtocolParser.h"

#include "utils/Log.h"
#include "control/ZKDigitalClock.h"
#include "control/ZKPainter.h"
#include "control/ZKButton.h"
#include "control/ZKCircleBar.h"
#include "control/ZKDiagram.h"
#include "control/ZKListView.h"
#include "control/ZKPointer.h"
#include "control/ZKQRCode.h"
#include "control/ZKTextView.h"
#include "control/ZKSeekBar.h"
#include "control/ZKEditText.h"
#include "control/ZKVideoView.h"
#include "window/ZKSlideWindow.h"

/*TAG:Macro宏ID*/
#define ID_IMAGE_NavBtn8    110138
#define ID_IMAGE_NavBtn7    110137
#define ID_IMAGE_NavBtn6    110136
#define ID_IMAGE_NavBtn5    110135
#define ID_IMAGE_NavBtn4    110134
#define ID_IMAGE_NavBtn3    110133
#define ID_IMAGE_NavBtn2    110132
#define ID_IMAGE_NavBtn1    110131
#define ID_IMAGE_NavLabel8    110031
#define ID_IMAGE_NavIcon8    110030
#define ID_IMAGE_NavLabel7    110029
#define ID_IMAGE_NavIcon7    110028
#define ID_IMAGE_NavLabel6    110027
#define ID_IMAGE_NavIcon6    110026
#define ID_IMAGE_NavLabel5    110025
#define ID_IMAGE_NavIcon5    110024
#define ID_IMAGE_NavLabel4    110023
#define ID_IMAGE_NavIcon4    110022
#define ID_IMAGE_NavLabel3    110021
#define ID_IMAGE_NavIcon3    110020
#define ID_IMAGE_NavLabel2    110019
#define ID_IMAGE_NavIcon2    110018
#define ID_IMAGE_NavLabel1    110017
#define ID_IMAGE_NavIcon1    110016
#define ID_IMAGE_NavBar    110006
#define ID_IMAGE_Crop3    110005
#define ID_IMAGE_Crop2    110004
#define ID_IMAGE_Crop1    110003
#define ID_IMAGE_CropLb    50016
#define ID_IMAGE_ToolTip    50015
#define ID_IMAGE_BtnSave    20006
#define ID_IMAGE_BtnClear    20005
#define ID_IMAGE_BtnUndo    20004
#define ID_IMAGE_W3    20003
#define ID_IMAGE_W3Rad    52004
#define ID_IMAGE_W2    20002
#define ID_IMAGE_W2Rad    52003
#define ID_IMAGE_W1    20001
#define ID_IMAGE_W1Rad    52002
#define ID_IMAGE_ToolWidthLb    50014
#define ID_IMAGE_Sw5    50013
#define ID_IMAGE_Sw4    50012
#define ID_IMAGE_Sw3    50011
#define ID_IMAGE_Sw2    50010
#define ID_IMAGE_Sw1    50009
#define ID_IMAGE_ToolColorLb    50008
#define ID_IMAGE_ToolTitle    50007
#define ID_IMAGE_ToolCard    110002
#define ID_IMAGE_CanvasHint    50006
#define ID_IMAGE_Canvas    52001
#define ID_IMAGE_CanvasCard    110001
#define ID_IMAGE_ImageView3    50005
#define ID_IMAGE_ImageView2    50004
#define ID_IMAGE_ImageView1    50003
#define ID_IMAGE_TitleSub    50002
#define ID_IMAGE_TitleText    50001
/*TAG:Macro宏ID END*/

class imageActivity : public Activity, 
                     public ZKSeekBar::ISeekBarChangeListener, 
                     public ZKListView::IItemClickListener,
                     public ZKListView::AbsListAdapter,
                     public ZKSlideWindow::ISlideItemClickListener,
                     public EasyUIContext::ITouchListener,
                     public ZKEditText::ITextChangeListener,
                     public ZKVideoView::IVideoPlayerMessageListener
{
public:
    imageActivity();
    virtual ~imageActivity();

    /**
     * 注册定时器
     */
	void registerUserTimer(int id, int time);
	/**
	 * 取消定时器
	 */
	void unregisterUserTimer(int id);
	/**
	 * 重置定时器
	 */
	void resetUserTimer(int id, int time);

protected:
    /*TAG:PROTECTED_FUNCTION*/
    virtual const char* getAppName() const;
    virtual void onCreate();
    virtual void onClick(ZKBase *pBase);
    virtual void onResume();
    virtual void onPause();
    virtual void onIntent(const Intent *intentPtr);
    virtual bool onTimer(int id);

    virtual void onProgressChanged(ZKSeekBar *pSeekBar, int progress);

    virtual int getListItemCount(const ZKListView *pListView) const;
    virtual void obtainListItemData(ZKListView *pListView, ZKListView::ZKListItem *pListItem, int index);
    virtual void onItemClick(ZKListView *pListView, int index, int subItemIndex);

    virtual void onSlideItemClick(ZKSlideWindow *pSlideWindow, int index);

    virtual bool onTouchEvent(const MotionEvent &ev);

    virtual void onTextChanged(ZKTextView *pTextView, const string &text);

    void rigesterActivityTimer();

    virtual void onVideoPlayerMessage(ZKVideoView *pVideoView, int msg);
    void videoLoopPlayback(ZKVideoView *pVideoView, int msg, size_t callbackTabIndex);
    void startVideoLoopPlayback();
    void stopVideoLoopPlayback();
    bool parseVideoFileList(const char *pFileListPath, std::vector<string>& mediaFileList);
    int removeCharFromString(string& nString, char c);


private:
    /*TAG:PRIVATE_VARIABLE*/
    int mVideoLoopIndex;
    int mVideoLoopErrorCount;

};

#endif