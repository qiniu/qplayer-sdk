/*******************************************************************************
	File:		qcErr.h

	Contains:	qc player error define header file.

	Written by:	Bangfei Jin

	Change History (most recent first):
	2016-11-29		Bangfei			Create file

*******************************************************************************/
#ifndef __qcErr_H__
#define __qcErr_H__

// define the error ID 
// define the common return value.
#define QC_ERR_NONE						0x00000000
#define QC_ERR_FINISH					0x00000001
#define QC_ERR_RETRY					0x00000002
#define QC_ERR_NEWFORMAT				0x00000003
#define QC_ERR_NEWSTREAM				0x00000004
#define QC_ERR_NEEDMORE					0x00000005
#define QC_ERR_BUFFERING				0X00000006
#define QC_ERR_EMPTY					0X00000007
#define QC_ERR_FAILED					0x80000001
#define QC_ERR_MEMORY					0x80000002
#define QC_ERR_IMPLEMENT				0x80000003
#define QC_ERR_ARG						0x80000004
#define QC_ERR_TIMEOUT					0x80000005
#define QC_ERR_STATUS					0x80000008
#define QC_ERR_PARAMID					0x80000009
#define QC_ERR_LICENSE					0x8000000a
#define QC_ERR_UNSUPPORT				0x8000000b
#define QC_ERR_FORCECLOSE				0x8000000c
#define QC_ERR_FORMAT					0x8000000d
#define QC_ERR_Overflow					0x8000000e
#define QC_ERR_EMPTYPOINTOR				0x8000000f
#define QC_ERR_IO_FAILED				0x80000010

// define the IO error ID
#define QC_ERR_IO_BASE					0x81000000
#define QC_ERR_IO_EOF                   0x81000001
#define QC_ERR_IO_DNS_FAILED            0x81000002
#define QC_ERR_IO_AGAIN                 0x81000003

// define the IO File error ID
#define QC_ERR_IO_FILE					0x81100000

// define the IO HTTP error ID
#define QC_ERR_IO_HTTP					0x81200000
#define QC_ERR_CANNOT_CONNECT			0x81200001
#define QC_ERR_ServerTerminated			0x81200002
#define QC_ERR_NTAbnormallDisconneted	0x81200003
#define QC_ERR_ServerStreamEOF			0x81200004
#define QC_ERR_ServerDataError			0x81200005
#define QC_ERR_DNSFailed				0x81200006
#define QC_ERR_Disconnected				0x81200007
#define QC_ERR_BadDescriptor			0x81200008
#define QC_ERR_HTTP_EOS					0x81200009


// define the IO RTMP error ID
#define QC_ERR_IO_RTMP					0x81300000

// define the Parser base error ID
#define QC_ERR_PARSER_BASE				0x82000000

// define the MP4 parser error ID
#define QC_ERR_PARSER_MP4				0x82100000

// define the FLV parser error ID
#define QC_ERR_PARSER_FLV				0x82200000

// define the TS parser error ID
#define QC_ERR_PARSER_TS				0x82300000

// define the M3U8 parser error ID
#define QC_ERR_PARSER_M3U8				0x82400000

// define the Audio Dec error ID
#define QC_ERR_AUDIO_DEC				0x83000000

// define the Audio Dec error ID
#define QC_ERR_AUDIO_DEC_AAC			0x83100000

// define the Video Dec error ID
#define QC_ERR_VIDEO_DEC				0x84000000
#define QC_ERR_VIDEO_HWDEC				0x84000001

// define the Video Dec error ID
#define QC_ERR_VIDEO_DEC_H264			0x84100000

// define the Sink error ID
#define QC_ERR_SINK_BASE				0x85000000

// define the Audio Sink error ID
#define QC_ERR_SINK_AUDIO				0x85100000

// define the Video Sink error ID
#define QC_ERR_SINK_VIDEO				0x85200000

// define the play error ID
#define QC_ERR_PLAY_BASE				0x86000000


#define QC_ERR_CHECK(n)		\
	if (n < 0)					\
		return QC_ERR_FAILED;	\

#endif // __qcErr_H__
