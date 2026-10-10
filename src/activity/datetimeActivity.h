/***********************************************
/gen auto by zuitools
***********************************************/
#ifndef __DATETIMEACTIVITY_H__
#define __DATETIMEACTIVITY_H__


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
#define ID_DATETIME_NavBtn8    110137
#define ID_DATETIME_NavBtn7    110136
#define ID_DATETIME_NavBtn6    110135
#define ID_DATETIME_NavBtn5    110134
#define ID_DATETIME_NavBtn4    110133
#define ID_DATETIME_NavBtn3    110132
#define ID_DATETIME_NavBtn2    110131
#define ID_DATETIME_NavBtn1    110130
#define ID_DATETIME_NavLabel8    110030
#define ID_DATETIME_NavIcon8    110029
#define ID_DATETIME_NavLabel7    110028
#define ID_DATETIME_NavIcon7    110027
#define ID_DATETIME_NavLabel6    110026
#define ID_DATETIME_NavIcon6    110025
#define ID_DATETIME_NavLabel5    110024
#define ID_DATETIME_NavIcon5    110023
#define ID_DATETIME_NavLabel4    110022
#define ID_DATETIME_NavIcon4    110021
#define ID_DATETIME_NavLabel3    110020
#define ID_DATETIME_NavIcon3    110019
#define ID_DATETIME_NavLabel2    110018
#define ID_DATETIME_NavIcon2    110017
#define ID_DATETIME_NavLabel1    110016
#define ID_DATETIME_NavIcon1    110015
#define ID_DATETIME_NavBar    110005
#define ID_DATETIME_Pk33    50077
#define ID_DATETIME_Pk32    50076
#define ID_DATETIME_Pk31    50075
#define ID_DATETIME_Pk23    50074
#define ID_DATETIME_Pk22    50073
#define ID_DATETIME_Pk21    50072
#define ID_DATETIME_Pk13    50071
#define ID_DATETIME_Pk12    50070
#define ID_DATETIME_Pk11    50069
#define ID_DATETIME_PkBand    50068
#define ID_DATETIME_PkColH3    50067
#define ID_DATETIME_PkColH2    50066
#define ID_DATETIME_PkColH1    50065
#define ID_DATETIME_PickerTitle    50064
#define ID_DATETIME_PickerCard    110004
#define ID_DATETIME_NeedleHub    50063
#define ID_DATETIME_Needle    50062
#define ID_DATETIME_DialH12    50061
#define ID_DATETIME_DialH11    50060
#define ID_DATETIME_DialH10    50059
#define ID_DATETIME_DialH9    50058
#define ID_DATETIME_DialH8    50057
#define ID_DATETIME_DialH7    50056
#define ID_DATETIME_DialH6    50055
#define ID_DATETIME_DialH5    50054
#define ID_DATETIME_DialH4    50053
#define ID_DATETIME_DialH3    50052
#define ID_DATETIME_DialH2    50051
#define ID_DATETIME_DialH1    50050
#define ID_DATETIME_Dial    110003
#define ID_DATETIME_ImageView6    50049
#define ID_DATETIME_ClockDate    50048
#define ID_DATETIME_ClockBig    50047
#define ID_DATETIME_ClockCard    110002
#define ID_DATETIME_CalD31    50046
#define ID_DATETIME_CalD30    50045
#define ID_DATETIME_CalD29    50044
#define ID_DATETIME_CalD28    50043
#define ID_DATETIME_CalD27    50042
#define ID_DATETIME_CalD26    50041
#define ID_DATETIME_CalD25    50040
#define ID_DATETIME_CalD24    50039
#define ID_DATETIME_CalD23    50038
#define ID_DATETIME_CalD22    50037
#define ID_DATETIME_CalD21    50036
#define ID_DATETIME_CalD20    50035
#define ID_DATETIME_CalD19    50034
#define ID_DATETIME_CalD18    50033
#define ID_DATETIME_CalD17    50032
#define ID_DATETIME_CalD16    50031
#define ID_DATETIME_CalD15    50030
#define ID_DATETIME_CalD14    50029
#define ID_DATETIME_CalD13    50028
#define ID_DATETIME_CalD12    50027
#define ID_DATETIME_CalD11    50026
#define ID_DATETIME_CalD10    50025
#define ID_DATETIME_CalD9    50024
#define ID_DATETIME_CalD8    50023
#define ID_DATETIME_CalD7    50022
#define ID_DATETIME_CalD6    50021
#define ID_DATETIME_CalD5    50020
#define ID_DATETIME_CalD4    50019
#define ID_DATETIME_CalD3    50018
#define ID_DATETIME_CalD2    50017
#define ID_DATETIME_CalD1    50016
#define ID_DATETIME_CalW7    50015
#define ID_DATETIME_CalW6    50014
#define ID_DATETIME_CalW5    50013
#define ID_DATETIME_CalW4    50012
#define ID_DATETIME_CalW3    50011
#define ID_DATETIME_CalW2    50010
#define ID_DATETIME_CalW1    50009
#define ID_DATETIME_ImageView5    50008
#define ID_DATETIME_ImageView4    50007
#define ID_DATETIME_CalTitle    50006
#define ID_DATETIME_CalCard    110001
#define ID_DATETIME_ImageView3    50005
#define ID_DATETIME_ImageView2    50004
#define ID_DATETIME_ImageView1    50003
#define ID_DATETIME_TitleSub    50002
#define ID_DATETIME_TitleText    50001
/*TAG:Macro宏ID END*/

class datetimeActivity : public Activity, 
                     public ZKSeekBar::ISeekBarChangeListener, 
                     public ZKListView::IItemClickListener,
                     public ZKListView::AbsListAdapter,
                     public ZKSlideWindow::ISlideItemClickListener,
                     public EasyUIContext::ITouchListener,
                     public ZKEditText::ITextChangeListener,
                     public ZKVideoView::IVideoPlayerMessageListener
{
public:
    datetimeActivity();
    virtual ~datetimeActivity();

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