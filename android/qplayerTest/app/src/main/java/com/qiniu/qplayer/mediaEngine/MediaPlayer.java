/*******************************************************************************
	File:		MediaPlayer.java

	Contains:	player wrap implement file.

	Written by:	Fenger King

	Change History (most recent first):
	2013-09-28		Fenger			Create file

*******************************************************************************/
package com.qiniu.qplayer.mediaEngine;

import android.content.Context;
import android.media.AudioManager;
import android.os.Environment;
import android.os.Handler;
import android.os.Message;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.ViewGroup.LayoutParams;
import android.widget.RelativeLayout;
import android.graphics.Bitmap;

import java.io.File;
import java.io.FileOutputStream;
import java.lang.ref.WeakReference;

import android.util.DisplayMetrics;
import android.util.Log;
import java.io.UnsupportedEncodingException;

public class MediaPlayer implements BasePlayer {
	private static final String TAG = "QCLOGMediaPlayer";

	private Context 	m_context = null;
	private long 		m_NativeContext = 0;
	private Surface 	m_NativeSurface = null;
	private SurfaceView m_SurfaceView = null;

	private int m_nStreamNum = 0;
	private int m_nStreamPlay = -1;
	private int	m_nStreamBitrate = 0;

	private int m_nVideoWidth = 0;
	private int m_nVideoHeight = 0;
	private int m_nSampleRate = 0;
	private int m_nChannels = 0;
	private int m_nBTOffset = 0;

	private Object				m_pObjPlayer = null;
	private String				m_strApkPath = null;
	private String				m_strURL = null;
	private AudioRender			m_AudioRender = null;
	private onEventListener 	m_EventListener = null;
	private msgHandler 			m_hHandler = null;

	static {
		System.loadLibrary("QPlayer");
	}

