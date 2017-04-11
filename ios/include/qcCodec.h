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

/**
 * the qc parser interface 
 */
typedef struct
{
	// Define the version of the Codec. It shuild be 1
	int				nVer;
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
DLLEXPORT_C int	qcCreateDecoder (QC_Codec_Func * pCodec, int nCodecID);
typedef int	(* QCCREATEDECODER) (QC_Codec_Func * pCodec, int nCodecID);

// destory the Codec
DLLEXPORT_C int	qcDestroyDecoder (QC_Codec_Func * pCodec);
typedef int	(* QCDESTROYDECODER) (QC_Codec_Func * pCodec);

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif // __qcCodec_h__
