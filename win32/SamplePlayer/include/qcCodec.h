/*******************************************************************************
	File:		qcCodec.h

	Contains:	codec interface define header file.

	Written by:	Bangfei Jin

	Change History (most recent first):
	2017-02-24		Bangfei			Create file

*******************************************************************************/
#ifndef __qcCodec_h__
#define __qcCodec_h__

#include "qcData.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

enum qcAudioSampleFormat {
	QA_SAMPLE_FMT_NONE = -1,
	QA_SAMPLE_FMT_U8,          ///< unsigned 8 bits
	QA_SAMPLE_FMT_S16,         ///< signed 16 bits
	QA_SAMPLE_FMT_S32,         ///< signed 32 bits
	QA_SAMPLE_FMT_FLT,         ///< float
	QA_SAMPLE_FMT_DBL,         ///< double

	QA_SAMPLE_FMT_U8P,         ///< unsigned 8 bits, planar
	QA_SAMPLE_FMT_S16P,        ///< signed 16 bits, planar
	QA_SAMPLE_FMT_S32P,        ///< signed 32 bits, planar
	QA_SAMPLE_FMT_FLTP,        ///< float, planar
	QA_SAMPLE_FMT_DBLP,        ///< double, planar
	QA_SAMPLE_FMT_S64,         ///< signed 64 bits
	QA_SAMPLE_FMT_S64P,        ///< signed 64 bits, planar

	QA_SAMPLE_FMT_NB           ///< Number of sample formats. DO NOT USE if linking dynamically
};

// qc Audio frame info
typedef struct
{
	int			nSampleRate;
	int			nChannels;
	int			nFormat;
	int			nNBSamples;
	char *		pDataBuff[8];
	int			nDataSize[8];
} QC_AUDIO_FRAME;

/**
 * the qc parser interface 
 */
typedef struct
{
	// Define the version of the Codec. It shuild be 1
	int				nVer;

	// indicate it is video or audio. 1 is video, 0 is audio, -1 is subtt
	int				nAVType;
	// The Codec handle, it will fill in function qcCreateDecoder.
	void *			hCodec;

	// set the input buffer into codec
	int 			(* SetBuff)		(void * hCodec, QC_DATA_BUFF * pBuff);
	// get the output buffer from codec
	int 			(* GetBuff)		(void * hCodec, QC_DATA_BUFF ** ppBuff);

	// flush the Codec
	int				(* Flush)		(void * hCodec);

	// control the Codec
	int 			(* Run)			(void * hCodec);
	int 			(* Pause)		(void * hCodec);
	int 			(* Stop)		(void * hCodec);

	// for extend function later.
	int 			(* GetParam)	(void * hCodec, int nID, void * pParam);
	int 			(* SetParam)	(void * hCodec, int nID, void * pParam);
} QC_Codec_Func;

// create the Codec with Codec type.
DLLEXPORT_C int	qcCreateDecoder (QC_Codec_Func * pCodec, void * pFormat);
typedef int(*QCCREATEDECODER) (QC_Codec_Func * pCodec, void * pFormat);

// destory the Codec
DLLEXPORT_C int	qcDestroyDecoder (QC_Codec_Func * pCodec);
typedef int	(* QCDESTROYDECODER) (QC_Codec_Func * pCodec);

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif // __qcCodec_h__
