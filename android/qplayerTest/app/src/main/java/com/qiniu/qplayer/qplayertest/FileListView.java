package com.qiniu.qplayer.qplayertest;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.HashMap;
import java.util.Map;
import android.content.pm.PackageManager;

import android.net.Uri;
import android.os.Bundle;
import android.os.Environment;
import android.app.Activity;
import android.content.Intent;
import android.view.Menu;
import android.view.MenuItem;
import android.view.KeyEvent;
import android.view.View;
import android.widget.AdapterView;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ListView;
import android.widget.SimpleAdapter;

public class FileListView extends AppCompatActivity {
    private String							m_strRootPath = null;
    static private String					m_strListPath = null;
    private ListView						m_lstFiles = null;
    private EditText                        m_txtURL = null;
    private Button                          m_btnPlay = null;
    private AdapterView.OnItemClickListener m_lvListener = null;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_file_list_view);

        initListView ();

        m_strRootPath = "/";
        //m_strListPath = "/mnt/sdcard/";
        if (m_strListPath == null) {
            File file = Environment.getExternalStorageDirectory();
            m_strListPath = file.getPath();
            updateFileList (m_strListPath);
        } else {
            updateFileList (m_strListPath);
        }

        String  strVer = "V1.0";
        int     bldVer = 1;
        try {
        strVer = this.getPackageManager().getPackageInfo(getPackageName(), 0).versionName;
        bldVer = this.getPackageManager().getPackageInfo(getPackageName(), 0).versionCode;
        } catch (PackageManager.NameNotFoundException e) {
        }
        strVer = "corePlayer V" + strVer + "  B " + bldVer;
        super.setTitle(strVer);
    }

    @Override
    public boolean onKeyDown(int keyCode, KeyEvent event) {
        if (keyCode == KeyEvent.KEYCODE_BACK)
        {
        /*
            String strFile = "http://ojpjb7lbl.bkt.clouddn.com/bipbopall.m3u8";
            Intent intent = new Intent(FileListView.this, PlayerView.class);
            intent.setData(Uri.parse(strFile));
            startActivity(intent);
            return true;
        */

            if (m_strListPath.equals(m_strRootPath)) {
                //finish ();
                System.exit(0);
            }
            else {
                File file = new File (m_strListPath);
                m_strListPath = file.getParent();
                updateFileList (m_strListPath);
            }
            return true;

        }

        return super.onKeyDown(keyCode, event);
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.activity_file_list_view, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // TODO Auto-generated method stub
        int id = item.getItemId();
        switch (id) {
            case R.id.menu_settings:
                startActivity(new Intent(this, SettingView.class));
                break;
            case R.id.menu_exit:
                finish();
                System.exit(RESULT_OK);
                break;
            default:
                break;
        }
        return super.onOptionsItemSelected(item);
    }

    private void initListView () {
        m_lvListener = new AdapterView.OnItemClickListener() {
            @Override
            public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
                SimpleAdapter adapter = (SimpleAdapter)m_lstFiles.getAdapter();
                Map<String, Object> map = (Map<String, Object>)adapter.getItem(position);
                String  strFile = (String) map.get("path");
                String	strDir = (String) map.get("dir");

                if (strDir.equals("1")) {
                    updateFileList (strFile);
                } else {
                    String strExt = strFile.substring(strFile.length() - 4);
                    if (strExt.equalsIgnoreCase(".url")) {
                        updateURLList (strFile);
                    } else {
                        Intent intent = new Intent(FileListView.this, PlayerView.class);
                        intent.setData(Uri.parse(strFile));
                        startActivity(intent);
                    }
                }
            }
        };
        m_lstFiles = (ListView)findViewById (R.id.listViewFile);
        m_lstFiles.setOnItemClickListener(m_lvListener);
        m_txtURL = (EditText)findViewById (R.id.editTextURL);
        m_btnPlay = (Button)findViewById (R.id.buttonPlay);

        m_btnPlay.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
            String strURL = m_txtURL.getText ().toString();
            Intent intent = new Intent(FileListView.this, PlayerView.class);
            intent.setData(Uri.parse(strURL));
            startActivity(intent);
            }
        });
    }

    private void updateURLList (String strFile) {
        m_strListPath = m_strListPath + "/url";
        ArrayList<Map<String, Object>> list = new ArrayList<Map<String, Object>>();
        HashMap<String, Object> map;

        try {
            FileInputStream fis = new FileInputStream (strFile);
            BufferedReader br = new BufferedReader(new InputStreamReader(fis));
            String line = null;
            while((line = br.readLine())!=null)
            {
                map = new HashMap<String, Object>();
                map.put("name", line);
                map.put("path", line);
                map.put("img", R.drawable.item_video);
                map.put("dir", "2");
                list.add(map);
            }
        }catch (Exception e) {
            e.printStackTrace();
        }

        SimpleAdapter adapter = new SimpleAdapter(this, list, R.layout.list_item,
                new String[]{"name","img"}, new int[]{R.id.name, R.id.img});
        m_lstFiles.setAdapter(adapter);
    }

    private void updateFileList (String strPath){
        m_strListPath = strPath;

        ArrayList<Map<String, Object>> list = getFileList(strPath);
        SimpleAdapter adapter = new SimpleAdapter(this, list, R.layout.list_item,
                new String[]{"name","img"}, new int[]{R.id.name, R.id.img});

        Comparator comp = new nameComparator();
        Collections.sort(list, comp);

        m_lstFiles.setAdapter(adapter);
    }

    private ArrayList<Map<String, Object>> getFileList(String strPath) {
        ArrayList<Map<String, Object>> list = new ArrayList<Map<String, Object>>();
        File fPath = new File(strPath);
        File[] fList = fPath.listFiles();

        HashMap<String, Object> map;
        if (fList != null) {
            for (int i = 0; i < fList.length; i++)
            {
                File file = fList[i];
                if (file.isHidden())
                    continue;
                if (file.isDirectory()){
                    File fSubPath = new File(file.getPath());
                    File[] fSubList = fSubPath.listFiles();
                    if (fSubList != null) {
                        if (fSubList.length <= 0)
                            continue;
                    }
                }

                map = new HashMap<String, Object>();
                map.put("name", file.getName());
                map.put("path", file.getPath());
                if (file.isDirectory()){
                    map.put("img", R.drawable.item_folder);
                    map.put("dir", "1");
                }
                else {
                    map.put("img", R.drawable.item_video);
                    map.put("dir", "0");
                }
                list.add(map);
            }
        }
        return list;
    }

    public class nameComparator implements Comparator<Object> {
        @SuppressWarnings("unchecked")
        public int compare(Object o1, Object o2) {
            HashMap<String, Object> p1 = (HashMap<String, Object>) o1;
            HashMap<String, Object> p2 = (HashMap<String, Object>) o2;
            String strName1, strName2, strDir1, strDir2;
            strName1 = (String) p1.get("name");
            strName2 = (String) p2.get("name");
            strDir1 = (String) p1.get("dir");
            strDir2 = (String) p2.get("dir");

            if (strDir1.equals("1") && strDir2.equals("0") ){
                return -1;
            } else if (strDir1.equals("0") && strDir2.equals("1") ){
                return 1;
            } else {
                if (strName1.compareToIgnoreCase(strName2) > 0) {
                    return 1;
                } else {
                    return -1;
                }
            }
        }
    }
}
