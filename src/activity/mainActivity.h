/***********************************************
/gen auto by zuitools
***********************************************/
#ifndef __MAINACTIVITY_H__
#define __MAINACTIVITY_H__


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
#define ID_MAIN_NavBtn8    110140
#define ID_MAIN_NavBtn7    110139
#define ID_MAIN_NavBtn6    110138
#define ID_MAIN_NavBtn5    110137
#define ID_MAIN_NavBtn4    110136
#define ID_MAIN_NavBtn3    110135
#define ID_MAIN_NavBtn2    110134
#define ID_MAIN_NavBtn1    110133
#define ID_MAIN_NavLabel8    110033
#define ID_MAIN_NavIcon8    110032
#define ID_MAIN_NavLabel7    110031
#define ID_MAIN_NavIcon7    110030
#define ID_MAIN_NavLabel6    110029
#define ID_MAIN_NavIcon6    110028
#define ID_MAIN_NavLabel5    110027
#define ID_MAIN_NavIcon5    110026
#define ID_MAIN_NavLabel4    110025
#define ID_MAIN_NavIcon4    110024
#define ID_MAIN_NavLabel3    110023
#define ID_MAIN_NavIcon3    110022
#define ID_MAIN_NavLabel2    110021
#define ID_MAIN_NavIcon2    110020
#define ID_MAIN_NavLabel1    110019
#define ID_MAIN_NavIcon1    110018
#define ID_MAIN_NavBar    110008
#define ID_MAIN_PerfBar    91001
#define ID_MAIN_PerfLabel    50022
#define ID_MAIN_Metric4Value    50021
#define ID_MAIN_Metric4Label    50020
#define ID_MAIN_Metric4    110007
#define ID_MAIN_Metric3Value    50019
#define ID_MAIN_Metric3Label    50018
#define ID_MAIN_Metric3    110006
#define ID_MAIN_Metric2Value    50017
#define ID_MAIN_Metric2Label    50016
#define ID_MAIN_Metric2    110005
#define ID_MAIN_Metric1Value    50015
#define ID_MAIN_Metric1Label    50014
#define ID_MAIN_Metric1    110004
#define ID_MAIN_ImageView5    50013
#define ID_MAIN_Stat2Value    50012
#define ID_MAIN_Stat2Label    50011
#define ID_MAIN_CardStat2    110003
#define ID_MAIN_ImageView4    50010
#define ID_MAIN_Stat1Value    50009
#define ID_MAIN_Stat1Label    50008
#define ID_MAIN_CardStat1    110002
#define ID_MAIN_BtnHeroGo    20001
#define ID_MAIN_BtnHeroGoRad    52001
#define ID_MAIN_HeroSub    50007
#define ID_MAIN_HeroTitle    50006
#define ID_MAIN_HeroCard    110001
#define ID_MAIN_ImageView3    50005
#define ID_MAIN_ImageView2    50004
#define ID_MAIN_ImageView1    50003
#define ID_MAIN_TitleSub    50002
#define ID_MAIN_TitleText    50001
/*TAG:Macro宏ID END*/

class mainActivity : public Activity, 
                     public ZKSeekBar::ISeekBarChangeListener, 
                     public ZKListView::IItemClickListener,
                     public ZKListView::AbsListAdapter,
                     public ZKSlideWindow::ISlideItemClickListener,
                     public EasyUIContext::ITouchListener,
                     public ZKEditText::ITextChangeListener,
                     public ZKVideoView::IVideoPlayerMessageListener
{
public:
    mainActivity();
    virtual ~mainActivity();

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