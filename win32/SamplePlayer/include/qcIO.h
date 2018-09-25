/*******************************************************************************
	File:		qcIO.h

	Contains:	IO interface define header file.

	Written by:	Bangfei Jin

	Change History (most recent first):
	2016-11-29		Bangfei			Create file

*******************************************************************************/
#ifndef __qcIO_h__
#define __qcIO_h__
#include "qcErr.h"
#include "qcDef.h"
#include "qcType.h"
#include "qcData.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define	QCIO_FLAG_READ			1                                   /**< read-only */
#define QCIO_FLAG_WRITE			2                                   /**< write-only */
#define QCIO_FLAG_READ_WRITE	(QCIO_FLAG_READ | QCIO_FLAG_WRITE)	/**< read-write pseudo flag */
#define	QCIO_FLAG_URL			8                                   /**< connect with URL  */

#define QCIO_SEEK_SIZE			0X1000
#define QCIO_SEEK_BEGIN			0X2000
#define QCIO_SEEK_CUR			0X3000
#define QCIO_SEEK_END			0X4000

#define QCIO_READ_DATA			0X0100
#define QCIO_READ_HEAD			0X0200
#define QCIO_READ_AUDIO			0X0400
#define QCIO_READ_VIDEO			0X0800

// open flag
#define QCIO_OPEN_MAINURL		0X0100
#define QCIO_OPEN_CONTENT		0X0200

#define	QCIO_MAX_CONTENT_LEN	0X7FFFFFFFFFFFFFFF

typedef struct
{
	long long	llPos;
	int			nSize;
} QCIO_READ_INFO;

// Get the IO source type at special pos 
// The parameter is pos, it should be long long *. 
#define	QCIO_PID_SourceType				QC_MOD_IO_BASE + 0X01

// Set the IO Exit read if not ready
// The parameter is int *.
#define	QCIO_PID_EXIT_READ				QC_MOD_IO_BASE + 0X02

// Set the ext lib file name
// The parameter should be char *. 
#define	QCIO_PID_EXT_LibName			QC_MOD_IO_BASE + 0X03

// Set the file key
// The parameter is char *.
#define	QCIO_PID_FILE_KEY				QC_MOD_IO_FILE + 0X01

// Set the http host header 
// The parameter should be char *. 
#define	QCIO_PID_HTTP_HeadHost			QC_MOD_IO_HTTP + 0X01

// Get HTTP content type
// The parameter should be char **.
#define QCIO_PID_HTTP_CONTENT_TYPE		QC_MOD_IO_HTTP + 0X02

// Set HTTP notify download percent
// The parameter should be int *.
#define QCIO_PID_HTTP_NOTIFYDL_PERCENT	QC_MOD_IO_HTTP + 0X03

// Get HTTP buffer size
// The parameter not used. return buffer size
#define QCIO_PID_HTTP_BUFF_SIZE			QC_MOD_IO_HTTP + 0X04

// Set HTTP Stop pos
// The parameter should be long long *.
#define QCIO_PID_HTTP_STOP_POS			QC_MOD_IO_HTTP + 0X05

// Set HTTP need sleep when downloading
// The parameter should be int *.
#define QCIO_PID_HTTP_NEED_SLEEP		QC_MOD_IO_HTTP + 0X06

// Get HTTP had down or not
// The parameter should be QCIO_READ_INFO *.
#define QCIO_PID_HTTP_HAD_DOWNLOAD		QC_MOD_IO_HTTP + 0X07

// Set HTTP PD delete the cache file
// The parameter should be int * . 1 delete ,0 not
#define QCIO_PID_HTTP_DEL_FILE			QC_MOD_IO_HTTP + 0X08

// Get HTTP PD save file size
// The parameter should be long long *
#define QCIO_PID_HTTP_SAVE_SIZE			QC_MOD_IO_HTTP + 0X09
    
// Set HTTP Disconnect
// The parameter.
#define QCIO_PID_HTTP_DISCONNECT		QC_MOD_IO_HTTP + 0X10

// Set HTTP Reconnect
// The parameter.
#define QCIO_PID_HTTP_RECONNECT			QC_MOD_IO_HTTP + 0X12

// Set HTTP Data start pos
// The parameter, long long *
#define QCIO_PID_HTTP_DATAPOS			QC_MOD_IO_HTTP + 0X13

// Set HTTP Moov start pos
// The parameter, long long *
#define QCIO_PID_HTTP_MOOVPOS			QC_MOD_IO_HTTP + 0X14

// Set HTTP Moov size
// The parameter, int *
#define QCIO_PID_HTTP_MOOVSIZE			QC_MOD_IO_HTTP + 0X15

// Set HTTP the memory
// The parameter, CHTTPIO2*
#define QCIO_PID_HTTP_COPYMEM			QC_MOD_IO_HTTP + 0X16

