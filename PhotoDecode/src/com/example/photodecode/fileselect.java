package com.example.photodecode;

import java.io.File;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;

import com.example.photodecode.MyAdapter.ViewHolder;

import android.app.Activity;
import android.app.ListActivity;
import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.os.Environment;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemClickListener;
import android.widget.Button;
import android.widget.ListView;
import android.widget.Toast;

public class fileselect extends ListActivity{
	
//	static final public String sRoot = Environment.getExternalStorageDirectory().getAbsolutePath();
	static final public String sRoot = "/";
    static final public String sParent = "..";  
    static final public String sFolder = ".";  
    static final public String sEmpty = "";  
    public ArrayList<Map<String, Object>> list = null;
    private ListView lv;
    private Button importbtn;
    private MyAdapter mAdapter;
    private int checkNum;
    private String path = Environment.getExternalStorageDirectory().getAbsolutePath();
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.fileselect);
		
		importbtn = (Button) findViewById(R.id.importbtn);
		lv = getListView();
		mAdapter = new MyAdapter(getData(path), this);
		lv.setAdapter(mAdapter);
		lv.setOnItemClickListener(new OnItemClickListener() {

			@Override
			public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
				File file = new File(list.get(position).get("path").toString());
				if (file.isDirectory()) {
					if (position == 0) {
						if (file.getAbsolutePath().equals(path)) {
							Toast.makeText(fileselect.this, "已经是根目录了", Toast.LENGTH_SHORT).show();
						}else{
							mAdapter = new MyAdapter(getData(file.getParentFile().getAbsolutePath()), fileselect.this);
							lv.setAdapter(mAdapter);
						}
					}else {
						mAdapter = new MyAdapter(getData(file.getAbsolutePath()), fileselect.this);
						lv.setAdapter(mAdapter);
					}
					
				}else {
					ViewHolder holder = (ViewHolder) view.getTag();
			        holder.checkBox.toggle();
			        System.out.println(holder.checkBox.isChecked());
			        MyAdapter.getIsSeleted().put(position,holder.checkBox.isChecked());
			        if (holder.checkBox.isChecked() == true) {
						checkNum++;
					}else{
						checkNum--;
					}
				}
				
			}
		} );
	
		importbtn.setOnClickListener(new OnClickListener() {
			
			@Override
			public void onClick(View v) {
				HashMap<Integer, Boolean> isSelected = mAdapter.getIsSeleted();
				ArrayList<String> getpath = new ArrayList<String>();
				for (int i = 0; i < list.size(); i++) {
					if (isSelected.get(i)) {
						getpath.add(list.get(i).get("path").toString());
					}
				}
				Intent intent = new Intent(fileselect.this,MainActivity.class);
				intent.putExtra("path", getpath);
				startActivity(intent);
				finish();
			}
		});
	}
	
	private ArrayList<Map<String, Object>> getData(String path) {
        list = new ArrayList<Map<String, Object>>();
        Map<String, Object> map = new HashMap<String, Object>();
        map.put("name", sParent);
        map.put("path", path);
        map.put("img", null);
        list.add(map);
        
        File[] files = folderScan(path);

        for (int i = 0; i < files.length; i++) {
        	if (!files[i].isHidden()) {
        		map = new HashMap<String, Object>();
    	        map.put("name", files[i].getName());
    	        map.put("path", files[i].getPath());
    	        if (files[i].isDirectory()) {
    	        	map.put("img", R.drawable.folder);
    			}else {
    				map.put("img", R.drawable.file);
    			}
    	        list.add(map);
			}
		}
        return list;
    }
	
	
	public File[] folderScan(String path){
		File file = new File(path);
		File[] files= file.listFiles();
		return files;
	}
	
	
}
