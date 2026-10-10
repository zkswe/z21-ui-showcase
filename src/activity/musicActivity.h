/***********************************************
/gen auto by zuitools
***********************************************/
#ifndef __MUSICACTIVITY_H__
#define __MUSICACTIVITY_H__


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
#define ID_MUSIC_Btn_volume    20005
#define ID_MUSIC_Btn_heart    20004
#define ID_MUSIC_Btn_next    20003
#define ID_MUSIC_Btn_play    20002
#define ID_MUSIC_Btn_prev    20001
#define ID_MUSIC_MuTimeAll    50009
#define ID_MUSIC_MuTimeNow    50008
#define ID_MUSIC_LyricNext    50007
#define ID_MUSIC_LyricCur    50006
#define ID_MUSIC_LyricPrev    50005
#define ID_MUSIC_SongArtist    50004
#define ID_MUSIC_SongName    50003
#define ID_MUSIC_MuProgress    91001
#define ID_MUSIC_Cover    110001
#define ID_MUSIC_NavBtn8    110134
#define ID_MUSIC_NavBtn7    110133
#define ID_MUSIC_NavBtn6    110132
#define ID_MUSIC_NavBtn5    110131
#define ID_MUSIC_NavBtn4    110130
#define ID_MUSIC_NavBtn3    110129
#define ID_MUSIC_NavBtn2    110128
#define ID_MUSIC_NavBtn1    110127
#define ID_MUSIC_NavLabel8    110027
#define ID_MUSIC_NavIcon8    110026
#define ID_MUSIC_NavLabel7    110025
#define ID_MUSIC_NavIcon7    110024
#define ID_MUSIC_NavLabel6    110023
#define ID_MUSIC_NavIcon6    110022
#define ID_MUSIC_NavLabel5    110021
#define ID_MUSIC_NavIcon5    110020
#define ID_MUSIC_NavLabel4    110019
#define ID_MUSIC_NavIcon4    110018
#define ID_MUSIC_NavLabel3    110017
#define ID_MUSIC_NavIcon3    110016
#define ID_MUSIC_NavLabel2    110015
#define ID_MUSIC_NavIcon2    110014
#define ID_MUSIC_NavLabel1    110013
#define ID_MUSIC_NavIcon1    110012
#define ID_MUSIC_NavBar    110002
#define ID_MUSIC_MuTitle    50002
#define ID_MUSIC_MuBack    50001
/*TAG:Macro宏ID END*/

class musicActivity : public Activity, 
                     public ZKSeekBar::ISeekBarChangeListener, 
                     public ZKListView::IItemClickListener,
                     public ZKListView::AbsListAdapter,
                     public ZKSlideWindow::ISlideItemClickListener,
                     public EasyUIContext::ITouchListener,
                     public ZKEditText::ITextChangeListener,
                     public ZKVideoView::IVideoPlayerMessageListener
{
public:
    musicActivity();
    virtual ~musicActivity();

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