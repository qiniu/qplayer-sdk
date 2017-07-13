/*******************************************************************************
	File:		BasePlayer.java

	Contains:	player interface file.

	Written by:	Fenger King

	Change History (most recent first):
	2013-12-28		Fenger			Create file

*******************************************************************************/
package com.qiniu.qplayer.mediaEngine;

import android.content.Context;
import android.view.SurfaceView;
import android.graphics.Bitmap;

public interface BasePlayer {
	public static final int		QCPLAY_FORMAT_M3U8			= 1;
	public static final int		QCPLAY_FORMAT_MP4			= 2;
	public static final int		QCPLAY_FORMAT_FLV			= 3;

	public static final int		QC_IOPROTOCOL_HTTPPD 		= 5;

	// define the flag for player
	public static final int		QCPLAY_OPEN_VIDDEC_HW		= 0X01000000;
	public static final int		QCPLAY_OPEN_SAME_SOURCE		= 0X02000000;

	// define the param id
	public static final int 	PARAM_PID_EVENT_DONE		= 0X100;
	// get: the param is null, set the param is 0 -100
	public static final int 	PARAM_PID_AUDIO_VOLUME		= 0X101;

	// the param should be: 0X00010002 is 0.5 speed. 0X00040001 is 4 speed.
	public static final	int 	QCPLAY_PID_Speed			= 0X11000002;
	// the param shoud be 1 disable video rnd, 0 enable render.
	public static final	int 	QCPLAY_PID_Disable_Video	= 0X11000003;

	// the param should be int
	public static final	int 	QCPLAY_PID_StreamNum		= 0X11000005;
	public static final	int 	QCPLAY_PID_StreamPlay		= 0X11000006;
	public static final	int 	QCPLAY_PID_StreamInfo		= 0X1100000F;
	public static final int 	QCPLAY_PID_Clock_OffTime	= 0X11000020;

	// the param.
	public static final int 	QCPLAY_PID_Reconnect		= 0X11000030;

	// Set the perfer io protocol. Param should QC_IOPROTOCOL_HTTPPD 
	// This should be called before open.
	public static final int 	QCPLAY_PID_Prefer_Protocol	= 0X11000060;

	// Set the PD save path. Param should char *
	// This should be called before open.
	public static final int 	QCPLAY_PID_PD_Save_Path		= 0X11000061;

	// Set the perfer file format. Param should QCPLAY_FORMAT_* 
	// This should be called before open.
	public static final int 	QCPLAY_PID_Prefer_Format	= 0X11000050;

	// Set to capture video image
	// The parameter should be long long * (ms). capture time. 0 is immediatily.
	public static final int 	QCPLAY_PID_Capture_Image	= 0X11000310;

	// Set / get the socket connect timeout time
	// The parameter should be int (ms)
	public static final int	QCPLAY_PID_Socket_ConnectTimeout	= 0X11000200;

	// Set / get the socket read timeout time
	// The parameter should be int (ms)
	public static final int	QCPLAY_PID_Socket_ReadTimeout		= 0X11000201;

	// Set the http header referer
	// The parameter should be String.
	public static final int	QCPLAY_PID_HTTP_HeadReferer			= 0X11000205;

	// Set the max buffer time
	// The parameter should be int (ms)
	public static final int	QCPLAY_PID_PlayBuff_MaxTime			= 0X11000211;

	// Set  the min buffer time
	// The parameter should be int (ms)
	public static final int	QCPLAY_PID_PlayBuff_MinTime			= 0X11000212;

	// Set the log out level
	// The parameter should be int, 0, None, 1 error, 2 warning, 3 info, 4 debug.
	public static final int	QCPLAY_PID_Log_Level				= 0X11000320;
	// Set the DRM key
	// The parameter should be byte[].
	public static final int	QCPLAY_PID_DRM_KeyText				= 0X11000301;

	// Define id of event listener.
	public static final int 	QC_MSG_PLAY_OPEN_DONE 			= 0x16000001;
	public static final int 	QC_MSG_PLAY_OPEN_FAILED 		= 0x16000002;
	public static final int 	QC_MSG_PLAY_COMPLETE 			= 0x16000007;
	public static final int 	QC_MSG_PLAY_SEEK_DONE 			= 0x16000005;
	public static final int 	QC_MSG_PLAY_SEEK_FAILED 		= 0x16000006;
	// The first frame video was displayed.
	public static final int 	QC_MSG_SNKV_FIRST_FRAME 		= 0x15200001;
	// The first frame audio was displayed.
	public static final int 	QC_MSG_SNKA_FIRST_FRAME 		= 0x15100001;


	// The nArg1 is the value
	public static final int 	QC_MSG_PLAY_STATUS	 			= 0x16000008;
	public static final int 	QC_MSG_PLAY_DURATION 			= 0x16000009;
	
