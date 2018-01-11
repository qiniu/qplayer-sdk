/*******************************************************************************
	File:		AudioRener.java

	Contains:	Audio data render implement file.

	Written by:	Fenger King

	Change History (most recent first):
	2013-09-28		Fenger			Create file

*******************************************************************************/
package com.qiniu.qplayer.mediaEngine;

import android.content.Context;
import android.media.AudioFormat;
import android.media.AudioManager;
import android.media.AudioTrack;
import android.util.Log;

public class AudioRender {
	private static final String TAG = "YYLOGAudioRender";
	
	public static final int PARAM_PID_AUDIO_OFFSEST		= 0X300;
		
	private BasePlayer		mPlayer = null;
	private AudioTrack 		mAudioTrack = null;
	private int				mOffset = 0;

	public AudioRender(Context context, BasePlayer player) {
		mPlayer = player;
		mAudioTrack = null;
		
		AudioManager am = (AudioManager)context.getSystemService(context.AUDIO_SERVICE);
		if (am != null && am.isBluetoothA2dpOn())
			mOffset = 250;
		
		Log.v (TAG, "Audio Manager, offset  " + mOffset);
	}
	
	public int openTrack(int sampleRate, int channelCount) 
	{
		if (mAudioTrack != null)
			closeTrack();

		int nChannelConfig = (channelCount == 1) ? AudioFormat.CHANNEL_CONFIGURATION_MONO : AudioFormat.CHANNEL_CONFIGURATION_STEREO;
		int nMinBufSize = AudioTrack.getMinBufferSize(sampleRate, nChannelConfig, AudioFormat.ENCODING_PCM_16BIT);
		if (nMinBufSize == AudioTrack.ERROR_BAD_VALUE || nMinBufSize == AudioTrack.ERROR)
			return -1;

		nMinBufSize = nMinBufSize * 2;

		if (nMinBufSize < 8192)
			nMinBufSize = 8192;
		Log.v (TAG, "Audio track, nMinBufSize  " + nMinBufSize);		
		
		mAudioTrack = new AudioTrack(AudioManager.STREAM_MUSIC, sampleRate,
									nChannelConfig, AudioFormat.ENCODING_PCM_16BIT, nMinBufSize,
									AudioTrack.MODE_STREAM);
				
		int param = nMinBufSize * 1000 / (sampleRate * channelCount * 2);
		// param = param + mOffset;
		if(mPlayer!=null) {
			if (mOffset > 0)
				param = param | 0X80000000;
			mPlayer.SetParam (PARAM_PID_AUDIO_OFFSEST,  param, null);	
		}

		mAudioTrack.play();
				
		return 0;
	}	
	
	public long writeData(byte[] audioData, long nSize) 
	{		
		if (mAudioTrack != null && nSize > 0) 
			mAudioTrack.write(audioData, 0, (int) nSize);

		return 0;
	}	
	
	public void closeTrack() {
		if (mAudioTrack != null) {
			mAudioTrack.stop();
			mAudioTrack.release();
			mAudioTrack = null;
		}
	}	
	
	public void SetVolume(float left, float right) {
		if (mAudioTrack != null)
			mAudioTrack.setStereoVolume(left, right);
	}
}
