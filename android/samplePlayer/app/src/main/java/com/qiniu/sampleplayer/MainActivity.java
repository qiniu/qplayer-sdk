package com.qiniu.sampleplayer;

import android.os.Environment;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.content.pm.PackageManager;
import android.os.Handler;
import android.os.Message;
import android.util.DisplayMetrics;
import android.view.KeyEvent;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ListView;
import android.widget.RelativeLayout;
import android.widget.ArrayAdapter;
import android.widget.SeekBar;
import android.widget.TextView;
import android.widget.Toast;

import java.io.File;
import java.util.Timer;
import java.util.TimerTask;

import com.qiniu.qplayer.mediaEngine.*;

public class MainActivity extends AppCompatActivity
        implements SurfaceHolder.Callback, BasePlayer.onEventListener {

    private static final int    MSG_UPDATE_UI  	= 0X1002;

    private SurfaceView         m_svVideo = null;
    private SurfaceHolder       m_shVideo = null;
    private TextView            m_txtPos = null;
    private TextView            m_txtDur = null;
    private SeekBar             m_sbPos = null;
    private RelativeLayout      m_layVideo = null;
    private ListView            m_lstFiles = null;
    private AdapterView.OnItemClickListener m_lvListener = null;

    private MediaPlayer         m_Player = null;
    private int                 m_nViewWidth = 0;
    private int                 m_nViewHeight = 0;

    private int                 m_nDuration = 0;

    private Timer               m_tmPlay = null;
    private TimerTask           m_ttPlay = null;
    private uiHandler           m_handlerEvent = null;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        initListView();
    }

    public void surfaceCreated(SurfaceHolder surfaceholder) {
        if (m_Player != null)
            m_Player.SetView(surfaceholder.getSurface());
        else
            InitPlayer();
    }

    public void surfaceChanged(SurfaceHolder surfaceholder, int format, int w, int h) {
    }

    public void surfaceDestroyed(SurfaceHolder surfaceholder) {
        if (m_Player != null)
            m_Player.SetView(null);
    }

    public int onEvent(int nID, int nArg1, int nArg2, Object obj) {
        if (m_Player == null)
            return 0;

        if (nID == BasePlayer.QC_MSG_PLAY_OPEN_DONE) {
            m_Player.Play();
        } else if (nID == BasePlayer.QC_MSG_SNKV_NEW_FORMAT) {
            UpdateSurfaceViewPos(nArg1, nArg2);
        } else if (nID == BasePlayer.QC_MSG_PLAY_OPEN_FAILED) {
            Toast.makeText(this, "打开文件失败了！", Toast.LENGTH_SHORT).show();
        } else if (nID == BasePlayer.QC_MSG_PLAY_COMPLETE) {
            m_Player.SetPos(0);
            //m_Player.Open(m_strFile, 0);
        } else if (nID == BasePlayer.QC_MSG_PLAY_DURATION) {
            m_nDuration = (int)m_Player.GetDuration();
            String strDur = new String();
            strDur = strDur.format ("%02d:%02d", m_nDuration / 60000, (m_nDuration % 60000) / 1000);
            m_txtDur.setText (strDur);
        } else if (nID == BasePlayer.QC_MSG_PLAY_SEEK_DONE || nID == BasePlayer.QC_MSG_PLAY_SEEK_FAILED) {
        } else if (nID == BasePlayer.QC_MSG_HTTP_DISCONNECTED) {
        } else if (nID == BasePlayer.QC_MSG_HTTP_RECONNECT_FAILED) {
        } else if (nID == BasePlayer.QC_MSG_HTTP_RECONNECT_SUCESS) {
            return 0;
        } else if (nID == BasePlayer.QC_MSG_SNKV_FIRST_FRAME) {
        } else if (nID == BasePlayer.QC_MSG_RTMP_METADATA) {
        }

        return 0;
    }

    public int OnRevData(byte[] pData, int nSize, int nFlag) {
        return 0;
    }

    private void InitPlayer() {
        int nFlag = 0;
        //nFlag = BasePlayer.QCPLAY_OPEN_VIDDEC_HW;
        int nRet = 0;
        if (m_Player == null) {
            m_Player = new MediaPlayer();
            m_Player.SetView(m_svVideo.getHolder().getSurface());
            nRet = m_Player.Init(this, nFlag);
            if (nRet != 0) {
                Close();
                return;
            }

            String strDnsServer = "114.114.114.114";    // UDP dns parser
            //String strDnsServer = "127.0.0.1";        // local system dns parser
            //m_Player.SetParam(BasePlayer.QCPLAY_PID_DNS_SERVER, 0, strDnsServer);
            String strHost = "video.qiniu.3tong.com";
            // m_Player.SetParam(BasePlayer.QCPLAY_PID_DNS_DETECT, 0, strHost);
            //m_Player.SetParam(BasePlayer.QCPLAY_PID_Playback_Loop, 1, null);

            //m_Player.SetParam(BasePlayer.QCPLAY_PID_Prefer_Protocol, BasePlayer.QC_IOPROTOCOL_HTTPPD, null);
            File file = Environment.getExternalStorageDirectory();
            String strPDPath = file.getPath() + "/QPlayer/PDFile";
            m_Player.SetParam(BasePlayer.QCPLAY_PID_PD_Save_Path, 0, strPDPath);

        }
        m_Player.setEventListener(this);
    }

    private void Close() {
        if (m_Player != null) {
            m_Player.Stop();
            m_Player.Uninit();
            m_Player = null;
        }
    }

    private void UpdateSurfaceViewPos(int nW, int nH) {
        if (nW == 0 || nH == 0)
            return;
        RelativeLayout.LayoutParams lpView = (RelativeLayout.LayoutParams) m_svVideo.getLayoutParams();
        RelativeLayout.LayoutParams lpLayout = (RelativeLayout.LayoutParams) m_layVideo.getLayoutParams();
        if (m_nViewWidth == 0 || m_nViewHeight == 0) {
            DisplayMetrics dm = this.getResources().getDisplayMetrics();
            m_nViewWidth = lpLayout.width;
            m_nViewHeight = lpLayout.height;
            if (m_nViewWidth <= 0)
                m_nViewWidth = dm.widthPixels;
            if (m_nViewHeight <= 0)
                m_nViewHeight = dm.heightPixels;
        }
        if (m_nViewWidth * nH > nW * m_nViewHeight) {
            lpView.height = m_nViewHeight;
            lpView.width = nW * m_nViewHeight / nH;
        } else {
            lpView.width = m_nViewWidth;
            lpView.height = nH * m_nViewWidth / nW;
        }
        m_svVideo.setLayoutParams(lpView);
    }

    @Override
    public boolean onKeyDown(int keyCode, KeyEvent event) {
        if (keyCode == KeyEvent.KEYCODE_BACK) {
            Close();
        }
        return super.onKeyDown(keyCode, event);
    }

    private void initListView() {
        m_lvListener = new AdapterView.OnItemClickListener() {
            @Override
            public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
                ArrayAdapter adapter = (ArrayAdapter) m_lstFiles.getAdapter();
                String strFile = (String) adapter.getItem(position).toString();
                int nFlag = 0;
                //nFlag = BasePlayer.QCPLAY_OPEN_SAME_SOURCE;
                m_Player.Open(strFile, nFlag);
            }
        };
        m_lstFiles = (ListView) findViewById(R.id.listViewFile);
        m_lstFiles.setOnItemClickListener(m_lvListener);

        m_svVideo = (SurfaceView) findViewById(R.id.svVideo);
        m_shVideo = m_svVideo.getHolder();
        m_shVideo.addCallback(this);

        m_txtPos = (TextView) findViewById(R.id.txtPos);
        m_txtPos.setText("00:00");
        m_sbPos = (SeekBar) findViewById(R.id.sbPos);
        m_txtDur = (TextView) findViewById(R.id.txtDur);
        m_txtDur.setText("00:00");

        m_layVideo = (RelativeLayout) findViewById(R.id.videoView);

        String[] strFiles = new String[]{
                "http://video.qiniu.3tong.com/720_201883248781950976.mp4",
                "http://video.qiniu.3tong.com/720_182584969019785216.mp4",
                "http://video.qiniu.3tong.com/720_179737636708024320.mp4",
                "http://video.qiniu.3tong.com/720_188810429944823808.mp4",
                "http://oh4yf3sig.cvoda.com/cWEtZGlhbmJvdGVzdDpY54m56YGj6ZifLumfqeeJiC5IRGJ1eHVhbnppZG9uZzAwNC5tcDQ=_q00030002.mp4"
        };
        m_lstFiles.setAdapter(new ArrayAdapter<String>(this, android.R.layout.simple_list_item_1, strFiles));

        m_handlerEvent = new uiHandler ();
        m_sbPos.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            public void onStopTrackingTouch(SeekBar seekBar) {
                int nPos = seekBar.getProgress() * (m_nDuration / 100);
                if (m_Player != null) {
                    m_Player.SetPos(nPos);
                }
            }
            public void onStartTrackingTouch(SeekBar seekBar) {}
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) { }
        });

        m_ttPlay = new TimerTask() {
            public void run() {
                m_handlerEvent.sendEmptyMessage(MSG_UPDATE_UI);
            }
        };
        m_tmPlay = new Timer();
        m_tmPlay.schedule(m_ttPlay, 1000, 500);
    }

    class uiHandler extends Handler {
        public uiHandler() {
        }

        public void handleMessage(Message msg) {
            if (m_Player == null || m_nDuration == 0)
                return;
            int nPos = (int) (m_Player.GetPos());
            if (nPos < 0)
                return;
            String strPos = new String();
            strPos = strPos.format("%02d:%02d", nPos / 60000, (nPos % 60000) / 1000);
            m_txtPos.setText(strPos);
            m_sbPos.setProgress(nPos / (m_nDuration / 100));
        }
    }
}
