/***********************************************
/gen auto by zuitools
***********************************************/
#ifndef __WIDGETSACTIVITY_H__
#define __WIDGETSACTIVITY_H__


#include "app/Activity.h"
#include "entry/EasyUIContext.h"

#include "uart/ProtocolData.h"
#include "uart/ProtocolParser.h"

#include "utils/Log.h"
#include "control/ZKDigitalClock.h"
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
#define ID_WIDGETS_NavBtn8    110136
#define ID_WIDGETS_NavBtn7    110135
#define ID_WIDGETS_NavBtn6    110134
#define ID_WIDGETS_NavBtn5    110133
#define ID_WIDGETS_NavBtn4    110132
#define ID_WIDGETS_NavBtn3    110131
#define ID_WIDGETS_NavBtn2    110130
#define ID_WIDGETS_NavBtn1    110129
#define ID_WIDGETS_NavLabel8    110029
#define ID_WIDGETS_NavIcon8    110028
#define ID_WIDGETS_NavLabel7    110027
#define ID_WIDGETS_NavIcon7    110026
#define ID_WIDGETS_NavLabel6    110025
#define ID_WIDGETS_NavIcon6    110024
#define ID_WIDGETS_NavLabel5    110023
#define ID_WIDGETS_NavIcon5    110022
#define ID_WIDGETS_NavLabel4    110021
#define ID_WIDGETS_NavIcon4    110020
#define ID_WIDGETS_NavLabel3    110019
#define ID_WIDGETS_NavIcon3    110018
#define ID_WIDGETS_NavLabel2    110017
#define ID_WIDGETS_NavIcon2    110016
#define ID_WIDGETS_NavLabel1    110015
#define ID_WIDGETS_NavIcon1    110014
#define ID_WIDGETS_NavBar    110004
#define ID_WIDGETS_ImageView6    50031
#define ID_WIDGETS_PanelBar    91002
#define ID_WIDGETS_PanelBody    50030
#define ID_WIDGETS_PanelTitle    50029
#define ID_WIDGETS_CardPanel    110003
#define ID_WIDGETS_BadgeNum    50028
#define ID_WIDGETS_BadgeDot    50027
#define ID_WIDGETS_ImageView5    50026
#define ID_WIDGETS_BadgeHost    110002
#define ID_WIDGETS_ToastTxt    50025
#define ID_WIDGETS_ImageView4    50024
#define ID_WIDGETS_Toast    110001
#define ID_WIDGETS_Lb3166    50023
#define ID_WIDGETS_StepPlus    20007
#define ID_WIDGETS_StepVal    50022
#define ID_WIDGETS_StepMinus    20006
#define ID_WIDGETS_SkVal    50021
#define ID_WIDGETS_SkDemo    91001
#define ID_WIDGETS_Lb3084    50020
#define ID_WIDGETS_Rd1Txt    50019
#define ID_WIDGETS_Rd1Dot    50018
#define ID_WIDGETS_Rd1    50017
#define ID_WIDGETS_CbOffTxt    50016
#define ID_WIDGETS_CbOff    50015
#define ID_WIDGETS_CbOnTxt    50014
#define ID_WIDGETS_CbOnMark    50013
#define ID_WIDGETS_CbOn    50012
#define ID_WIDGETS_SwOffKnob    50011
#define ID_WIDGETS_SwOff    50010
#define ID_WIDGETS_SwOnKnob    50009
#define ID_WIDGETS_SwOn    50008
#define ID_WIDGETS_Lb2990    50007
#define ID_WIDGETS_BtnDisabled    20005
#define ID_WIDGETS_BtnIcon    20004
#define ID_WIDGETS_BtnGhost    20003
#define ID_WIDGETS_BtnOutline    20002
#define ID_WIDGETS_BtnPrimary    20001
#define ID_WIDGETS_Lb2896    50006
#define ID_WIDGETS_ImageView3    50005
#define ID_WIDGETS_ImageView2    50004
#define ID_WIDGETS_ImageView1    50003
#define ID_WIDGETS_TitleSub    50002
#define ID_WIDGETS_TitleText    50001
/*TAG:Macro宏ID END*/

class widgetsActivity : public Activity, 
                     public ZKSeekBar::ISeekBarChangeListener, 
                     public ZKListView::IItemClickListener,
                     public ZKListView::AbsListAdapter,
                     public ZKSlideWindow::ISlideItemClickListener,
                     public EasyUIContext::ITouchListener,
                     public ZKEditText::ITextChangeListener,
                     public ZKVideoView::IVideoPlayerMessageListener
{
public:
    widgetsActivity();
    virtual ~widgetsActivity();

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