// Set HTTP notity message
// The parameter, int * 0, not notify
#define QCIO_PID_HTTP_NOTIFY			QC_MOD_IO_HTTP + 0X17

// Set HTTP open cache
// The parameter, int * 1, Open cache
#define QCIO_PID_HTTP_OPENCACHE			QC_MOD_IO_HTTP + 0X18

// Set HTTP open cache size
// The parameter, int * 
#define QCIO_PID_HTTP_CACHE_SIZE		QC_MOD_IO_HTTP + 0X19

// Get RTMP audio message timestamp
// The parameter should be long long *.
#define QCIO_PID_RTMP_AUDIO_MSG_TIMESTAMP     QC_MOD_IO_RTMP + 0X01
    
// Get RTMP video message timestamp
// The parameter should be long long *.
#define QCIO_PID_RTMP_VIDEO_MSG_TIMESTAMP     QC_MOD_IO_RTMP + 0X02

// Set RTSP tcp or upd  0 UPD, 1 TCP
// The parameter should be int *.
#define QCIO_PID_RTSP_UDP_TCP_MODE			  QC_MOD_IO_RTSP + 0X01

// the IO source type
typedef enum {
    QC_IOTYPE_NONE,
    QC_IOTYPE_FILE,
    QC_IOTYPE_HTTP_VOD,
    QC_IOTYPE_HTTP_LIVE,
    QC_IOTYPE_RTMP,
	QC_IOTYPE_RTSP,
	QC_IOTYPE_EXTLIB,
	QC_IOTYPE_MAX		= 0X7FFFFFFF
}QCIOType;

/**
 * the qc IO interface 
 */
typedef struct
{
	// Define the version of the IO. It shuild be 1
	int				nVer;

	// Define the base instance
	void *			pBaseInst;

	// The IO handle, it will fill in function.
	void *			hIO;

	// Func: Open the IO with URL. 
	// Parm: nFlag is read or write. 
	//       llOffset is start pos of content
	//       nFlag is main url or content
	int 			(* Open)		(void * hIO, const char * pURL, long long llOffset, int nFlag);
	// Func: Reconnect the url. If the URL is null, It will connect url which used in open
	// Parm: pNewURL is NULL, it should connect the original URL.
	//       llOffset is start pos of content. If it is -1, with current pos
	int 			(* Reconnect)	(void * hIO, const char * pNewURL, long long llOffset);
	// Close the IO to release the resource
	int 			(* Close)		(void * hIO);

	// control the IO
	int 			(* Run)			(void * hIO);
	int 			(* Pause)		(void * hIO);
	int 			(* Stop)		(void * hIO);

	// return the io total size.
	long long 		(* GetSize)		(void * hIO);

	// Func: read the data, 
	// Parm: nSize is the pBuff size and return read size
	//	     bFull if it is true, io will fill data eq nSize, false, it will return immedidately
	int				(* Read)		(void * hIO, unsigned char * pBuff, int & nSize, bool bFull, int nFlag);
	// read the data from llPos
	int				(* ReadAt)		(void * hIO, long long llPos, unsigned char * pBuff, int & nSize, bool bFull, int nFlag);
	// read the data from llPos until fill buff full
	int				(* ReadSync)	(void * hIO, long long llPos, unsigned char * pBuff, int nSize, int nFlag);
	// write the data, 
	int				(* Write)		(void * hIO, unsigned char * pBuff, int & nSize);
	// return current pos after set pos.
	long long 		(* SetPos)		(void * hIO, long long llPos, int nFlag);

	// get the down pos, return -1 was failed.
	long long 		(* GetDownPos)		(void * hIO);
	// get the read pos, return -1 was failed.
	long long 		(* GetReadPos)		(void * hIO);
	// get the io download speed it is Bytes/Second
	int				(* GetSpeed)	(void * hIO, int nLastSecs);
	// return the io is newwork live, vod or local file
	QCIOType		(* GetType)		(void * hIO);
	// get the io is live streaming or vod
	bool			(* IsStreaming)	(void * hIO);

	// for extend function later.
	int 			(* GetParam)	(void * hIO, int nID, void * pParam);
	int 			(* SetParam)	(void * hIO, int nID, void * pParam);

	QCIOProtocol	m_nProtocol;
	char			m_szLibInfo[256];
} QC_IO_Func;

// create the IO with IO type.
DLLEXPORT_C int		qcCreateIO (QC_IO_Func * pIO, QCIOProtocol nProt);

// destroy the IO
DLLEXPORT_C int		qcDestroyIO (QC_IO_Func * pIO);

DLLEXPORT_C int		qcFFCreateIO(QC_IO_Func * pIO, QCIOProtocol nProt);
DLLEXPORT_C int		qcFFDestroyIO(QC_IO_Func * pIO);

typedef int (*QCCREATEIO) (QC_IO_Func * pIO, QCIOProtocol nProt);
typedef int (*QCDESTROYIO) (QC_IO_Func * pIO);

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif // __qcIO_h__
