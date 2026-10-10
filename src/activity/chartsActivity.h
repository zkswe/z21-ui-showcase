/***********************************************
/gen auto by zuitools
***********************************************/
#ifndef __CHARTSACTIVITY_H__
#define __CHARTSACTIVITY_H__


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
#define ID_CHARTS_NavBtn8    130134
#define ID_CHARTS_NavBtn7    130133
#define ID_CHARTS_NavBtn6    130132
#define ID_CHARTS_NavBtn5    130131
#define ID_CHARTS_NavBtn4    130130
#define ID_CHARTS_NavBtn3    130129
#define ID_CHARTS_NavBtn2    130128
#define ID_CHARTS_NavBtn1    130127
#define ID_CHARTS_NavLabel8    130027
#define ID_CHARTS_NavIcon8    130026
#define ID_CHARTS_NavLabel7    130025
#define ID_CHARTS_NavIcon7    130024
#define ID_CHARTS_NavLabel6    130023
#define ID_CHARTS_NavIcon6    130022
#define ID_CHARTS_NavLabel5    130021
#define ID_CHARTS_NavIcon5    130020
#define ID_CHARTS_NavLabel4    130019
#define ID_CHARTS_NavIcon4    130018
#define ID_CHARTS_NavLabel3    130017
#define ID_CHARTS_NavIcon3    130016
#define ID_CHARTS_NavLabel2    130015
#define ID_CHARTS_NavIcon2    130014
#define ID_CHARTS_NavLabel1    130013
#define ID_CHARTS_NavIcon1    130012
#define ID_CHARTS_NavBar    110005
#define ID_CHARTS_B12b    50036
#define ID_CHARTS_B12a    50035
#define ID_CHARTS_B11b    50034
#define ID_CHARTS_B11a    50033
#define ID_CHARTS_B10b    50032
#define ID_CHARTS_B10a    50031
#define ID_CHARTS_B9b    50030
#define ID_CHARTS_B9a    50029
#define ID_CHARTS_B8b    50028
#define ID_CHARTS_B8a    50027
#define ID_CHARTS_B7b    50026
#define ID_CHARTS_B7a    50025
#define ID_CHARTS_B6b    50024
#define ID_CHARTS_B6a    50023
#define ID_CHARTS_B5b    50022
#define ID_CHARTS_B5a    50021
#define ID_CHARTS_B4b    50020
#define ID_CHARTS_B4a    50019
#define ID_CHARTS_B3b    50018
#define ID_CHARTS_B3a    50017
#define ID_CHARTS_B2b    50016
#define ID_CHARTS_B2a    50015
#define ID_CHARTS_B1b    50014
#define ID_CHARTS_B1a    50013
#define ID_CHARTS_BarTitle    50012
#define ID_CHARTS_CardBar    110004
#define ID_CHARTS_GaugeBar    130002
#define ID_CHARTS_GaugeValue    50011
#define ID_CHARTS_GaugeTitle    50010
#define ID_CHARTS_CardGauge    110003
#define ID_CHARTS_RingBar    130001
#define ID_CHARTS_RingValue    50009
#define ID_CHARTS_RingTitle    50008
#define ID_CHARTS_CardRing    110002
#define ID_CHARTS_WaveChart    60001
#define ID_CHARTS_LineValue    50007
#define ID_CHARTS_LineTitle    50006
#define ID_CHARTS_CardLine    110001
#define ID_CHARTS_ImageView3    50005
#define ID_CHARTS_ImageView2    50004
#define ID_CHARTS_ImageView1    50003
#define ID_CHARTS_TitleSub    50002
#define ID_CHARTS_TitleText    50001
/*TAG:Macro宏ID END*/

class chartsActivity : public Activity, 
                     public ZKSeekBar::ISeekBarChangeListener, 
                     public ZKListView::IItemClickListener,
                     public ZKListView::AbsListAdapter,
                     public ZKSlideWindow::ISlideItemClickListener,
                     public EasyUIContext::ITouchListener,
                     public ZKEditText::ITextChangeListener,
                     public ZKVideoView::IVideoPlayerMessageListener
{
public:
    chartsActivity();
    virtual ~chartsActivity();

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