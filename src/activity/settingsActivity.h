/***********************************************
/gen auto by zuitools
***********************************************/
#ifndef __SETTINGSACTIVITY_H__
#define __SETTINGSACTIVITY_H__


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
#define ID_SETTINGS_NavBtn8    110134
#define ID_SETTINGS_NavBtn7    110133
#define ID_SETTINGS_NavBtn6    110132
#define ID_SETTINGS_NavBtn5    110131
#define ID_SETTINGS_NavBtn4    110130
#define ID_SETTINGS_NavBtn3    110129
#define ID_SETTINGS_NavBtn2    110128
#define ID_SETTINGS_NavBtn1    110127
#define ID_SETTINGS_NavLabel8    110027
#define ID_SETTINGS_NavIcon8    110026
#define ID_SETTINGS_NavLabel7    110025
#define ID_SETTINGS_NavIcon7    110024
#define ID_SETTINGS_NavLabel6    110023
#define ID_SETTINGS_NavIcon6    110022
#define ID_SETTINGS_NavLabel5    110021
#define ID_SETTINGS_NavIcon5    110020
#define ID_SETTINGS_NavLabel4    110019
#define ID_SETTINGS_NavIcon4    110018
#define ID_SETTINGS_NavLabel3    110017
#define ID_SETTINGS_NavIcon3    110016
#define ID_SETTINGS_NavLabel2    110015
#define ID_SETTINGS_NavIcon2    110014
#define ID_SETTINGS_NavLabel1    110013
#define ID_SETTINGS_NavIcon1    110012
#define ID_SETTINGS_NavBar    110002
#define ID_SETTINGS_BtnReset    20004
#define ID_SETTINGS_BtnSave    20003
#define ID_SETTINGS_Diag6V    50033
#define ID_SETTINGS_Diag6K    50032
#define ID_SETTINGS_Diag5V    50031
#define ID_SETTINGS_Diag5K    50030
#define ID_SETTINGS_Diag4V    50029
#define ID_SETTINGS_Diag4K    50028
#define ID_SETTINGS_Diag3V    50027
#define ID_SETTINGS_Diag3K    50026
#define ID_SETTINGS_Diag2V    50025
#define ID_SETTINGS_Diag2K    50024
#define ID_SETTINGS_Diag1V    50023
#define ID_SETTINGS_Diag1K    50022
#define ID_SETTINGS_DiagCard    110001
#define ID_SETTINGS_Lb2096    50021
#define ID_SETTINGS_Tg4Knob    50020
#define ID_SETTINGS_Tg4    50019
#define ID_SETTINGS_Tg4Name    50018
#define ID_SETTINGS_Tg3Knob    50017
#define ID_SETTINGS_Tg3    50016
#define ID_SETTINGS_Tg3Name    50015
#define ID_SETTINGS_Tg2Knob    50014
#define ID_SETTINGS_Tg2    50013
#define ID_SETTINGS_Tg2Name    50012
#define ID_SETTINGS_Tg1Knob    50011
#define ID_SETTINGS_Tg1    50010
#define ID_SETTINGS_Tg1Name    50009
#define ID_SETTINGS_Lb2990    50008
#define ID_SETTINGS_SegLight    20002
#define ID_SETTINGS_SegDark    20001
#define ID_SETTINGS_SegTrack    50007
#define ID_SETTINGS_Lb2896    50006
#define ID_SETTINGS_ImageView3    50005
#define ID_SETTINGS_ImageView2    50004
#define ID_SETTINGS_ImageView1    50003
#define ID_SETTINGS_TitleSub    50002
#define ID_SETTINGS_TitleText    50001
/*TAG:Macro宏ID END*/

class settingsActivity : public Activity, 
                     public ZKSeekBar::ISeekBarChangeListener, 
                     public ZKListView::IItemClickListener,
                     public ZKListView::AbsListAdapter,
                     public ZKSlideWindow::ISlideItemClickListener,
                     public EasyUIContext::ITouchListener,
                     public ZKEditText::ITextChangeListener,
                     public ZKVideoView::IVideoPlayerMessageListener
{
public:
    settingsActivity();
    virtual ~settingsActivity();

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