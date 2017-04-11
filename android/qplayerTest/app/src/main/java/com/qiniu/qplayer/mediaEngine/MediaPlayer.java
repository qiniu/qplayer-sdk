/*******************************************************************************
	File:		MediaPlayer.java

	Contains:	player wrap implement file.

	Written by:	Fenger King

	Change History (most recent first):
	2013-09-28		Fenger			Create file

*******************************************************************************/
package com.qiniu.qplayer.mediaEngine;

import android.content.Context;
import android.os.Handler;
import android.os.Message;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.ViewGroup.LayoutParams;
import android.widget.RelativeLayout;
import android.graphics.Bitmap;

import java.lang.ref.WeakReference;

import android.util.DisplayMetrics;
import android.util.Log;
import java.io.UnsupportedEncodingException;

public class MediaPlayer implements BasePlayer {
	private static final String TAG = "QCLOGMediaPlayer";
	// The draw the video on view in main thread
	public static final int QC_EV_Draw_Video = 0x1001;

	private int m_nInitFlag = 0;
	private Context m_context = null;
	private int m_NativeContext = 0;
	private Surface m_NativeSurface = null;
	private SurfaceView m_SurfaceView = null;

	private int m_nStreamNum = 0;
	private int m_nStreamPlay = -1;
	private int	m_nStreamBitrate = 0;

	private int m_nVideoWidth = 0;
	private int m_nVideoHeight = 0;
	private int m_nSampleRate = 0;
	private int m_nChannels = 0;

	private AudioRender m_AudioRender = null;
	private onEventListener m_EventListener = null;

	static {
		System.loadLibrary("QPlayer");
	}

	public int Init(Context context, String apkPath, int nFlag) {
		m_context = context;
		m_nInitFlag = nFlag;
		m_NativeContext = nativeInit(new WeakReference<MediaPlayer>(this), apkPath, nFlag);
		if (m_NativeContext == 0)
			return -1;
		if (m_NativeSurface != null)
			nativeSetView(m_NativeContext, m_NativeSurface);
		return 0;
	}

	public void setEventListener(onEventListener listener) {
		m_EventListener = listener;
	}

	public void SetView(SurfaceView sv) {
		m_SurfaceView = sv;
		if (sv != null) {
			SurfaceHolder sh = sv.getHolder();
			m_NativeSurface = sh.getSurface();
		} else {
			m_NativeSurface = null;
		}
		if (m_NativeContext != 0)
			nativeSetView(m_NativeContext, m_NativeSurface);
	}

	public int Open(String strPath, int nFlag) {
		return nativeOpen(m_NativeContext, strPath, nFlag);
	}

	public void Play() {
		nativePlay(m_NativeContext);
	}

	public void Pause() {
		nativePause(m_NativeContext);
	}

	public void Stop() {
		nativeStop(m_NativeContext);
	}

	public long GetDuration() {
		return nativeGetDuration(m_NativeContext);
	}

	public int GetPos() {
		return nativeGetPos(m_NativeContext);
	}

	public int SetPos(int nPos) {
		return nativeSetPos(m_NativeContext, nPos);
	}

	public int WaitRendTime(long lTime) {
		return nativeWaitRendTime(m_NativeContext, (int) lTime);
	}

	public int GetParam(int nParamId, int nParam, Object objParam) {
		return nativeGetParam(m_NativeContext, nParamId, nParam, objParam);
	}

	public int SetParam(int nParamId, int nParam, Object objParam) {
		return nativeSetParam(m_NativeContext, nParamId, nParam, objParam);
	}

	public void Uninit() {
		if (m_AudioRender != null)
			m_AudioRender.closeTrack();
		nativeUninit(m_NativeContext);
		m_AudioRender = null;
	}

	public int GetVideoWidth() {
		return m_nVideoWidth;
	}

	public int GetVideoHeight() {
		return m_nVideoHeight;
	}

	public void SetVolume(float left, float right) {
		if (m_AudioRender != null)
			m_AudioRender.SetVolume(left, right);
	}

	public int GetStreamNum() {
		if (m_nStreamNum == 0)
			GetParam (QCPLAY_PID_StreamNum, 0, this);
		return m_nStreamNum;

	}
	public int	SetStreamPlay (int nStream) {
		if (nStream == m_nStreamPlay)
			return nStream;
		m_nStreamPlay = nStream;
		SetParam (QCPLAY_PID_StreamPlay, m_nStreamPlay, null);
		return m_nStreamPlay;
	}

	public int GetStreamPlay () {
		GetParam (QCPLAY_PID_StreamPlay, 0, this);
		return m_nStreamPlay;
	}

