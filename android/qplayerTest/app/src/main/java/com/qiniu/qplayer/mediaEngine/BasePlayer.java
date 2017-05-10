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
	// define the flag for player
	public static final int		QCPLAY_OPEN_VIDDEC_HW		= 0X01000000;

	// define the param id
	public static final int 	PARAM_PID_EVENT_DONE		= 0X100;
	// the param should be double
	public static final	int 	QCPLAY_PID_Speed			= 0X11000002;
	// the param shoud be 1 disable video rnd, 0 enable render.
	public static final	int 	QCPLAY_PID_Disable_Video	= 0X11000003;

	// the param shoudl be int
	public static final	int 	QCPLAY_PID_StreamNum		= 0X11000005;
	public static final	int 	QCPLAY_PID_StreamPlay		= 0X11000006;
	public static final	int 	QCPLAY_PID_StreamInfo		= 0X1100000F;
	public static final int 	QCPLAY_PID_Clock_OffTime	= 0X11000020;
	public static final int 	QCPLAY_PID_Reconnect		= 0X11000030;

	// Define id of event listener.
	public static final int 	QC_MSG_PLAY_OPEN_DONE 		= 0x16000001;
	public static final int 	QC_MSG_PLAY_OPEN_FAILED 	= 0x16000002;
	public static final int 	QC_MSG_PLAY_COMPLETE 		= 0x16000007;
	public static final int 	QC_MSG_PLAY_STATUS	 		= 0x16000008;
	public static final int 	QC_MSG_PLAY_DURATION 		= 0x16000009;
	public static final int 	QC_MSG_PLAY_SEEK_DONE 		= 0x16000005;
	public static final int 	QC_MSG_PLAY_SEEK_FAILED 	= 0x16000006;
	// The first frame video was displayed.
	public static final int 	QC_MSG_SNKV_FIRST_FRAME 	= 0x15200001;

	public static final int 	QC_MSG_HTTP_DISCONNECTED 		= 0x11000050;
	public static final int 	QC_MSG_HTTP_RECONNECT_FAILED 	= 0x11000051;
	public static final int 	QC_MSG_HTTP_RECONNECT_SUCESS 	= 0x11000052;



	// The video size was changed. The player need to resize the
	// surface pos and size.
	public static final int 	QC_MSG_SNKV_NEW_FORMAT 		= 0x15200003;
	// The audio format was changed.
	public static final int 	QC_MSG_SNKA_NEW_FORMAT 		= 0x15100003;


	// The event listener function
	public interface onEventListener{
		public int onEvent (int nID, Object obj);
		public int OnSubTT (String strText, int nTime);	
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
	public int		WaitRendTime (long lTime);
	public int	 	GetParam(int nParamId, int nParam, Object objParam);
	public int	 	SetParam(int nParamId, int nParam, Object objParam);
	public void 	Uninit();
	
	public int 		GetVideoWidth();
	public int 		GetVideoHeight();
	public void 	SetVolume(float left, float right);
	public int		GetStreamNum ();
	public int		SetStreamPlay (int nStream);
	public int		GetStreamPlay ();
	public int		GetStreamBitrate (int nStream);
}


