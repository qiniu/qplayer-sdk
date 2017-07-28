/*******************************************************************************
	File:		Player view.java

	Contains:	the player UI implement file.

	Written by:	Fenger King

	Change History (most recent first):
	2014-01-05		Fenger			Create file

*******************************************************************************/
package com.qiniu.qplayer.qplayertest;

import java.util.Date;
import java.util.Timer;
import java.util.TimerTask;

import android.content.res.Configuration;
import android.net.Uri;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.os.Environment;

import java.io.File;
import java.io.FileOutputStream;

import android.app.Activity;
import android.util.Log;
import android.content.SharedPreferences;

import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.WindowManager;
import android.view.View;
import android.widget.Button;
import android.widget.RelativeLayout;
import android.widget.ImageButton;
import android.widget.SeekBar;
import android.widget.TextView;

import android.app.AlertDialog;
import android.content.DialogInterface;
import android.app.ProgressDialog;

import android.content.pm.ActivityInfo;

import com.qiniu.qplayer.mediaEngine.*;

public class PlayerView extends Activity 
						implements SurfaceHolder.Callback,
						BasePlayer.onEventListener {
	
	private static final int MSG_UPDATE_UI 	= 0X1001;
	
	private SurfaceView 	m_svVideo = null;
	private SurfaceHolder 	m_shVideo = null;
	private TextView 		m_txtSubTT = null;

	private RelativeLayout 	m_layButtons = null;
	private ImageButton		m_btnPlay = null;
	private ImageButton		m_btnPause = null;
	private SeekBar 		m_sbPlayer = null;
	private Button			m_btnStream = null;

	private boolean 		m_bActionMove = false;
	
	private String			m_strFile = null;
	private String			m_strNextFile1 = "http://mus-oss.muscdn.com/reg02/2017/07/06/14/247382630843777024.mp4";
	private String			m_strNextFile2 = "http://mus-oss.muscdn.com/reg02/2017/07/02/00/245712223036194816.mp4";
	private String			m_strNextFile3 = "http://musically.muscdn.com/reg02/2017/06/29/09/244762267827998720.mp4";
	private String			m_strNextFile4 = "http://musically.muscdn.com/reg02/2017/07/05/04/246872853734834176.mp4";
	private String			m_strNextFile5 = "http://musically.muscdn.com/reg02/2017/05/31/02/234148590598897664.mp4";
	private int				m_nPlayIndex = 0;

	private MediaPlayer		m_Player = null;
	private int				m_nDuration = 0;
	private int				m_nStreamPlay = -1;
	
	private Timer			m_tmPlay = null;
	private TimerTask		m_ttPlay = null;
	private Date 			m_dateShowTime = null;	
	private Handler 		m_handlerEvent = null;

	private AlertDialog.Builder 	m_dlgOK = null;
	private ProgressDialog 			m_dlgWait = null;
	private Activity				m_Activity = null;

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);     
        
		getWindow().setFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON,
				WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);

        setContentView(R.layout.activity_player_view);
        
        initControls ();
		
		Uri uri = getIntent().getData();
		if (uri != null) 
			m_strFile = uri.toString();
		m_Activity = this;
		Log.v ("PlayerView", m_strFile);
    }
    
	public void surfaceChanged(SurfaceHolder surfaceholder, int format, int w, int h) 
	{
		Log.v ("PlayerView", "SurfaceChanged  Format: " + format + "  Width = " + w + "  Height = " + h);
	//	if (m_Player != null)
	//		m_Player.SetView(m_svVideo);
	}

	public void surfaceCreated(SurfaceHolder surfaceholder) 
	{		
		Log.v ("PlayerView", "SurfaceCreated");
		if (m_Player != null) 
		{
			m_Player.SetView(m_svVideo);

			if (m_Player.GetDuration() <= 0)
				m_Player.SetParam(BasePlayer.QCPLAY_PID_Disable_Video, 0, null);
			else
				m_Player.Play();

			m_btnPause.setVisibility(View.VISIBLE);
			m_btnPlay.setVisibility(View.INVISIBLE);
			m_layButtons.setVisibility(View.VISIBLE);
			m_btnStream.setVisibility(View.VISIBLE);
			return;
		}
		
		OpenFile (m_strFile);
		m_dateShowTime = new Date(System.currentTimeMillis());
		showControls ();		
	}

	public void surfaceDestroyed(SurfaceHolder surfaceholder) 
	{	
		Log.v ("PlayerView", "surfaceDestroyed");
		if (m_Player != null)
			m_Player.SetView(null);
	}

	@Override
	public void onConfigurationChanged(Configuration newConfig) {
		super.onConfigurationChanged(newConfig);
		if(m_Player != null)
			m_Player.onVideoSizeChanged ();
	}
	// @Override
	public int onEvent(int nID, int nArg1, int nArg2, Object obj) {
		if (nID == BasePlayer.QC_MSG_PLAY_OPEN_DONE) {

		//	m_Player.SetParam(BasePlayer.QCPLAY_PID_SendOut_AudioBuff, 0, null);
		//	m_Player.SetParam(BasePlayer.QCPLAY_PID_SendOut_VideoBuff, 0, null);

			m_nDuration = (int)m_Player.GetDuration();
			m_Player.Play ();
		}
		else if (nID == BasePlayer.QC_MSG_SNKV_NEW_FORMAT) {
			if (nArg1 == 0 || nArg2 == 0)
				return 0;
			if (nArg1 > nArg2) {
				if(getRequestedOrientation()!=ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE){
					setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);
				}
			}
			else{
				if(getRequestedOrientation()!=ActivityInfo.SCREEN_ORIENTATION_PORTRAIT){
					setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_PORTRAIT);
				}
			}
		}
		else if (nID == BasePlayer.QC_MSG_PLAY_DURATION)
			m_nDuration = (int)m_Player.GetDuration();
		else if (nID == BasePlayer.QC_MSG_PLAY_OPEN_FAILED) {
			m_dlgOK = new AlertDialog.Builder(this);
			m_dlgOK.setTitle("Error");
			m_dlgOK.setMessage("Open file failed!");
			m_dlgOK.setPositiveButton("OK",
					new DialogInterface.OnClickListener() {
						@Override
						public void onClick(DialogInterface dialog, int which) {
							Close ();
						}
					});
			m_dlgOK.show();
			m_dlgOK = null;
		}
		else if (nID == BasePlayer.QC_MSG_PLAY_COMPLETE)
			Close ();
		else if (nID == BasePlayer.QC_MSG_PLAY_SEEK_DONE || nID == BasePlayer.QC_MSG_PLAY_SEEK_FAILED) {
			if (m_dlgWait != null) {
				m_dlgWait.cancel();
				m_dlgWait = null;
			}
		}
		else if (nID == BasePlayer.QC_MSG_HTTP_DISCONNECTED) {
			if (m_dlgOK != null)
				return 0;
			m_dlgOK = new AlertDialog.Builder(this);
			m_dlgOK.setTitle("Error");
			m_dlgOK.setMessage("The network disconnected!");
			m_dlgOK.setPositiveButton("OK",
					new DialogInterface.OnClickListener() {
						@Override
						public void onClick(DialogInterface dialog, int which) {

						}
					}
				);
			m_dlgOK.show();
			m_dlgOK = null;
		}
		else if (nID == BasePlayer.QC_MSG_HTTP_RECONNECT_FAILED) {
			if (m_dlgOK != null)
				return 0;
			m_dlgOK = new AlertDialog.Builder(this);
			m_dlgOK.setTitle("Error");
			m_dlgOK.setMessage("The network reconnect failed!");
			m_dlgOK.setPositiveButton("OK",
					new DialogInterface.OnClickListener() {
						@Override
						public void onClick(DialogInterface dialog, int which) {

						}
					}
				);
			m_dlgOK.show();
			m_dlgOK = null;
		}
		else if (nID == BasePlayer.QC_MSG_HTTP_RECONNECT_SUCESS) {
			return 0;
		}
		else if (nID == BasePlayer.QC_MSG_SNKV_FIRST_FRAME) {
			if (m_dlgWait != null) {
				m_dlgWait.cancel();
				m_dlgWait = null;
			}
		}
		else if (nID == BasePlayer.QC_MSG_RTMP_METADATA) {
			String strMetaData = (String) obj;
			Log.v("PlayerView  MetaData *** ", strMetaData);
		}

		return 0;
	}
	
	public int OnSubTT (String strText, long lTime) {
		m_txtSubTT.setText(strText);
		if (lTime >= 0) {
			m_txtSubTT.setText(strText);
			m_txtSubTT.setVisibility(View.VISIBLE);
		} else {
			m_txtSubTT.setText("");		
			m_txtSubTT.setVisibility(View.INVISIBLE);			
		}
		return 0;
	}

	public int OnImage (byte[] pData, int nSize) {
		try{
			File parent_path = Environment.getExternalStorageDirectory();
			File dir = new File(parent_path.getAbsoluteFile(), "CaptureVideo");
			dir.mkdir();
			File file = new File(dir.getAbsoluteFile(), "0001.jpg");
			file.createNewFile();
			FileOutputStream fos = new FileOutputStream(file);
			fos.write(pData, 0, pData.length);
			fos.flush();
			fos.close();
		} catch(Exception e){
			e.printStackTrace();
		}
		return 0;
	}

	@Override
	public boolean onKeyDown(int keyCode, KeyEvent event) {
		if (keyCode == KeyEvent.KEYCODE_BACK) 
			Close ();

		return super.onKeyDown(keyCode, event);
	}	
	
	@Override
	public boolean onTouchEvent(MotionEvent event) {
		int nAction = event.getAction();
		switch (nAction) {
			case MotionEvent.ACTION_DOWN:
				return true;

			case MotionEvent.ACTION_MOVE:
				if (m_bActionMove == true)
					return false;
				m_bActionMove = true;
				if (m_Player != null) {
					if (m_nPlayIndex == 0)
						m_Player.Open(m_strNextFile1, BasePlayer.QCPLAY_OPEN_SAME_SOURCE);
					else if (m_nPlayIndex == 1)
						m_Player.Open(m_strNextFile2, BasePlayer.QCPLAY_OPEN_SAME_SOURCE);
					else if (m_nPlayIndex == 2)
						m_Player.Open(m_strNextFile3, BasePlayer.QCPLAY_OPEN_SAME_SOURCE);
					else if (m_nPlayIndex == 3)
						m_Player.Open(m_strNextFile4, BasePlayer.QCPLAY_OPEN_SAME_SOURCE);
					else if (m_nPlayIndex == 4)
						m_Player.Open(m_strNextFile5, BasePlayer.QCPLAY_OPEN_SAME_SOURCE);
					}
				m_nPlayIndex++;
				if (m_nPlayIndex >= 5)
					m_nPlayIndex = 0;
				//  m_Player.SetParam (BasePlayer.QCPLAY_PID_Capture_Image, 0, null);
				return true;

			case MotionEvent.ACTION_UP:
				if (m_bActionMove == true) {
					m_bActionMove = false;
					return true;
				}
				m_dateShowTime = new Date(System.currentTimeMillis());
				if (m_layButtons.getVisibility() != View.VISIBLE) {
					showControls ();
				} else {
					hideControls ();
				}
				break;

			default:
				break;
		}

		return super.onTouchEvent(event);
	}	
	
	@Override
	protected void onPause() 
	{
		super.onPause();
		Log.v("PlayerView", "Player onPause");		
		if (m_Player != null) 
		{
			if (m_Player.GetDuration() <= 0) {
				m_Player.SetParam(BasePlayer.QCPLAY_PID_Disable_Video, 1, null);
			}  else {
				m_Player.SetPos(m_Player.GetPos());
				m_Player.Pause();
				m_btnPause.setVisibility(View.INVISIBLE);
				m_btnPlay.setVisibility(View.VISIBLE);
				m_layButtons.setVisibility(View.VISIBLE);
			}
		}		
	}
	
	@Override
	protected void onDestroy() {
		hideControls ();
		if (m_shVideo != null) {
			m_shVideo.removeCallback(this);
			m_shVideo = null;
		}
		super.onDestroy();
	}
	
	private void OpenFile (String strPath) {
		SharedPreferences settings = this.getSharedPreferences("Player_Setting", 0);
		int nDownloadFile  = settings.getInt("DownloadFile", 0);
		int nColorType     = settings.getInt("ColorType", 0);
		int nVideoDec      = settings.getInt("VideoDec", 1);
		int	nFlag = 0;
		if (nVideoDec == 3)
			nFlag = BasePlayer.QCPLAY_OPEN_VIDDEC_HW;

		int nRet;
		m_Player = new MediaPlayer();

		String apkPath = "/data/data/" + this.getPackageName() + "/lib/";
		m_Player.SetView(m_svVideo);
		nRet = m_Player.Init(this, apkPath, nFlag);
		if (nRet != 0) {
			Close ();
			return;
		}
				
		m_Player.setEventListener(this);
		//String 	strKeyText = "kdnljjlcn2iu2384";
		//byte[]  byKeyText = {0x6b, 0x64, 0x6e, 0x6c, 0x6a, 0x6a, 0x6c, 0x63, 0x6e, 0x32, 0x69, 0x75, 0x32, 0x33, 0x38, 0x34 };
		//m_Player.SetParam(BasePlayer.QCPLAY_PID_Speed, 0, byKeyText);

		if (nDownloadFile > 0) {
			m_Player.SetParam(BasePlayer.QCPLAY_PID_Prefer_Protocol, BasePlayer.QC_IOPROTOCOL_HTTPPD, null);
			File file = Environment.getExternalStorageDirectory();
			String strPDPath = file.getPath() + "/QPlayer/PDFile";
			m_Player.SetParam(BasePlayer.QCPLAY_PID_PD_Save_Path, 0, strPDPath);
		}

		nRet = m_Player.Open (strPath, 	0);
		if (nRet != 0) {
			Close ();		
			return;
		}
		//showWaitDialog ();
	}	
	
	private void Close () {
		if (m_Player != null) {
			m_Player.Stop();
			m_Player.Uninit();
			m_Player = null;
		}	
		
		finish ();
	}
	
	private void initControls (){
		m_svVideo = (SurfaceView) findViewById(R.id.svVideo);
		m_shVideo = m_svVideo.getHolder();
		m_shVideo.addCallback(this);

		m_txtSubTT = (TextView) findViewById(R.id.tvSubTT);
		m_layButtons = (RelativeLayout) findViewById(R.id.layControls);
		m_btnPlay = (ImageButton) findViewById (R.id.btnPlay);
		m_btnPause = (ImageButton) findViewById (R.id.btnPause);
		m_sbPlayer = (SeekBar) findViewById (R.id.sbPlayer);
		m_btnStream = (Button) findViewById (R.id.btnStreams);
		
		m_btnPlay.setVisibility(View.INVISIBLE);
	//	m_layButtons.setVisibility(View.INVISIBLE);
		m_txtSubTT.setVisibility(View.INVISIBLE);
		m_btnStream.setVisibility(View.VISIBLE);
		
		m_btnPlay.setOnClickListener(new View.OnClickListener() {
			public void onClick(View v) {
				if (m_Player != null)
					m_Player.Play();
				m_btnPlay.setVisibility(View.INVISIBLE);
				m_btnPause.setVisibility(View.VISIBLE);
			}
		});
		
		m_btnPause.setOnClickListener(new View.OnClickListener() {
			public void onClick(View v) {
				//if (m_Player != null) {
				//	m_Player.Open(m_strFile, BasePlayer.QCPLAY_OPEN_SAME_SOURCE);
				//  m_Player.SetParam (BasePlayer.QCPLAY_PID_Capture_Image, 0, null);
				//	return;
				//}
				if (m_Player != null)
					m_Player.Pause();
				m_btnPause.setVisibility(View.INVISIBLE);
				m_btnPlay.setVisibility(View.VISIBLE);
			}
		});

		m_btnStream.setOnClickListener(new View.OnClickListener() {
			public void onClick(View v) {
				showSingleChoiceDialog ();
			}
		});

		m_sbPlayer.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
			public void onStopTrackingTouch(SeekBar seekBar) {
				int nPos = seekBar.getProgress() * (m_nDuration / 100);
				if (m_Player != null) {
					if (m_Player.SetPos(nPos) != 0)
						return;
					m_dlgWait = new ProgressDialog(m_Activity);
					m_dlgWait.setProgressStyle(ProgressDialog.STYLE_SPINNER);
					m_dlgWait.setMessage("wait...");
					m_dlgWait.setIndeterminate(true);
					m_dlgWait.setCancelable(false);
					m_dlgWait.show();
				}
			}
			
			public void onStartTrackingTouch(SeekBar seekBar) {
			}

			public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
			}
		});	
		
		m_handlerEvent = new Handler() 
		{
			public void handleMessage(Message msg) 
			{
				if (m_Player == null)
					return;

				if (msg.what == MSG_UPDATE_UI) 
				{			
					Date dateNow = new Date(System.currentTimeMillis());
					long timePeriod = (dateNow.getTime() - m_dateShowTime.getTime()) / 1000;
					if (timePeriod >= 10)
						hideControls ();
					
					if (m_nDuration > 0) {
						int nPos = (int)(m_Player.GetPos() / (m_nDuration / 100));
						m_sbPlayer.setProgress(nPos);
					}
				} 
			}
		};		
	}
	
	private void showControls (){
		m_layButtons.setVisibility(View.VISIBLE);
		m_btnStream.setVisibility(View.VISIBLE);
		if (m_ttPlay != null)
			m_ttPlay = null;
		m_ttPlay = new TimerTask() {
			public void run() {
				m_handlerEvent.sendEmptyMessage(MSG_UPDATE_UI);
			}
		};			
		
		if (m_tmPlay == null)
			m_tmPlay = new Timer ();
		
		m_tmPlay.schedule(m_ttPlay, 0, 1000);		
	}
	
	private void hideControls () {
		if (m_tmPlay != null) {
			m_tmPlay.cancel();
			m_tmPlay.purge();
			m_tmPlay = null;
			m_ttPlay = null;
		}
		m_btnStream.setVisibility(View.INVISIBLE);
		m_layButtons.setVisibility(View.INVISIBLE);		
	}

	private void showWaitDialog () {
		m_dlgWait = new ProgressDialog(this);
		m_dlgWait.setProgressStyle(ProgressDialog.STYLE_SPINNER);
		m_dlgWait.setMessage("wait...");
		m_dlgWait.setIndeterminate(false);
		m_dlgWait.setCancelable(true);
		m_dlgWait.show();
	}

	private void showSingleChoiceDialog(){
		int	nStreamNum = m_Player.GetStreamNum();
		if (nStreamNum <= 0)
			return;
		final String[] 	strStreams = new String[nStreamNum + 1];
		strStreams[0] = "Auto Play";
		for (int i = 1; i <= nStreamNum; i++){
			strStreams[i] = "Stream " + i + " - " + m_Player.GetStreamBitrate(i -1);
		}
		m_nStreamPlay = m_Player.GetStreamPlay();
		AlertDialog.Builder singleChoiceDialog  = new AlertDialog.Builder(this);
		singleChoiceDialog.setTitle("Select Stream");
		singleChoiceDialog.setSingleChoiceItems(strStreams, m_nStreamPlay + 1,
				new DialogInterface.OnClickListener() {
					@Override
					public void onClick(DialogInterface dialog, int which) {
						m_nStreamPlay = which - 1;
					}
				});
		singleChoiceDialog.setPositiveButton("Select",
				new DialogInterface.OnClickListener() {
					@Override
					public void onClick(DialogInterface dialog, int which) {
					m_Player.SetStreamPlay(m_nStreamPlay);
					}
				});
		singleChoiceDialog.show();
	}
}