	public int Init(Context context, String apkPath, int nFlag) {
		m_hHandler = new msgHandler();
		m_context = context;
		AudioManager am = (AudioManager)context.getSystemService(context.AUDIO_SERVICE);
		if (am != null && am.isBluetoothA2dpOn())
			m_nBTOffset = 250;
		m_strApkPath = apkPath;
		m_pObjPlayer = new WeakReference<MediaPlayer>(this);
		m_NativeContext = nativeInit(m_pObjPlayer, apkPath, nFlag);
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
		m_strURL = strPath;
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

	public long GetPos() {
		return nativeGetPos(m_NativeContext);
	}

	public int SetPos(long lPos) {
		return nativeSetPos(m_NativeContext, lPos);
	}

	public int GetParam(int nParamId, int nParam, Object objParam) {
		return nativeGetParam(m_NativeContext, nParamId, nParam, objParam);
	}

	public int SetParam(int nParamId, int nParam, Object objParam) {
		return nativeSetParam(m_NativeContext, nParamId, nParam, objParam);
	}

	public void Uninit() {
		nativeUninit(m_NativeContext);
		if (m_AudioRender != null)
			m_AudioRender.closeTrack();
		m_AudioRender = null;
		m_NativeContext = 0;
	}

	public int GetVideoWidth() {
		return m_nVideoWidth;
	}

	public int GetVideoHeight() {
		return m_nVideoHeight;
	}

	public void SetVolume(int nVolume) {
		SetParam (PARAM_PID_AUDIO_VOLUME, nVolume, null);
	}

	public int GetStreamNum() {
		if (m_nStreamNum == 0)
			GetParam (QCPLAY_PID_StreamNum, 0, this);
		return m_nStreamNum;

	}
	public int SetStreamPlay (int nStream) {
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
		SetParam (QCPLAY_PID_Clock_OffTime,  m_nBTOffset, null);
	}

	public void onVideoSizeChanged () {
		RelativeLayout.LayoutParams lp = (RelativeLayout.LayoutParams)m_SurfaceView.getLayoutParams();
		DisplayMetrics dm = m_context.getResources().getDisplayMetrics();
		if (m_nVideoWidth != 0 && m_nVideoHeight != 0)// && lp.width == LayoutParams.FILL_PARENT && lp.height == LayoutParams.FILL_PARENT)
		{
			int nMaxOutW = lp.width;
			if (nMaxOutW <= 0)
				nMaxOutW = dm.widthPixels;
			int nMaxOutH = lp.height;
			if (nMaxOutH <= 0)
				nMaxOutH = dm.heightPixels;

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
		if (what != BasePlayer.QC_MSG_SNKA_RENDER && what != BasePlayer.QC_MSG_SNKV_RENDER)
			Log.v("MediaPlayer", String.format("QC_MSG ID = 0X%08X   value %d   %d", what, ext1, ext2));

		MediaPlayer player = (MediaPlayer)((WeakReference)baselayer_ref).get();
		if (player == null) 
			return;
		
		if (what == QC_MSG_SNKV_NEW_FORMAT)
		{
			if (player.m_nVideoWidth == ext1 && player.m_nVideoHeight == ext2){
				player.SetParam (PARAM_PID_EVENT_DONE, 0, null);
				return;
			}

			player.m_nVideoWidth = ext1;
			player.m_nVideoHeight = ext2;
		}		
		else if (what == QC_MSG_SNKA_NEW_FORMAT)
		{
			player.m_nSampleRate = ext1;
			player.m_nChannels = ext2;

			if (player.m_nSampleRate == 0 && player.m_nChannels == 0){
				if (player.m_AudioRender != null)
					player.m_AudioRender.closeTrack();
				return;
			}

			if (player.m_AudioRender == null)
				player.m_AudioRender = new AudioRender(player.m_context, player);
			player.m_AudioRender.openTrack (player.m_nSampleRate, player.m_nChannels);
			return;
		}
		else if (what == QC_MSG_RTMP_METADATA) {
			if (player.m_EventListener != null)
				player.m_EventListener.onEvent(what, ext1, ext2, obj);
			return;
		}

		Message msg = player.m_hHandler.obtainMessage(what, ext1, ext2, obj);
		msg.sendToTarget();
	}
		
	private static void audioDataFromNative(Object baselayer_ref, byte[] data, int size, long lTime)
	{
		// Log.v("audioDataFromNative", String.format("Size %d  Time  %d", size, lTime));
		MediaPlayer player = (MediaPlayer)((WeakReference)baselayer_ref).get();
		if (player == null) 
			return;
		if (player.m_AudioRender != null)
			player.m_AudioRender.writeData(data,  size);
	}
	
	private static void videoDataFromNative(Object baselayer_ref, byte[] data, int size, long lTime, int nFlag)
	{
		Log.v("videoDataFromNative", String.format("Size %d  Time  %d, Flag   %x", size, lTime, nFlag));
		MediaPlayer player = (MediaPlayer)((WeakReference)baselayer_ref).get();
		if (player == null) 
			return;
		if (nFlag == QC_FLAG_Video_CaptureImage) {
			// Save the jpeg buff to file.
			player.m_EventListener.OnImage(data, size);
		}
		else if (nFlag == QC_FLAG_Video_SEIDATA) {
			// handle the SEI DATA.
			Log.v("videoDataFromNative SEI", String.format("Size %d  Time  %d, data   %d", size, lTime, data[0]));
		}
	}

	class msgHandler extends Handler {
		public msgHandler() {
		}
		@Override
		public void handleMessage(Message msg) {
			if (m_NativeContext == 0)
				return;
			if (msg.what == QC_MSG_VIDEO_HWDEC_FAILED) {
				nativeUninit(m_NativeContext);
				m_NativeContext = nativeInit(m_pObjPlayer, m_strApkPath, 0);
				if (m_NativeSurface != null)
					nativeSetView(m_NativeContext, m_NativeSurface);
				nativeOpen (m_NativeContext, m_strURL, 0);
				//return;
			}
			if (msg.what == QC_MSG_PLAY_OPEN_DONE) {
				OnOpenComplete ();
			}
			if (m_EventListener != null) {
				m_EventListener.onEvent(msg.what, msg.arg1, msg.arg2, msg.obj);
			}
			if (msg.what == QC_MSG_SNKV_NEW_FORMAT) {
				//onVideoSizeChanged ();
				SetParam(PARAM_PID_EVENT_DONE, 0, null);
			}
		}
	}

	// the native functions
    private native long	nativeInit(Object player, String apkPath, int nFlag);
    private native int 	nativeUninit(long nNativeContext);
    private native int 	nativeSetView(long nNativeContext, Object view);
    private native int 	nativeOpen(long nNativeContext,String strPath, int nFlag);
    private native int 	nativePlay(long nNativeContext);
    private native int 	nativePause(long nNativeContext);
    private native int 	nativeStop(long nNativeContext);
    private native long	nativeGetPos(long nNativeContext);
    private native int 	nativeSetPos(long nNativeContext,long lPos);
    private native long nativeGetDuration(long nNativeContext);
	private native int 	nativeGetParam(long nNativeContext,int nParamId, int nParam, Object objParam);
    private native int 	nativeSetParam(long nNativeContext,int nParamId, int nParam, Object objParam);
}
