/*******************************************************************************
	File:		qcMsg.h

	Contains:	qc player message define header file.

	Written by:	Bangfei Jin

	Change History (most recent first):
	2016-11-29		Bangfei			Create file

*******************************************************************************/
#ifndef __QCMSG_H__
#define __QCMSG_H__

// define the msg ID 
// define msg utility
#define QC_MSG_LOG_TEXT					0x10000001		//param: string

// define the IO msg ID
#define QC_MSG_IO_BASE					0x11000000
#define QC_MSG_HTTP_CONNECT_START		0x11000001		// Param: URL.
#define QC_MSG_HTTP_CONNECT_FAILED		0x11000002		// Param: 
#define QC_MSG_HTTP_CONNECT_SUCESS		0x11000003		// Param: 
#define QC_MSG_HTTP_DNS_START			0x11000004		// Param: 
#define QC_MSG_HTTP_DNS_GET_CACHE		0x11000005		// Param: 
#define QC_MSG_HTTP_DNS_GET_IPADDR		0x11000006		// Param:
#define QC_MSG_HTTP_GET_HEADDATA		0x11000010		// Param: 
#define QC_MSG_HTTP_CONTENT_LEN			0x11000011		// Param: long long size.
#define QC_MSG_HTTP_REDIRECT			0x11000012		// Param: redirect url
#define QC_MSG_HTTP_DISCONNECT_START	0x11000021		// Param: 
#define QC_MSG_HTTP_DISCONNECT_DONE		0x11000022		// Param: 
#define QC_MSG_HTTP_RETURN_CODE			0x11000023		// Param: long 

#define QC_MSG_HTTP_DOWNLOAD_SPEED		0x11000030		// Param: int speed
#define QC_MSG_HTTP_DISCONNECTED		0x11000050		// Param: 
#define QC_MSG_HTTP_RECONNECT_FAILED	0x11000051		// Param: 
#define QC_MSG_HTTP_RECONNECT_SUCESS	0x11000052		// Param:
#define QC_MSG_HTTP_DOWNLOAD_FINISH		0x11000060		// Param:
#define QC_MSG_HTTP_DOWNLOAD_PERCENT	0x11000061		// Param: int 0 -100 %
#define QC_MSG_HTTP_CONTENT_SIZE		0x11000062		// Param: long long bytes
#define QC_MSG_HTTP_BUFFER_SIZE			0x11000063		// Param: long long bytes
#define QC_MSG_HTTP_CONTENT_TYPE        0x11000064      // Param: string
#define QC_MSG_HTTP_SEND_BYTE        	0x11000065      // Param: long long bytes

#define QC_MSG_RTMP_CONNECT_START		0x11010001		// Param: URL.
#define QC_MSG_RTMP_CONNECT_FAILED		0x11010002		// Param:
#define QC_MSG_RTMP_CONNECT_SUCESS		0x11010003		// Param:
#define QC_MSG_RTMP_DOWNLOAD_SPEED		0x11010004		// Param: int speed
#define QC_MSG_RTMP_DNS_GET_IPADDR		0x11010005		// Param:
#define QC_MSG_RTMP_METADATA			0x11010006		// Param: string, JSON object
#define QC_MSG_RTMP_DISCONNECTED		0x11010007		// Param:
#define QC_MSG_RTMP_RECONNECT_FAILED	0x11010008		// Param:
#define QC_MSG_RTMP_RECONNECT_SUCESS	0x11010009		// Param:
#define QC_MSG_RTMP_DNS_GET_CACHE       0x11010010        // Param:


#define QC_MSG_IO_FIRST_BYTE_DONE		0x11020001		// Param:
#define QC_MSG_IO_SEEK_SOURCE_TYPE		0x11020002		// Param: int * 0, None, 1 File, 2 Http,
#define QC_MSG_IO_HANDSHAKE_START       0x11020003
#define QC_MSG_IO_HANDSHAKE_FAILED      0x11020004
#define QC_MSG_IO_HANDSHAKE_SUCESS      0x11020005


// define the parser msg ID
#define QC_MSG_PARSER_BASE				0x12000000
// m_nValue is bitrate
#define QC_MSG_PARSER_NEW_STREAM		0x12000001
#define QC_MSG_PARSER_M3U8_ERROR		0x12000010
#define QC_MSG_PARSER_FLV_ERROR			0x12000020
#define QC_MSG_PARSER_MP4_ERROR			0x12000030


