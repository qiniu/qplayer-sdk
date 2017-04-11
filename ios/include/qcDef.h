/*******************************************************************************
	File:		qcDef.h

	Contains:	qc player micro define header file.

	Written by:	Bangfei Jin

	Change History (most recent first):
	2016-11-29		Bangfei			Create file

*******************************************************************************/
#ifndef __qcDef_H__
#define __qcDef_H__

#ifdef __QC_OS_WIN32__
#pragma warning (disable : 4996)
#endif // __QC_OS_WIN32__

#ifdef __QC_OS_IOS__
#include <stdlib.h>
#endif // __QC_OS_IOS__

// define the module ID
#define QC_MOD_IO_FILE					0x41100000
#define QC_MOD_IO_HTTP					0x41200000
#define QC_MOD_IO_RTMP					0x41300000

#define QC_MOD_PARSER_MP4				0x42100000
#define QC_MOD_PARSER_FLV				0x42200000
#define QC_MOD_PARSER_TS				0x42300000
#define QC_MOD_PARSER_M3U8				0x42400000

#define QC_MOD_AUDIO_DEC_AAC			0x43100000
#define QC_MOD_VIDEO_DEC_H264			0x44100000

#define QC_MOD_SINK_AUDIO				0x45100000
#define QC_MOD_SINK_VIDEO				0x45200000
#define QC_MOD_SINK_DATA				0x45300000

#define QC_MOD_MFW_PLAY					0x46000000


#define	QC_DEL_P(p)   \
	if (p != NULL) {  \
		delete p;     \
		p = NULL; }   \

#define	QC_DEL_A(a)   \
	if (a != NULL) {  \
		delete []a;   \
		a = NULL; }   \

#define	QC_FREE_P(p)   \
	if (p != NULL) {  \
		free (p);     \
		p = NULL; }   \

#define	QC_REL_P(a)   \
	if (a != NULL) {  \
		a->Release(); \
		a = NULL; }   \


#define QC_MAX(a,b) ((a) > (b) ? (a) : (b))
#define QC_MAX3(a,b,c) QC_MAX(QC_MAX(a,b),c)
#define QC_MIN(a,b) ((a) > (b) ? (b) : (a))
#define QC_MIN3(a,b,c) QC_MIN(QC_MIN(a,b),c)

#define QC_MKTAG(a,b,c,d) ((a) | ((b) << 8) | ((c) << 16) | ((unsigned)(d) << 24))
#define QC_MKBETAG(a,b,c,d) ((d) | ((c) << 8) | ((b) << 16) | ((unsigned)(a) << 24))

#ifndef __QC_OS_WIN32__
typedef struct tagRECT
{
    int    left;
    int    top;
    int    right;
    int    bottom;
} RECT, *PRECT;
#endif // __QC_OS_WIN32__

#ifdef __QC_OS_WIN32__
#pragma warning (disable : 4996)
#pragma warning (disable : 4390)
#endif // __QC_OS_WIN32__

#endif // __qcDef_H__
