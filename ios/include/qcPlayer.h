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
	// The volume should be 0 - 100. 
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
#define	QCPLAY_PID_AspectRatio		QC_PLAY_BASE + 0X01

// Set the play speed.
// The parameter should be double *. it is 0.2 - 32.0
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

// Set clock offset time. The parameter int *.
#define	QCPLAY_PID_Clock_OffTime	QC_PLAY_BASE + 0X20

// Set the seek mode. 0, key frame, 1 any pos. int *.
#define	QCPLAY_PID_Seek_Mode		QC_PLAY_BASE + 0X21

// Set. Flush the buffer.
#define	QCPLAY_PID_Flush_Buffer		QC_PLAY_BASE + 0X25

// Set to reconnect source.
#define	QCPLAY_PID_Reconnect		QC_PLAY_BASE + 0X30

// Set the perfer file format. Param should QCParserFormat *
// This should be called before open.
#define	QCPLAY_PID_Prefer_Format	QC_PLAY_BASE + 0X50

// Set the perfer io protocol. Param should QCIOProtocol *
// This should be called before open.
#define	QCPLAY_PID_Prefer_Protocol	QC_PLAY_BASE + 0X60

// Set the PD save path. Param should char *
// This should be called before open.
#define	QCPLAY_PID_PD_Save_Path		QC_PLAY_BASE + 0X61

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

// Set the dns server
// The parameter should be char *. "127.0.0.1" use local. "0.0.0.0" use default.
#define	QCPLAY_PID_DNS_SERVER				QC_PLAY_BASE + 0X0208

// Set / get the max buffer time
// The parameter should be int *. (ms)
#define	QCPLAY_PID_PlayBuff_MaxTime			QC_PLAY_BASE + 0X0211

// Set / get the min buffer time
// The parameter should be int *. (ms)
#define	QCPLAY_PID_PlayBuff_MinTime			QC_PLAY_BASE + 0X0212

// Set the DRM key 
// The parameter should be char *.
#define	QCPLAY_PID_DRM_KeyText				QC_PLAY_BASE + 0X0301

// Set to capture video image
// The parameter should be long long * (ms). capture time. 0 is immediatily.
#define	QCPLAY_PID_Capture_Image			QC_PLAY_BASE + 0X0310

// Set the log out level
// The parameter should be int *. 0, None, 1 error, 2 warning, 3 info, 4 debug.
#define	QCPLAY_PID_Log_Level				QC_PLAY_BASE + 0X0320

// Set to call back video buffer. It should be set after open before run.
// The parameter should be function: QCPlayerOutAVData
#define	QCPLAY_PID_SendOut_VideoBuff		QC_PLAY_BASE + 0X0330

// Set to call back Audio buffer. It should be set after open before run.
// The parameter should be function: QCPlayerOutAVData
#define	QCPLAY_PID_SendOut_AudioBuff		QC_PLAY_BASE + 0X0331

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
