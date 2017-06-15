/*******************************************************************************
	File:		qcData.h

	Contains:	qc player data define header file.

	Written by:	Bangfei Jin

	Change History (most recent first):
	2016-11-29		Bangfei			Create file

*******************************************************************************/
#ifndef __qcData_H__
#define __qcData_H__

#include "qcType.h"

//#ifdef __cplusplus
//extern "C" {
//#endif /* __cplusplus */

// What is the source from.
typedef enum {
	QC_SOURCE_QC		= 1,
	QC_SOURCE_FF		= 2,
	QC_SOURCE_EX		= 4,
	QC_SOURCE_MAX		= 0X7FFFFFFF
}QCSourceType;

// the IO protocol
typedef enum {
    QC_IOPROTOCOL_NONE,
    QC_IOPROTOCOL_FILE,
    QC_IOPROTOCOL_HTTP,
    QC_IOPROTOCOL_RTMP,
	QC_IOPROTOCOL_EXTLIB,
	QC_IOPROTOCOL_MAX = 0X7FFFFFFF
}QCIOProtocol;

// the parser format 
typedef enum {
    QC_PARSER_NONE,
    QC_PARSER_M3U8,
    QC_PARSER_MP4,
    QC_PARSER_FLV,
	QC_PARSER_TS,
    QC_PARSER_MP3,
    QC_PARSER_AAC,
	QC_PARSER_MAX		= 0X7FFFFFFF
}QCParserFormat;

typedef enum {
    QC_CODEC_ID_NONE,

    // video codecs 
    QC_CODEC_ID_H264,
    QC_CODEC_ID_H265,
    QC_CODEC_ID_MPEG4,
	QC_CODEC_ID_MJPEG,

    // audio codecs 
    QC_CODEC_ID_AAC		= 0x10000,
    QC_CODEC_ID_MP3, 
	QC_CODEC_ID_MP2,
	QC_CODEC_ID_SPEEX,
	QC_CODEC_ID_MAX		= 0X7FFFFFFF
}QCCodecID;

// the video raw data format
typedef enum  {
	QC_VDT_YUV420_P		= 0,
	QC_VDT_NV12			= 1,
	QC_VDT_YUYV422		= 2,
	QC_VDT_YUV422_P		= 3,
	QC_VDT_YUV444_P		= 4,
	QC_VDT_YUV410_P		= 5,
	QC_VDT_YUV411_P		= 6,
	QC_VDT_RGB565		= 11,
	QC_VDT_RGB24		= 12,
	QC_VDT_RGBA			= 13,
	QC_VDT_ARGB			= 14,
	QC_VDT_MAX			= 0X7FFFFFFF
} QCVideoType;

// the stream media type
typedef enum {
	QC_MEDIA_Data		= 0,
	QC_MEDIA_Source		= 1,
	QC_MEDIA_Video		= 11,
	QC_MEDIA_Audio		= 12,
	QC_MEDIA_Subtt		= 13,
	QC_MEDIA_Image		= 14,
	QC_MEDIA_VBuff		= 21,
	QC_MEDIA_ABuff		= 22,
	QC_MEDIA_All		= 255,
	QC_MEDIA_MAX		= 0X7FFFFFFF
}QCMediaType;

/**
 * General stream format
 */
typedef struct
{
	int				nID;			/*!< Stream id*/
	int				nBitrate;		/*!< bitrate */
	int				nWidth;			/*!< video width */
	int				nHeight;		/*!< video height */
	int				nLevel;			/*!< quality level */
	void *			pPrivateData;	/*!< Private data pointer*/
	int				nPrivateFlag;	/*!< Private data flag*/
} QC_STREAM_FORMAT;

/**
 * General audio format
 */
