/*******************************************************************************
	File:		qcType.h

	Contains:	qc player type define header file.

	Written by:	Bangfei Jin

	Change History (most recent first):
	2016-11-29		Bangfei			Create file

*******************************************************************************/
#ifndef __qcType_H__
#define __qcType_H__

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
    
#ifdef __QC_OS_WIN32__
#include "windows.h"
#include "tchar.h"
#elif defined __QC_OS_NDK__ || defined __QC_OS_IOS__
#include <stddef.h>
#include <string.h>
#endif // __QC_OS_WIN32__

#ifdef __QC_OS_WIN32__
#define QC_API __cdecl
#define QC_CBI __stdcall
#define DLLIMPORT_C extern __declspec(dllimport)
#define DLLEXPORT_C __declspec(dllexport)
#else
#define QC_API
#define QC_CBI
#define DLLIMPORT_C 
#define DLLEXPORT_C __attribute__ ((visibility("default")))
#define TCHAR	char
#endif // __QC_OS_WIN32__
    
#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif // __qcType_H__
