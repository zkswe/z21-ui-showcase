/***********************************************
/gen auto by zuitools
***********************************************/
#ifndef __LISTACTIVITY_H__
#define __LISTACTIVITY_H__


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
#define ID_LIST_NavBtn8    110149
#define ID_LIST_NavBtn7    110148
#define ID_LIST_NavBtn6    110147
#define ID_LIST_NavBtn5    110146
#define ID_LIST_NavBtn4    110145
#define ID_LIST_NavBtn3    110144
#define ID_LIST_NavBtn2    110143
#define ID_LIST_NavBtn1    110142
#define ID_LIST_NavLabel8    110042
#define ID_LIST_NavIcon8    110041
#define ID_LIST_NavLabel7    110040
#define ID_LIST_NavIcon7    110039
#define ID_LIST_NavLabel6    110038
#define ID_LIST_NavIcon6    110037
#define ID_LIST_NavLabel5    110036
#define ID_LIST_NavIcon5    110035
#define ID_LIST_NavLabel4    110034
#define ID_LIST_NavIcon4    110033
#define ID_LIST_NavLabel3    110032
#define ID_LIST_NavIcon3    110031
#define ID_LIST_NavLabel2    110030
#define ID_LIST_NavIcon2    110029
#define ID_LIST_NavLabel1    110028
#define ID_LIST_NavIcon1    110027
#define ID_LIST_NavBar    110017
#define ID_LIST_Cov6Txt    50029
#define ID_LIST_Cov6    110016
#define ID_LIST_Cov5Txt    50028
#define ID_LIST_Cov5    110015
#define ID_LIST_Cov4Txt    50027
#define ID_LIST_Cov4    110014
#define ID_LIST_Cov3Txt    50026
#define ID_LIST_Cov3    110013
#define ID_LIST_Cov2Txt    50025
#define ID_LIST_Cov2    110012
#define ID_LIST_Cov1Txt    50024
#define ID_LIST_Cov1    110011
#define ID_LIST_GridTitle    50023
#define ID_LIST_GridCard    110010
#define ID_LIST_ImageView11    50022
#define ID_LIST_Row4Sub    50021
#define ID_LIST_Row4Name    50020
#define ID_LIST_ImageView10    50019
#define ID_LIST_Row4Cover    110009
#define ID_LIST_Row4    110008
#define ID_LIST_ImageView9    50018
#define ID_LIST_Row3Sub    50017
#define ID_LIST_Row3Name    50016
#define ID_LIST_ImageView8    50015
#define ID_LIST_Row3Cover    110007
#define ID_LIST_Row3    110006
#define ID_LIST_ImageView7    50014
#define ID_LIST_Row2Sub    50013
#define ID_LIST_Row2Name    50012
#define ID_LIST_ImageView6    50011
#define ID_LIST_Row2Cover    110005
#define ID_LIST_Row2    110004
#define ID_LIST_ImageView5    50010
#define ID_LIST_Row1Sub    50009
#define ID_LIST_Row1Name    50008
#define ID_LIST_ImageView4    50007
#define ID_LIST_Row1Cover    110003
#define ID_LIST_Row1    110002
#define ID_LIST_ListTitle    50006
#define ID_LIST_ListCard    110001
#define ID_LIST_ImageView3    50005
#define ID_LIST_ImageView2    50004
#define ID_LIST_ImageView1    50003
#define ID_LIST_TitleSub    50002
#define ID_LIST_TitleText    50001
/*TAG:Macro宏ID END*/

class listActivity : public Activity, 
                     public ZKSeekBar::ISeekBarChangeListener, 
                     public ZKListView::IItemClickListener,
                     public ZKListView::AbsListAdapter,
                     public ZKSlideWindow::ISlideItemClickListener,
                     public EasyUIContext::ITouchListener,
                     public ZKEditText::ITextChangeListener,
                     public ZKVideoView::IVideoPlayerMessageListener
{
public:
    listActivity();
    virtual ~listActivity();

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