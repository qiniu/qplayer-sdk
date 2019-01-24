/*******************************************************************************
	File:		qcPlayer.h

	Contains:	yy media engine define header file.

	Written by:	Bangfei Jin

	Change History (most recent first):
	2016-12-18		Bangfei			Create file

*******************************************************************************/
#ifndef __qcPlayer_h__
#define __qcPlayer_h__
#include "qcDef.h"
#include "qcType.h"
#include "qcErr.h"
#include "qcMsg.h"
#include "qcData.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef enum {
	QC_PLAY_Init	= 0,
	QC_PLAY_Open	= 1,
	QC_PLAY_Run		= 2,
	QC_PLAY_Pause	= 3,
	QC_PLAY_Stop	= 4,
	QC_PLAY_MAX		= 0X7FFFFFFF
} QCPLAY_STATUS;

// the flag for open source
#define	QCPLAY_OPEN_VIDDEC_HW				0X01000000

#define	QCPLAY_OPEN_SAME_SOURCE				0X02000000

#define	QCPLAY_OPEN_EXT_SOURCE_AV			0X10000000
#define	QCPLAY_OPEN_EXT_SOURCE_IO			0X20000000

// Call back function of player notify event
typedef void (QC_API * QCPlayerNotifyEvent) (void * pUserData, int nID, void * pValue1);

// Call back function of player send out video or audio render data. 
// The pUserData as same as pUserData of QCPlayerNotifyEvent.
// The param pBuffer field nValue is 11 means player don't need render.
typedef int  (QC_API * QCPlayerOutAVData) (void * pUserData, QC_DATA_BUFF * pBuffer);

/**
 * the yy media player interface 
 * The parameter void * hPlayer should be QCM_Player which was created from yyMediaCreate
 */
typedef struct
{
	// Define the version of the player.
	int				nVersion;
	// The player engine handle.
	void *			hPlayer;
	// It will recieve the message if you set the notify function.
	int				(* SetNotify)	(void * hPlayer, QCPlayerNotifyEvent pFunc, void * pUserData);
	// This function must call before open.
	int				(* SetView)		(void * hPlayer, void * hView, RECT * pRect);
	// Open the source. The format should be 0, the player will auto detect the format.
	int 			(* Open)		(void * hPlayer, const char * pURL, int nFlag);
	// Close the source to release the resource
	int 			(* Close)		(void * hPlayer);
	int 			(* Run)			(void * hPlayer);
	int 			(* Pause)		(void * hPlayer);
	int 			(* Stop)		(void * hPlayer);
	QCPLAY_STATUS	(* GetStatus)	(void * hPlayer);
	// The time is ms.
	long long		(* GetDur)		(void * hPlayer);
	long long		(* GetPos)		(void * hPlayer);
	long long		(* SetPos)		(void * hPlayer, long long llPos);
	// The volume 100 is orignal volume. 0-99 is low, > 100 is loud.
	int				(* SetVolume)	(void * hPlayer, int nVolume);
	int				(* GetVolume)	(void * hPlayer);
	// for extend function later.
	int 			(* GetParam)	(void * hPlayer, int nID, void * pParam);
	int 			(* SetParam)	(void * hPlayer, int nID, void * pParam);
} QCM_Player;

#define								QC_PLAY_BASE	0X11000000
// The following message for advance user
// Set the video aspect ratio. 
// The parameter should be QCPLAY_ARInfo *.
// This should be called after open done event.
#define	QCPLAY_PID_AspectRatio		QC_PLAY_BASE + 0X01

// Set the play speed.
// The parameter should be double *. it is 0.2 - 32.0
// This should be called after open done event.
#define	QCPLAY_PID_Speed			QC_PLAY_BASE + 0X02

// Set it to disable video. Playback audio only.
// The parameter should int *. 1 disable render, 2 decoder, 0, enable
#define	QCPLAY_PID_Disable_Video	QC_PLAY_BASE + 0X03

