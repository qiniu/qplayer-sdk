/*******************************************************************************
	File:		qcParser.h

	Contains:	Parser interface define header file.

	Written by:	Bangfei Jin

	Change History (most recent first):
	2016-11-29		Bangfei			Create file

*******************************************************************************/
#ifndef __qcParser_h__
#define __qcParser_h__

#include "qcIO.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/**
 * the qc parser interface 
 */
typedef struct
{
	// Define the version of the Parser. It shuild be 1
	int				nVer;
	// The Parser handle, it will fill in function.
	void *			hParser;

	// This is buffer manager pointer for internal use. It will set by caller.
	void *			pBuffMng;

	// Open the parser. The flag should be 0 now. the other value shulde be use later.
	int 			(* Open)		(void * hParser, QC_IO_Func * pIO, const char * pURL, int nFlag);
	// Close the parser to release the resource
	int 			(* Close)		(void * hParser);

	// get the stream info
	int				(* GetStreamCount) (void * hParser, QCMediaType nType);
	int				(* GetStreamPlay) (void * hParser, QCMediaType nType);
	int				(* SetStreamPlay) (void * hParser, QCMediaType nType, int nStream);

	// get the source duration.
	long long		(* GetDuration) (void * hParser);

	// get the playing stream format. Caller can not change the format.
	int				(* GetStreamFormat) (void * hParser, int nID, QC_STREAM_FORMAT ** ppStreamFmt);
	int				(* GetAudioFormat) (void * hParser, int nID, QC_AUDIO_FORMAT ** ppAudioFmt);
	int				(* GetVideoFormat) (void * hParser, int nID, QC_VIDEO_FORMAT ** ppVideoFmt);
	int				(* GetSubttFormat) (void * hParser, int nID, QC_SUBTT_FORMAT ** ppSubttFmt);

	// get the source is end of stream or not.
	bool			(* IsEOS) (void * hParser); 

	// get the source is live or not.
	bool			(* IsLive) (void * hParser); 

	// enable the subtitle or not.
	int				(* EnableSubtt) (void * hParser,bool bEnable);

	// control the Parser
	int 			(* Run)			(void * hParser);
	int 			(* Pause)		(void * hParser);
	int 			(* Stop)		(void * hParser);

	// read the data, return read size
	int				(* Read) (void * hParser, QC_DATA_BUFF * pBuff);

	// parse the data, return process size
	int				(* Process) (void * hParser, unsigned char * pBuff, int nSize);

	// return > 0 cans seek, < 0  was failed.
	int		 		(* CanSeek)		(void * hIO);
	// return the current time. ms
	long long		(* GetPos)		(void * hParser);
	// return current pos after set pos. ms
	long long		(* SetPos)		(void * hParser, long long llPos);

	// for extend function later.
	int 			(* GetParam)	(void * hParser, int nID, void * pParam);
	int 			(* SetParam)	(void * hParser, int nID, void * pParam);
} QC_Parser_Func;

// create the Parser with Parser type.
DLLEXPORT_C int		qcCreateParser (QC_Parser_Func * pParser, QCParserFormat nFormat);

// destory the Parser
DLLEXPORT_C int		qcDestroyParser (QC_Parser_Func * pParser);

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif // __qcParser_h__