typedef struct
{
	QCSourceType	nSourceType;	/*!< Souce Type*/
	int				nCodecID;		/*!< codec id*/
	int				nSampleRate;	/*!< Sample rate */
	int				nChannels;		/*!< Channel count */
	int				nBits;			/*!< Bits per sample 8, 16 */
	int				nFourCC;		/*!< Frame type */
	int				nHeadSize;		/*!< head data size*/
	unsigned char *	pHeadData;		/*!< head data*/
	char			szName[32];		/*!< language name */
	void *			pPrivateData;	/*!< Private data pointer*/
	int				nPrivateFlag;	/*!< Private data flag*/
} QC_AUDIO_FORMAT;

/**
 * General video Size
 */
typedef struct
{
	QCSourceType	nSourceType;	/*!< Souce Type*/
	int				nCodecID;		/*!< codec id*/
	int				nWidth;			/*!< Video Width */
	int				nHeight;		/*!< Video Height */
	int				nNum;			/*!< aspect ratio numerator */
	int				nDen;			/*!< aspect ratio denominator */
	int				nFrameTime;		/*!< frame time*/
	int				nFourCC;		/*!< codec type */
	int				nHeadSize;		/*!< head data size*/
	unsigned char *	pHeadData;		/*!< head data*/
	void *			pPrivateData;	/*!< Private data pointer*/
	int				nPrivateFlag;	/*!< Private data flag*/
} QC_VIDEO_FORMAT;

/**
 * General video Size
 */
typedef struct
{
	QCSourceType	nSourceType;	/*!< Souce Type*/
	int				nCodecID;		/*!< codec id*/
	int				nType;			/*!< the type */
	char			szlang[32];		/*!< language info */
	unsigned char *	pHeadData;		/*!< head data*/
	int				nHeadSize;		/*!< head data size*/
	void *			pPrivateData;	/*!< Private data pointer*/
	int				nPrivateFlag;	/*!< Private data flag*/
} QC_SUBTT_FORMAT;

// the buffer status
#define	QCBUFF_NEW_POS			0X00000001
#define	QCBUFF_NEW_FORMAT		0X00000002
#define	QCBUFF_EOS				0X00000004
#define	QCBUFF_KEY_FRAME		0X00000008
#define	QCBUFF_FLUSH			0X00000010
#define	QCBUFF_HEADDATA			0X00000020
#define	QCBUFF_NEWSTREAM		0X00000040
#define	QCBUFF_DISCONNECT		0X00000080
#define	QCBUFF_DROP_FRAME		0X00000100

// for dec
#define	QCBUFF_DEC_DISABLE		0X00010000	// disable dec
#define	QCBUFF_DEC_DISA_DEBLOCK	0X00020000	// disable deblock
#define	QCBUFF_DEC_SKIP_BFRAME	0X00040000	// skip the b frame in dec
#define	QCBUFF_DEC_RENDER		0X00080000	// Video had render in dec
#define	QCBUFF_DEC_SOFT			0X00100000	// use soft dec

// for render
#define	QCBUFF_RND_DISABLE		0X01000000

// the stream media type
typedef enum {
	QC_BUFF_TYPE_Data		= 0,	// the buff is normal data type
	QC_BUFF_TYPE_Video		= 1,	// the buff is video buffer
	QC_BUFF_TYPE_Packet		= 2,	// the buff is packet format from ffmpeg
	QC_BUFF_TYPE_FFFrame	= 3,	// the buff is avframe format from ffmpeg
	QC_BUFF_TYPE_FFSubtt	= 4,	// the buff is subtitle format from ffmpeg
	QC_BUFF_TYPE_PPointer	= 5,	// the buff is pointer pointer.
	QC_BUFF_TYPE_Text		= 6,	// the buff is text
	QC_BUFF_TYPE_MAX		= 0X7FFFFFFF
}QCBufferType;

// the BA mode
#define QC_BA_MODE_AUTO			0X00000001
#define QC_BA_MODE_MANUAL		0X00000002

/**
 * General data buffer.
 */
#ifdef __cplusplus
class CBuffMng;
#endif /* __cplusplus */
typedef int (*QC_FREE_BUFF) (void * pUser, void * pBuff);

