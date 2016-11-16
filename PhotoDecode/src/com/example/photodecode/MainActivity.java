package com.example.photodecode;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import android.Manifest;
import android.annotation.TargetApi;
import android.app.Activity;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.SharedPreferences;
import android.content.pm.PackageManager;
import android.graphics.Color;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.ListView;
import android.widget.TextView;

public class MainActivity extends Activity {
	
	private Button seletebtn;
	private Button decodebtn;
	private TextView path;
	private ArrayList<String> getpath = new ArrayList<String>();
	public static MainActivity activity;
	private MyReceiver mReceiver;
	private Intent intent;
	private TextView helptxt;
	private TextView showtext;
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		
		checkVersion();
		activity = this;
		seletebtn = (Button) findViewById(R.id.select);
		decodebtn = (Button) findViewById(R.id.decodebtn);
		helptxt = (TextView) findViewById(R.id.helptxt);
		showtext = (TextView) findViewById(R.id.showpath);
		mReceiver = new MyReceiver();
		
		IntentFilter filter = new IntentFilter();
		filter.addAction("Decode");
		MainActivity.this.registerReceiver(mReceiver, filter);
		
		if (getpath.size() != 0 ) {
			showtext.setVisibility(View.VISIBLE);
			for (int i = 0; i < getpath.size(); i++) {
				showtext.setText(showtext.getText() + "\n" + getpath.get(i));
			}
		}
		
		seletebtn.setOnClickListener(new Button.OnClickListener() {
			
			@Override
			public void onClick(View v) {
				intent = new Intent(MainActivity.this , fileselect.class);
				startActivity(intent);
			}
		});
		
		
		decodebtn.setOnClickListener(new Button.OnClickListener() {
			
			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
				intent = new Intent(MainActivity.this,DecodeService.class);
				intent.putExtra("path", getpath);
				startService(intent);
			}
		});
		
	}
	
	private boolean checkVersion(){
		int currentapiVersion=android.os.Build.VERSION.SDK_INT;
		System.out.println(currentapiVersion);
		if (currentapiVersion>=23) {
			initpermission();
		}
		return true;
	}
	
	@TargetApi(23)
	private void initpermission(){
		
		ArrayList<String> permissions = new ArrayList<String>();
		
		
		if(checkSelfPermission(Manifest.permission.MOUNT_UNMOUNT_FILESYSTEMS) != PackageManager.PERMISSION_GRANTED){
			permissions.add(Manifest.permission.MOUNT_UNMOUNT_FILESYSTEMS);
		}
		
		if(checkSelfPermission(Manifest.permission.WRITE_EXTERNAL_STORAGE) != PackageManager.PERMISSION_GRANTED){
			permissions.add(Manifest.permission.WRITE_EXTERNAL_STORAGE);
		}
		
		if (permissions.size() > 0) {
			requestPermissions(permissions.toArray(new String[permissions.size()]), 127);
		}
	}
	
	@Override
	protected void onNewIntent(Intent intent2) {
		// TODO Auto-generated method stub
		super.onNewIntent(intent);
		setContentView(R.layout.activity_main);
		activity = this;
		seletebtn = (Button) findViewById(R.id.select);
		decodebtn = (Button) findViewById(R.id.decodebtn);
		helptxt = (TextView) findViewById(R.id.helptxt);
		showtext = (TextView) findViewById(R.id.showpath);
		
		getpath = (ArrayList<String>) intent2.getSerializableExtra("path");
		
		mReceiver = new MyReceiver();
		IntentFilter filter = new IntentFilter();
		filter.addAction("Decode");
		MainActivity.this.registerReceiver(mReceiver, filter);
		
		if (getpath.size() != 0 ) {
			showtext.setVisibility(View.VISIBLE);
			for (int i = 0; i < getpath.size(); i++) {
				showtext.setText(showtext.getText() + "\n" + getpath.get(i));
			}
		}
		
		seletebtn.setOnClickListener(new Button.OnClickListener() {
			
			@Override
			public void onClick(View v) {
				intent = new Intent(MainActivity.this , fileselect.class);
				startActivity(intent);
			}
		});
		
		
		decodebtn.setOnClickListener(new Button.OnClickListener() {
			
			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
			    intent = new Intent(MainActivity.this,DecodeService.class);
			    intent.putExtra("path", getpath);
				startService(intent);
			}
		});
	}
	
	
	public class MyReceiver extends BroadcastReceiver {
		@Override
		public void onReceive(Context context, Intent intent) {
			Bundle bundle = intent.getExtras();
			String state = bundle.getString("state");
			if (state.equals("running")) {
				System.out.println("service running");
				seletebtn.setEnabled(false);
				decodebtn.setEnabled(false);
				helptxt.setTextColor(Color.BLUE);
				helptxt.setText("正在解密");
			}else if (state.equals("finished")||state.equals("missing")) {
				System.out.println("service stop");
				seletebtn.setEnabled(true);
				decodebtn.setEnabled(true);
				if (state.equals("finished")) {
					helptxt.setText("完成");
					helptxt.setTextColor(Color.GREEN);
				}else if (state.equals("missing")) {
					helptxt.setText("找不到文件");
					helptxt.setTextColor(Color.RED);
				}
			}
		}
	}
}