// Set the work path
// The parameter should be char *.
#define	QCPLAY_PID_SetWorkPath		QC_PLAY_BASE + 0X04

// Get Stream number.
// The parameter should be int *.
#define	QCPLAY_PID_StreamNum		QC_PLAY_BASE + 0X05

// Set Stream play. The parameter should be int * (0, ... Num - 1).
//								 -1 is auto select stream
// Get Stream play. The parameter should be int *.
#define	QCPLAY_PID_StreamPlay		QC_PLAY_BASE + 0X06

// Get Audio track number.
// The parameter should be int *.
#define	QCPLAY_PID_AudioTrackNum	QC_PLAY_BASE + 0X07

// Set Audio track play. The parameter should be int * (0, ... Num - 1).
// Get Audio track play. The parameter should be int *.
#define	QCPLAY_PID_AudioTrackPlay	QC_PLAY_BASE + 0X08

// Get Video track number.
// The parameter should be int *.
#define	QCPLAY_PID_VideoTrackNum	QC_PLAY_BASE + 0X09

// Set Video track play. The parameter should be int * (0, ... Num - 1).
// Get Video track play. The parameter should be int *.
#define	QCPLAY_PID_VideoTrackPlay	QC_PLAY_BASE + 0X0a

// Get Subtt track number.
// The parameter should be int *.
#define	QCPLAY_PID_SubttTrackNum	QC_PLAY_BASE + 0X0b

// Set Subtt track play. The parameter should be int * (0, ... Num - 1).
// Get Subtt track play. The parameter should be int *.
#define	QCPLAY_PID_SubttTrackPlay	QC_PLAY_BASE + 0X0c

// Get Stream info. The parameter should be QC_STREAM_FORMAT * 
#define	QCPLAY_PID_StreamInfo		QC_PLAY_BASE + 0X0f

// Set it to zoom video
// The parameter should RECT *. The pos could be divided by 4
// set {0, 0, 0, 0} to disable this feature
#define	QCPLAY_PID_Zoom_Video		QC_PLAY_BASE + 0X11

// Set clock offset time. The parameter int *.
#define	QCPLAY_PID_Clock_OffTime	QC_PLAY_BASE + 0X20

// Set the seek mode. 0, key frame, 1 any pos. int *.
// This can be called anytime
// But it must be called before open if QCPLAY_PID_START_POS enabled.
#define	QCPLAY_PID_Seek_Mode		QC_PLAY_BASE + 0X21

// Set the start pos before play. long long *
// This should be called before open.
#define	QCPLAY_PID_START_POS		QC_PLAY_BASE + 0X22

// Set. Flush the buffer.
#define	QCPLAY_PID_Flush_Buffer		QC_PLAY_BASE + 0X25

// Set to reconnect source.
#define	QCPLAY_PID_Reconnect		QC_PLAY_BASE + 0X30

// Set / Get downloading pause or run. The default is run 
// The param sould be int * . 0 run, 1 pause.
// This should be called when downloading. 
#define	QCPLAY_PID_Download_Pause	QC_PLAY_BASE + 0X31

// Set the perfer file format. Param should QCParserFormat *
// This should be called before open.
#define	QCPLAY_PID_Prefer_Format	QC_PLAY_BASE + 0X50

// Set the perfer io protocol. Param should QCIOProtocol *
// This should be called before open.
#define	QCPLAY_PID_Prefer_Protocol	QC_PLAY_BASE + 0X60

// Set the PD save path. Param should char *
// This should be called before open.
#define	QCPLAY_PID_PD_Save_Path		QC_PLAY_BASE + 0X61

// Set the PD save file ext name. Param should char *
// This should be called before open.
#define	QCPLAY_PID_PD_Save_ExtName	QC_PLAY_BASE + 0X62