typedef struct
{
	QCMediaType			nMediaType;		/*!< buffer type    */
	QCBufferType		uBuffType;		/*!< buffer flags    */
	unsigned int		uFlag;			/*!< the flag of the buffer  */
	unsigned char *		pBuff;			/*!< Buffer pointer */
	void *				pBuffPtr;		/*!< if type was not QC_BUFF_TYPE_Data, the pointer is special data */
	unsigned int		uSize;			/*!< Buffer size in byte */
	long long			llTime;			/*!< The time of the buffer */
	long long			llDelay;		/*!< The time of delay */
	void *				pFormat;		/*!< new format type info (AVT)  */
	int					nValue;			/*!< the value depend on the flag   */
	void *				pData;			/*!< the reverse data point  */
	int					nDataType;		/*!< the reverse data type  */
	unsigned int		uBuffSize;		/*!< Buffer size alloced */
	// for buffer return
#ifdef __cplusplus
	int	(CBuffMng::*	fReturn) (void *);
	CBuffMng *			pBuffMng;
	int					nUsed;			/*!< The buffer used times. It can free when is 0 */
#endif // __cplusplus
	void *				pUserData;
	QC_FREE_BUFF		fFreeBuff;
	// for buffer track info
#ifdef __QC_BUFF_TRACE__
	int					nBuffID;
	int					nRunTime;
	int					nPrevNum;
	int					aPrevID[8];
#endif // __QC_BUFF_TRACE__
} QC_DATA_BUFF;

/**
 * General video buffer structure
 */
typedef struct
{
	unsigned char *		pBuff[3];		/*!< Video buffer */
	int					nStride[3];     /*!< Video buffer stride */
	QCVideoType			nType;			/*!< Video buffer type */
	int					nWidth;			/*!< Video width*/
	int					nHeight;		/*!< Video height*/
} QC_VIDEO_BUFF;


/**
 * Call back function of player notify event
 */
typedef int	 (QC_API * QCDataBuff_CB) (void * pUser, QC_DATA_BUFF * pBuff);

/**
 * call back function structure
 */
typedef struct
{
	QCDataBuff_CB	funcCB;			/*!< call function pointer*/
	void *			userData;		/*!< user data*/
} QC_BUFF_CB;


/**
 * Resource information
 */
typedef struct
{
    char *		pszURL;			/*!< the URL of current stream*/
    char *		pszDomain;		/*!< domain*/
    char *		pszFormat;		/*!< foramt of resource, m3u8, mp4*/
    long long	llDuration;		/*!< duration of resource*/
    int			nVideoCodec;	/*!< video codec*/
    int			nAudioCodec;	/*!< audio codec*/
    int			nBitrate;		/*!< video bitrate*/
    int			nFrameRate;		/*!< frame rate*/
    int			nWidth;			/*!< width of video*/
    int			nHeight;		/*!< height of video*/
} QC_RESOURCE_INFO;

/**
 * Client information, includes device, os, app and so on
 */
typedef struct
{
    char *		pszTimeZone;		/*!< the time zone "+0800" */
    char *		pszDeviceIP;		/*!< IP address of device. IPV4*/
    char *		pszDeviceFamily;	/*!< device family. Phone or Pad...*/
    char *		pszDeviceVersion;	/*!< device version. */
    char *		pszOSFamily;		/*!< OS family. */
    char *		pszOSVersion;		/*!< OS version "6.0" */ 
    char *		pszAppFamily;		/*!< application family. */
    char *		pszAppVersion;		/*!< application version */
    char *		pszPlayerFamily;	/*!< player family. "corePlayer" */
    char *		pszPlayerVersion;	/*!< player version */
    char *		pszRefer;			/*!< refer*/
} QC_CLIENT_INFO;


//#ifdef __cplusplus
//} /* extern "C" */
//#endif /* __cplusplus */

#endif // __qcData_H__