	public static final int 	QC_MSG_PLAY_RUN					= 0x1600000C;
	public static final int 	QC_MSG_PLAY_PAUSE				= 0x1600000D;
	public static final int 	QC_MSG_PLAY_STOP				= 0x1600000E;


	// http protocol messaage id
	public static final int 	QC_MSG_HTTP_CONNECT_START		= 0x11000001;
	public static final int 	QC_MSG_HTTP_CONNECT_FAILED		= 0x11000002;
	public static final int 	QC_MSG_HTTP_CONNECT_SUCESS		= 0x11000003;
	public static final int 	QC_MSG_HTTP_DNS_START			= 0x11000004;
	public static final int 	QC_MSG_HTTP_REDIRECT			= 0x11000012;
	public static final int 	QC_MSG_HTTP_DISCONNECT_START	= 0x11000021;
	public static final int 	QC_MSG_HTTP_DISCONNECT_DONE		= 0x11000022;
	public static final int 	QC_MSG_HTTP_DOWNLOAD_SPEED		= 0x11000030;
	public static final int 	QC_MSG_HTTP_DISCONNECTED		= 0x11000050;
	public static final int 	QC_MSG_HTTP_RECONNECT_FAILED	= 0x11000051;
	public static final int 	QC_MSG_HTTP_RECONNECT_SUCESS	= 0x11000052;

	public static final int 	QC_MSG_RTMP_CONNECT_START		= 0x11010001;
	public static final int 	QC_MSG_RTMP_CONNECT_FAILED		= 0x11010002;
	public static final int 	QC_MSG_RTMP_CONNECT_SUCESS		= 0x11010003;
	public static final int 	QC_MSG_RTMP_DISCONNECTED		= 0x11010007;
	public static final int 	QC_MSG_RTMP_RECONNECT_FAILED	= 0x11010008;
	public static final int 	QC_MSG_RTMP_RECONNECT_SUCESS	= 0x11010009;

	public static final int		QC_MSG_HTTP_RETURN_CODE			= 0x11000023;

	// The obj is the string value
	public static final int		QC_MSG_RTMP_METADATA			= 0x11010006;

	// The nArg1 is the value.
	public static final int 	QC_MSG_RTMP_DOWNLOAD_SPEED		= 0x11010004;
	public static final int 	QC_MSG_BUFF_VBUFFTIME			= 0x18000001;
	public static final int 	QC_MSG_BUFF_ABUFFTIME			= 0x18000002;

	public static final int		QC_MSG_BUFF_GOPTIME				= 0x18000003;
	public static final int		QC_MSG_BUFF_VFPS				= 0x18000004;
	public static final int		QC_MSG_BUFF_AFPS				= 0x18000005;
	public static final int		QC_MSG_BUFF_VBITRATE			= 0x18000006;
	public static final int		QC_MSG_BUFF_ABITRATE			= 0x18000007;

	// there is no value.
	public static final int 	QC_MSG_BUFF_START_BUFFERING		= 0x18000016;
	public static final int 	QC_MSG_BUFF_END_BUFFERING		= 0x18000017;

	public static final int		QC_MSG_PARSER_M3U8_ERROR		= 0x12000010;
	public static final int		QC_MSG_PARSER_FLV_ERROR			= 0x12000020;
	public static final int		QC_MSG_PARSER_MP4_ERROR			= 0x12000030;	

	public static final int		QC_MSG_VIDEO_HWDEC_FAILED		= 0x14000001;

	// The video size was changed. The player need to resize the
	// surface pos and size.
	public static final int 	QC_MSG_SNKV_NEW_FORMAT 			= 0x15200003;
	// The audio format was changed.
	public static final int 	QC_MSG_SNKA_NEW_FORMAT 			= 0x15100003;


	public static final int 	QC_FLAG_Video_CaptureImage	= 0x00000010;

	// The event listener function
	public interface onEventListener{
		public int onEvent (int nID, int nArg1, int nArg2, Object obj);
		public int OnSubTT (String strText, int nTime);
		public int OnImage (byte[] pData, int nSize);
	}
	
	// Define the functions
	public int 		Init(Context context, String apkPath, int nFlag);
	public void 	setEventListener(onEventListener listener);	
	public void 	SetView(SurfaceView sv);
	public int 		Open (String strPath, int nFlag);
	public void 	Play();
	public void 	Pause();
	public void 	Stop();	
	public long 	GetDuration();	
	public int 		GetPos();
	public int	 	SetPos(int nPos);
	public int	 	GetParam(int nParamId, int nParam, Object objParam);
	public int	 	SetParam(int nParamId, int nParam, Object objParam);
	public void 	Uninit();
	
	public int 		GetVideoWidth();
	public int 		GetVideoHeight();
	public void 	SetVolume(int nVolume);
	public int		GetStreamNum ();
	public int		SetStreamPlay (int nStream);
	public int		GetStreamPlay ();
	public int		GetStreamBitrate (int nStream);
}


