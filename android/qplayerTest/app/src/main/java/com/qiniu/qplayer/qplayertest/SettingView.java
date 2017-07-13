
package com.qiniu.qplayer.qplayertest;

import java.util.ArrayList;
import java.util.List;


import android.app.Activity;
import android.content.SharedPreferences;
import android.content.res.Resources;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseExpandableListAdapter;
import android.widget.CheckBox;
import android.widget.ExpandableListView;
import android.widget.ExpandableListView.OnChildClickListener;
import android.widget.ExpandableListView.OnGroupClickListener;
import android.widget.ExpandableListView.OnGroupExpandListener;
import android.widget.ImageView;
import android.widget.TextView;

public class SettingView extends Activity implements OnGroupClickListener, OnChildClickListener,
        OnGroupExpandListener {
	public static final int 	POS_VIDEOQUALITY		= 0;
	public static final int 	POS_DOWNLOADFILE		= 1;
	public static final int 	POS_COLORTYPE			= 2;
	public static final int 	POS_VIDEODEC			= 3;
	
    LayoutInflater 			mInflater;
    List<List<String>> 		mLstGroup;
    List<List<String>> 		mLstChild;
    List<List<Boolean>>		mLstValue;
    
    ExpandableListView 				mListView;
    MyExpandableListViewAdapter 	mAdapter;
    private int 					m_nVideoQuality;
    private int 					m_nDownloadFile;
    private int 					m_nColorType;
    private int						m_nVideoDec;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        // TODO Auto-generated method stub
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_setting_view);
        this.setTitle("Setting");
        init();        
        loadDefaultValue();
    }

    private class MyExpandableListViewAdapter extends BaseExpandableListAdapter {
        @Override
        public Object getChild(int groupPosition, int childPosition) {
            // TODO Auto-generated method stub
            return mLstChild.get(groupPosition).get(childPosition);
        }

        @Override
        public long getChildId(int groupPosition, int childPosition) {
            // TODO Auto-generated method stub
            return childPosition;
        }

        @Override
        public View getChildView(int groupPosition, int childPosition, boolean isLastChild, View convertView, ViewGroup parent) {
            // TODO Auto-generated method stub
            String childItem = mLstChild.get(groupPosition).get(childPosition);   
            return generateChildView(childItem, convertView, groupPosition, childPosition);
        }

        private View generateChildView(String text, View convertView, int groupPosition, int childPosition) {
            if (null == convertView)                    
                convertView = mInflater.inflate(R.layout.set_item_checkbox, null);   
            TextView mTextView = (TextView) convertView.findViewById(R.id.title);
            mTextView.setText(text);
            CheckBox mBox = (CheckBox) convertView.findViewById(R.id.checkbox);
            mBox.setChecked(mLstValue.get(groupPosition).get(childPosition));
            return convertView;
        }

        private View generateGroupView(int groupPosition, String title, String summery, View convertView) {    	   
            if (null == convertView) 
                convertView = mInflater.inflate(R.layout.set_item, null);              
            ImageView mImageView = (ImageView) convertView.findViewById(R.id.icon);   
            switch(groupPosition) {
            case POS_VIDEOQUALITY:
            	mImageView.setImageResource(R.drawable.set_videoquality);
            	break;
            case POS_DOWNLOADFILE:
            	mImageView.setImageResource(R.drawable.set_subtitle);
            	break;
            case POS_COLORTYPE:
            	mImageView.setImageResource(R.drawable.set_color);
            	break;     
            case POS_VIDEODEC:
            	mImageView.setImageResource(R.drawable.set_subtitle);
            	break;  	
            }
            
            TextView mTitle = (TextView) convertView.findViewById(R.id.title);
            mTitle.setText(title);
            TextView mSummery = (TextView) convertView.findViewById(R.id.summary);
            mSummery.setText(summery);
            CheckBox mCheckBox = (CheckBox) convertView.findViewById(R.id.checkbox);
           
            if (mLstValue.get(groupPosition).size() == 1)
            	mCheckBox.setChecked(mLstValue.get(groupPosition).get(0));
            
            ImageView mArrow = (ImageView) convertView.findViewById(R.id.selectedIcon);
            if (getChildrenCount(groupPosition) == 0) {
                mCheckBox.setVisibility(View.VISIBLE);
                mArrow.setVisibility(View.GONE);
            } else {
                mCheckBox.setVisibility(View.GONE);
                mArrow.setVisibility(View.VISIBLE);
            }
            return convertView;
        }

        @Override
        public int getChildrenCount(int groupPosition) {
            // TODO Auto-generated method stub
            return mLstChild.get(groupPosition).size();
        }

        @Override
        public Object getGroup(int groupPosition) {
            // TODO Auto-generated method stub
            return mLstGroup.get(groupPosition);
        }

        @Override
        public int getGroupCount() {
            // TODO Auto-generated method stub
            return mLstGroup.size();
        }

        @Override
        public long getGroupId(int groupPosition) {
            // TODO Auto-generated method stub
            return groupPosition;
        }

        @Override
        public View getGroupView(int groupPosition, boolean isExpanded, View convertView, ViewGroup parent) {
            // TODO Auto-generated method stub
            String title = mLstGroup.get(groupPosition).get(0); 
            String summery = mLstGroup.get(groupPosition).get(1);     
            return generateGroupView(groupPosition, title, summery, convertView);
        }

        @Override
        public boolean hasStableIds() {
            // TODO Auto-generated method stub
            return false;
        }

        @Override
        public boolean isChildSelectable(int groupPosition, int childPosition) {
            // TODO Auto-generated method stub
            return true;
        }
    }

    @Override
    public void onGroupExpand(int groupPosition) {
        // TODO Auto-generated method stub
        int count = mAdapter.getGroupCount();
        for (int i = 0; i < count; i++) {
            if (i != groupPosition)
                mListView.collapseGroup(i);
        }
    }

    private void setChildValue(int groupPosition, int childPosition) {  	
    	int count = mLstValue.get(groupPosition).size();   	
    	if (count == 1) {
    		mLstValue.get(groupPosition).set(0, (childPosition == 1)?true:false);
    	} else {    		    	
	         for (int i = 0; i < count; i++) {
	             if (i != childPosition)
	            	 mLstValue.get(groupPosition).set(i, false);
	             else
	            	 mLstValue.get(groupPosition).set(i, true);
	         }
    	}
    }    
    
    @Override
    public boolean onChildClick(ExpandableListView listView, View view, int groupPosition, int childPosition, long id) {
        // TODO Auto-generated method stub  
    	setChildValue(groupPosition, childPosition);        
        mAdapter.notifyDataSetChanged();
        saveValues();     
        return false;
    }

    @Override
    public boolean onGroupClick(ExpandableListView listView, View view, int groupPosition, long id) {
        // TODO Auto-generated method stub
        CheckBox mBox = (CheckBox) view.findViewById(R.id.checkbox);
        if (mBox.getVisibility() == View.VISIBLE) {
        	setChildValue(groupPosition, (!mBox.isChecked() == true)?1:0);        
            mAdapter.notifyDataSetChanged();
            saveValues();
        }
        return false;
    }
    
    private void init() {    	
        mInflater = LayoutInflater.from(this);
        initData();
        mAdapter = new MyExpandableListViewAdapter();
        mListView = (ExpandableListView) findViewById(R.id.my_list);
        mListView.setAdapter(mAdapter);
        mListView.setOnGroupClickListener(this);
        mListView.setOnChildClickListener(this);
        mListView.setOnGroupExpandListener(this);
    }

    private void initData() {  	
    	mLstValue = new ArrayList<List<Boolean>>();  
        mLstGroup = new ArrayList<List<String>>();
        mLstChild = new ArrayList<List<String>>(); 

        Resources res = getResources();      
        addSetItem(new String[] {res.getString(R.string.set_videoquality_title), res.getString(R.string.set_videoquality_desc)}, 
        		new String[] {res.getString(R.string.set_videoquality_option_quality), res.getString(R.string.set_videoquality_option_smooth)});
        addSetItem(new String[] {res.getString(R.string.set_subtitle_title), res.getString(R.string.set_subtitle_desc)}, 
        		new String[] {});
        addSetItem(new String[] {res.getString(R.string.set_colortype_title), res.getString(R.string.set_colortype_desc)}, 
        		new String[] {res.getString(R.string.set_colortype_option_yuv), res.getString(R.string.set_colortype_option_rgb)});
        addSetItem(new String[] {res.getString(R.string.set_videodec_title), res.getString(R.string.set_videoeec_desc)}, 
        		new String[] {res.getString(R.string.set_videoeec_option_auto), res.getString(R.string.set_videoeec_option_Soft), res.getString(R.string.set_videoeec_option_IOMX), res.getString(R.string.set_videoeec_option_MediaCodec)});
    }

    private void addSetItem (String[] g, String[] c) {
        List<String> groupItem 			= new ArrayList<String>();
        List<Boolean> childValueItem 	= new ArrayList<Boolean>();
        for (int i = 0; i < g.length; i++) {
            groupItem.add(g[i]);
        }
        mLstGroup.add(groupItem);

        List<String> childItem = new ArrayList<String>();
        for (int i = 0; i < c.length; i++) {
            childItem.add(c[i]);
            childValueItem.add(false);
        }            
        if (c.length == 0) {
            childValueItem.add(false);
        }       
        mLstChild.add(childItem);
        mLstValue.add(childValueItem);    
    }
    
    private int getValueFromGroup (int nPos) {
    	int ret = 0;
    	
    	int count = mLstValue.get(nPos).size();	
    	if (count == 1) {
    		ret = (mLstValue.get(nPos).get(0))?1:0;
    	} else {    		    	
	         for (int i = 0; i < count; i++) {
	             if (mLstValue.get(nPos).get(i)) {
	            	 ret = i;
	            	 break;
	             }	                
	         }
    	}	
    	return ret;
    }
    
    private void saveValues() {    		
 	   m_nVideoQuality 	= getValueFromGroup(POS_VIDEOQUALITY);
 	   m_nDownloadFile 	= getValueFromGroup(POS_DOWNLOADFILE);
	   m_nColorType     = getValueFromGroup(POS_COLORTYPE);  
	   m_nVideoDec      = getValueFromGroup(POS_VIDEODEC);  
	   
	   SharedPreferences settings = this.getSharedPreferences("Player_Setting", 0);
	   SharedPreferences.Editor editor = settings.edit(); 

	   editor.putInt("VideoQuality", m_nVideoQuality);  
	   editor.putInt("DownloadFile", m_nDownloadFile);
	   editor.putInt("ColorType", m_nColorType); 
	   editor.putInt("VideoDec", m_nVideoDec); 
	   editor.commit(); 
    }
    
    private void loadDefaultValue() {
	   SharedPreferences settings = this.getSharedPreferences("Player_Setting", 0);
	   m_nVideoQuality 	= settings.getInt("VideoQuality", 0);
       m_nDownloadFile	= settings.getInt("DownloadFile", 0);
	   m_nColorType     = settings.getInt("ColorType", 0);   	   
	   m_nVideoDec      = settings.getInt("VideoDec", 1);   	
	   setChildValue(POS_VIDEOQUALITY, m_nVideoQuality);
	   setChildValue(POS_DOWNLOADFILE, m_nDownloadFile);
	   setChildValue(POS_COLORTYPE, m_nColorType);    	   
	   setChildValue(POS_VIDEODEC, m_nVideoDec);    
    }    
}