	public int GetStreamBitrate (int nStream) {
		GetParam (QCPLAY_PID_StreamInfo, nStream, this);
		return m_nStreamBitrate;
	}

	
	public void OnOpenComplete () {

	}

	public void onVideoSizeChanged () {	
		RelativeLayout.LayoutParams lp = (RelativeLayout.LayoutParams)m_SurfaceView.getLayoutParams();
		DisplayMetrics dm = m_context.getResources().getDisplayMetrics();
		if (m_nVideoWidth != 0 && m_nVideoHeight != 0 && lp.width == LayoutParams.FILL_PARENT && lp.height == LayoutParams.FILL_PARENT) 
		{
			int nMaxOutW = dm.widthPixels;
			int nMaxOutH = dm.heightPixels;
			if (nMaxOutW * m_nVideoHeight > m_nVideoWidth * nMaxOutH) {
				lp.height = nMaxOutH;
				lp.width  = m_nVideoWidth * nMaxOutH / m_nVideoHeight;
			} else {
				lp.width  = nMaxOutW;
				lp.height = m_nVideoHeight * nMaxOutW / m_nVideoWidth;
			}	
			//lp.width = lp.width * 2;
			//lp.height = lp.height * 2;		
			m_SurfaceView.setLayoutParams(lp);	
			Log.v("PlayerView", String.format("setSurfaceSize width = %d, height = %d", lp.width , lp.height));			
		}
	} 
		
	private static void postEventFromNative(Object baselayer_ref, int what, int ext1, int ext2, Object obj)
	{
		Log.v(TAG, "postEventFromNative this id is  " + what);
		
		MediaPlayer player = (MediaPlayer)((WeakReference)baselayer_ref).get();
		if (player == null) 
			return;
		
		if (what == QC_MSG_SNKV_NEW_FORMAT)
		{
			player.m_nVideoWidth = ext1;
			player.m_nVideoHeight = ext2;
		}		
		else if (what == QC_MSG_SNKA_NEW_FORMAT)
		{
			player.m_nSampleRate = ext1;
			player.m_nChannels = ext2;
			
			if (player.m_AudioRender == null)	
				player.m_AudioRender = new AudioRender(player.m_context, player);
			player.m_AudioRender.openTrack (player.m_nSampleRate, player.m_nChannels);
			return;
		}			
		Message msg = player.mHandle.obtainMessage(what, obj);
		msg.sendToTarget();	
	}
		
	private static void audioDataFromNative(Object baselayer_ref, byte[] data, int size)
	{
		MediaPlayer player = (MediaPlayer)((WeakReference)baselayer_ref).get();
		if (player == null) 
			return;		
		player.m_AudioRender.writeData(data,  size);
	}
	
	private static void videoDataFromNative(Object baselayer_ref, byte[] data, int size, int lTime, int nFlag)
	{
		MediaPlayer player = (MediaPlayer)((WeakReference)baselayer_ref).get();
		if (player == null) 
			return;
	}	

	private Handler mHandle = new Handler()  
	{
		public void handleMessage(Message msg) 
		{	
			int nRC = 0;
			if (msg.what == QC_MSG_PLAY_OPEN_DONE) {
				OnOpenComplete ();
			}
				
			if (m_EventListener != null) {
				nRC = m_EventListener.onEvent(msg.what, msg.obj);
			}

			if (msg.what == QC_MSG_SNKV_NEW_FORMAT && nRC == 0)
				onVideoSizeChanged ();	
			if (msg.what == QC_MSG_SNKV_NEW_FORMAT)
				SetParam (PARAM_PID_EVENT_DONE, 0, null);
		}
	};
	
	// the native functions
    private native int 	nativeInit(Object player, String apkPath, int nFlag);
    private native int 	nativeUninit(int nNativeContext);
    private native int 	nativeSetView(int nNativeContext, Object view);  
    private native int 	nativeOpen(int nNativeContext,String strPath, int nFlag);
    private native int 	nativePlay(int nNativeContext);
    private native int 	nativePause(int nNativeContext);   
    private native int 	nativeStop(int nNativeContext);     
    private native int 	nativeGetPos(int nNativeContext);    
    private native int 	nativeSetPos(int nNativeContext,int nPos);
    private native long nativeGetDuration(int nNativeContext);
	private native int  nativeWaitRendTime(int nNativeContext, int lTime);
	private native int 	nativeGetParam(int nNativeContext,int nParamId, int nParam, Object objParam);
    private native int 	nativeSetParam(int nNativeContext,int nParamId, int nParam, Object objParam);
}