// Background color for video render. Param should  QC_COLOR *
// This should be called before open. Default value is black, {0.0, 0.0, 0.0, 1.0}
#define	QCPLAY_PID_BG_COLOR			QC_PLAY_BASE + 0X71

// Set SDK ID. Param should be char*.
// This should be called before open.
#define	QCPLAY_PID_SDK_ID			QC_PLAY_BASE + 0X72
    
// Get RTMP audio message timestamp
// The parameter should be long long *.
#define QCPLAY_PID_RTMP_AUDIO_MSG_TIMESTAMP     QC_PLAY_BASE + 0X73
    
// Get RTMP video message timestamp
// The parameter should be long long *.
#define QCPLAY_PID_RTMP_VIDEO_MSG_TIMESTAMP     QC_PLAY_BASE + 0X74

// Get RTSP udp tcp mode. 0 UDP, 1 TCP
// The parameter should be int *.
#define QCPLAY_PID_RTSP_UDPTCP_MODE			    QC_PLAY_BASE + 0X81

// Set ext audio render. Internal use.
// The parameter should be CBaseAudioRnd *.
#define	QCPLAY_PID_EXT_AudioRnd		QC_PLAY_BASE + 0X0100

// Set ext video render. Internal use.
// The parameter should be CBaseVideoRnd *.
#define	QCPLAY_PID_EXT_VideoRnd		QC_PLAY_BASE + 0X0101

// Set / get the socket connect timeout time
// The parameter should be int *. (ms)
#define	QCPLAY_PID_Socket_ConnectTimeout	QC_PLAY_BASE + 0X0200

// Set / get the socket read timeout time
// The parameter should be int *. (ms)
#define	QCPLAY_PID_Socket_ReadTimeout		QC_PLAY_BASE + 0X0201

// Set the http header referer
// The parameter should be char *. 
#define	QCPLAY_PID_HTTP_HeadReferer			QC_PLAY_BASE + 0X0205

// Set the http header user-agent
// The parameter should be char *. for example "User-Agent: QPlayer Engine"
#define QCPLAY_PID_HTTP_HeadUserAgent       QC_PLAY_BASE + 0X0206

// Set the dns server
// The parameter should be char *. "127.0.0.1" use local. "0.0.0.0" use default.
#define	QCPLAY_PID_DNS_SERVER				QC_PLAY_BASE + 0X0208

// detect the dns get best IP address
// The parameter should be char *. "www.qiniu.com"
#define	QCPLAY_PID_DNS_DETECT				QC_PLAY_BASE + 0X0209

// detect the net work type had changed
// The parameter none
#define	QCPLAY_PID_NET_CHANGED				QC_PLAY_BASE + 0X020A

// Set / get the max buffer time
// The parameter should be int *. (ms)
#define	QCPLAY_PID_PlayBuff_MaxTime			QC_PLAY_BASE + 0X0211

// Set / get the min buffer time
// The parameter should be int *. (ms)
#define	QCPLAY_PID_PlayBuff_MinTime			QC_PLAY_BASE + 0X0212

// Add the source in cache.
// The parameter should be const char *
#define	QCPLAY_PID_ADD_Cache				QC_PLAY_BASE + 0X0250

// Del the source in cache list.
// The parameter should be const char *. 
// If the param is null, delete all cache source.
#define	QCPLAY_PID_DEL_Cache				QC_PLAY_BASE + 0X0251

// Add the source io in cache.
// The parameter should be const char *
#define	QCPLAY_PID_ADD_IOCache				QC_PLAY_BASE + 0X0255

// Del the source io in cache list.
// The parameter should be const char *. 
// If the param is null, delete all cache source.
#define	QCPLAY_PID_DEL_IOCache				QC_PLAY_BASE + 0X0256

// Set the io cache size.
// The parameter should be int *. 
#define	QCPLAY_PID_IOCache_Size				QC_PLAY_BASE + 0X0257

