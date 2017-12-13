/*******************************************************************************
	File:		qcAna.h

	Contains:	qc player analytics define header file.

	Written by:	Bangfei Jin

	Change History (most recent first):
	2017-04-22		Bangfei			Create file

*******************************************************************************/
#ifndef __QCANA_H__
#define __QCANA_H__

// General device info
typedef struct
{
	char	szDeviceID[64];			/*!< device unique ID */
    char	szAppID[64];			/*!< application unique ID */
    char	szAppFamily[64];		/*!< for example company name of appication */
    char	szAppVersion[64];		/*!< application version */
    char	szDeviceIP[64];			/*!< IP address of device. IPV4. To obtain region? */
    char	szDeviceFamily[64];		/*!< device family. Phone, Pad, Desktop... */
    char	szOSFamily[32];			/*!< OS family. iOS, Android, Windows, macOS */
    char	szOSVersion[16];		/*!< OS version. "6.0" */
    char    szPlayerVersion[16];	/*!< corePlayer SDK version */
    int		nScreenWidth;			/*!< width of device screen */
    int		nScreenHeight;			/*!< height of device screen */
} QCANA_DEVICE_INFO;

// General source info
typedef struct
{
	char*		pszURL;	/*!< the URL of current stream */
    char*		pszFormat;	/*!< format of resource, m3u8, mp4 */
    long long	llDuration;		/*!< duration of resource, 0 is live stream */
    int			nVideoCodec;	/*!< video codec, refer to QCCodecID */
    int			nAudioCodec;	/*!< audio codec, refer to QCCodecID */
    int			nBitrate;		/*!< video bitrate */
    int			nWidth;			/*!< width of video */
    int			nHeight;		/*!< height of video */
    int         nLiveStream;	/*!< stream type, 0 is VOD, 1 is live */
} QCANA_SOURCE_INFO;

typedef enum
{
    QCANA_EVTID_OPEN		= 0,
    QCANA_EVTID_CLOSE		= 1,
    QCANA_EVTID_STARTUP		= 2,
    QCANA_EVTID_LAG			= 3,
    QCANA_EVTID_SEEK		= 4,
    QCANA_EVTID_PAUSE		= 5,
    QCANA_EVTID_BA			= 6,
    QCANA_EVTID_RESUME		= 7,
    QCANA_EVTID_DOWNLOAD	= 8,
    QCANA_EVTID_CONNECT_CHNAGED	= 9,
    QCANA_EVTID_MAX
}QCANA_EVT_ID;

typedef struct tagQCANA_EVT_BASE
{
    long long	llTime;				/*!< event happen time, UTC */
    long long 	llPos;				/*!< event happen position, in ms */
    long long	llEvtDuration;		/*!< event duration, in ms */
    int			nEventID;			/*!< event ID */
    int			nErrCode;			/*!< error code, refer to QC_ERR_XXX */
    char		szSessionID[37];	/*!< playback session ID */
} QCANA_EVT_BASE;

// Close event
typedef struct tagQCANA_EVT_CLOSE : QCANA_EVT_BASE
{
    long long	llWatchDuration;	/*!< total playback time, not include seek */
}QCANA_EVT_CLOSE;

// Seek event
typedef struct tagQCANA_EVT_SEEK : QCANA_EVT_BASE
{
    long long   llWhereFrom;		/*!< where seek from */
    long long   llWhereTo;			/*!< where seek to */
}QCANA_EVT_SEEK;

// Bitrate adaption event
typedef struct tagQCANA_EVT_BA : QCANA_EVT_BASE
{
    long long 			llBAPosition;		/*!< where BA happens */
    QCANA_SOURCE_INFO*	pBitrateFrom;		/*!< the old bitrate */
    QCANA_SOURCE_INFO*	pBitrateTo;			/*!< the new bitrate */
    int					nBAMode;			/*!< manual or auto */
}QCANA_EVT_BA;

// Download information
typedef struct tagQCANA_EVT_DLD : QCANA_EVT_BASE
{
    long long	llDownloadSize;
    int			nDownloadUseTime;

}QCANA_EVT_DLD;


// Event information, (szDeviceID + szAppID + szSessionID) identify one playback for specified end user
typedef struct tagQCANA_EVENT_INFO
{
    QCANA_DEVICE_INFO*	pDevInfo;
    QCANA_SOURCE_INFO*	pSrcInfo;
    QCANA_EVT_BASE*		pEvtInfo;
}QCANA_EVENT_INFO;


#endif // __QCANA_H__