// define the Audio Dec msg ID
#define QC_MSG_AUDIO_DEC				0x13000000


// define the Video Dec msg ID
#define QC_MSG_VIDEO_DEC				0x14000000
#define QC_MSG_VIDEO_HWDEC_FAILED		0x14000001

// define the Sink msg ID
#define QC_MSG_SINK_BASE				0x15000000

// define the Audio Sink msg ID
#define QC_MSG_SINK_AUDIO				0x15100000
#define QC_MSG_SNKA_FIRST_FRAME			0x15100001
#define QC_MSG_SNKA_EOS					0x15100002
// m_nValue is sample rate, m_llValue is channel
#define QC_MSG_SNKA_NEW_FORMAT			0x15100003
// m_llValue is timestamp
#define QC_MSG_SNKA_RENDER				0x15100004

// define the Video Sink msg ID
#define QC_MSG_SINK_VIDEO				0x15200000
#define QC_MSG_SNKV_FIRST_FRAME			0x15200001
#define QC_MSG_SNKV_EOS					0x15200002
// m_nValue is width, m_llValue is Height
#define QC_MSG_SNKV_NEW_FORMAT			0x15200003
// m_llValue is timestamp
#define QC_MSG_SNKV_RENDER				0x15200004
// m_nValue is the rotate angle (90, 180, 270)
#define QC_MSG_SNKV_ROTATE				0x15200005


// define the play msg ID
#define QC_MSG_PLAY_BASE				0x16000000
#define QC_MSG_PLAY_OPEN_DONE			0x16000001
#define QC_MSG_PLAY_OPEN_FAILED			0x16000002
#define QC_MSG_PLAY_CLOSE_DONE			0x16000003
#define QC_MSG_PLAY_CLOSE_FAILED		0x16000004
#define QC_MSG_PLAY_SEEK_DONE			0x16000005
#define QC_MSG_PLAY_SEEK_FAILED			0x16000006
#define QC_MSG_PLAY_COMPLETE			0x16000007  // the param is int *. 0 is normal ,1 is error.
#define QC_MSG_PLAY_STATUS 				0x16000008
#define QC_MSG_PLAY_DURATION			0x16000009
#define QC_MSG_PLAY_OPEN_START			0x1600000A
#define QC_MSG_PLAY_SEEK_START			0x1600000B
#define QC_MSG_PLAY_RUN					0x1600000C
#define QC_MSG_PLAY_PAUSE				0x1600000D
#define QC_MSG_PLAY_STOP				0x1600000E
#define QC_MSG_PLAY_CAPTURE_IMAGE		0x16000010		//param m_pInfo QC_DATA_BUFF pointer. 
#define QC_MSG_PLAY_LOOP_TIMES			0x16000011

#define QC_MSG_PLAY_CACHE_DONE			0x16000021
#define QC_MSG_PLAY_CACHE_FAILED		0x16000022

// define the buffer msg ID
#define QC_MSG_BUFF_VBUFFTIME			0x18000001		//param: video buff time
#define QC_MSG_BUFF_ABUFFTIME			0x18000002		//param: audio buff time
#define QC_MSG_BUFF_GOPTIME				0x18000003		//param: video GOP time
#define QC_MSG_BUFF_VFPS				0x18000004		//param: video FPS
#define QC_MSG_BUFF_AFPS				0x18000005		//param: audio FPS
#define QC_MSG_BUFF_VBITRATE			0x18000006		//param: video bitrate
#define QC_MSG_BUFF_ABITRATE			0x18000007		//param: audio bitrate
#define QC_MSG_BUFF_SEI_DATA			0x18000008		//param: param m_pInfo QC_DATA_BUFF pointer, pBuff is NALU, uSize is size

#define QC_MSG_BUFF_NEWSTREAM			0x18000014		//param: 
#define QC_MSG_BUFF_START_BUFFERING		0x18000016		//param: playing time
#define QC_MSG_BUFF_END_BUFFERING		0x18000017		//param:

// define the render msg ID
#define QC_MSG_RENDER_BASE				0x19000000
#define QC_MSG_RENDER_VIDEO_FPS			0x19000001		//param: FPS of video render
#define QC_MSG_RENDER_AUDIO_FPS			0x19000002		//param: FPS of audio render

#define QC_MSG_THREAD_EXIT				0x20000001		//param: exit the work thread


#endif // __QCMSG_H__