// Set the DRM key 
// The parameter should be char *.
#define	QCPLAY_PID_DRM_KeyText				QC_PLAY_BASE + 0X0301

// Set the file key 
// The parameter should be char *.
#define	QCPLAY_PID_FILE_KeyText				QC_PLAY_BASE + 0X0302

// Set the comp key 
// The parameter should be char *.
#define	QCPLAY_PID_COMP_KeyText				QC_PLAY_BASE + 0X0303

// Set to capture video image
// The parameter should be long long * (ms). capture time. 0 is immediatily.
// This should be called after open done event.
#define	QCPLAY_PID_Capture_Image			QC_PLAY_BASE + 0X0310

// Set the log out level
// The parameter should be int *. 0, None, 1 error, 2 warning, 3 info, 4 debug, 5 dump.
#define	QCPLAY_PID_Log_Level				QC_PLAY_BASE + 0X0320

// Set to call back video buffer. It should be set after open before run.
// The parameter should be function: QCPlayerOutAVData
#define	QCPLAY_PID_SendOut_VideoBuff		QC_PLAY_BASE + 0X0330

// Set to call back Audio buffer. It should be set after open before run.
// The parameter should be function: QCPlayerOutAVData
#define	QCPLAY_PID_SendOut_AudioBuff		QC_PLAY_BASE + 0X0331

// Set the playback loop or not
// The parameter should be int *. 0, no loop, 1 loop
#define	QCPLAY_PID_Playback_Loop			QC_PLAY_BASE + 0X0340

// Set the mp4 preload time
// The parameter should be int *. it is ms to preload
#define	QCPLAY_PID_MP4_PRELOAD				QC_PLAY_BASE + 0X0341

// Set the ai tracking or not
// The parameter should be int *. 0, disable, 1 enable
#define	QCPLAY_PID_EXT_AITracking			QC_PLAY_BASE + 0X0350

// Set the ext source data
// The parameter should be 
// QCPLAY_OPEN_EXT_SOURCE_AV	QC_DATA_BUFF *		
// QCPLAY_OPEN_EXT_SOURCE_IO	QC_DATA_BUFF *	llTime is data pos
#define	QCPLAY_PID_EXT_SOURCE_DATA			QC_PLAY_BASE + 0X0500

// for Android
#define	QCPLAY_PID_EXT_SOURCE_INFO			QC_PLAY_BASE + 0X0501

// Set the ext source video codec id
// The parameter should be int * QCCodecID
#define	QCPLAY_PID_EXT_VIDEO_CODEC			QC_PLAY_BASE + 0X0511
// Set the ext source audio codec id
// The parameter should be int * QCCodecID
#define	QCPLAY_PID_EXT_AUDIO_CODEC			QC_PLAY_BASE + 0X0512

// Set to start save output file. The format depend the file ext name.
// The parameter should be char * . The output file name
#define	QCPLAY_PID_START_MUX_FILE			QC_PLAY_BASE + 0X0520
// Set to stop save output file. 
// The parameter should be int * 0 Stop, 1 Pause, 2 Restart.
#define	QCPLAY_PID_STOP_MUX_FILE			QC_PLAY_BASE + 0X0521

// the video aspect ratio.
typedef struct {
	int		nWidth;
	int		nHeight;
} QCPLAY_ARInfo;

// The video disable type
#define	QC_PLAY_VideoEnable				0
#define	QC_PLAY_VideoDisable_Render		1
#define	QC_PLAY_VideoDisable_Decoder	2

DLLEXPORT_C int QC_API qcCreatePlayer (QCM_Player * fPlayer, void * hInst);
typedef int (QC_API QCCREATEPLAYER) (QCM_Player * fPlayer, void * hInst);

DLLEXPORT_C int QC_API qcDestroyPlayer (QCM_Player * fPlayer);
typedef int (QC_API QCDESTROYPLAYER) (QCM_Player * fPlayer);

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif // __qcPlayer_h__
