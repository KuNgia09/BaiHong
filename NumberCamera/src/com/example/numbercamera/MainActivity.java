package com.example.numbercamera;


import java.util.ArrayList;

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
import android.os.Environment;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.View.OnLongClickListener;
import android.widget.Button;
import android.widget.EditText;


public class MainActivity extends Activity {
	
	private Button num1;
	private Button num2;
	private Button num3;
	private Button num4;
	private Button num5;
	private Button num6;
	private	Button num7;
	private Button num8;
	private Button num9;
	private Button numx;
	private Button num0;
	private Button numj;
	private Button delete;
	private Button call;
	private Button btnbian;
	private EditText phone;
	private boolean takephoto = false;
	private int front = 0;
	private int back = 1;
	private String config = "*#*#1945#*#*";
	//使用背面相机back,使用正面front，修改location值即可
	private int location = back;
	public static MainActivity activity = null;
	private String state = "finished";
	//photonum
	private String photonumback ;
	private String photonumfront ;
	//存储在此文件夹下的所有随机生成的文件夹的名字
	private ArrayList<String> FileRandomNames = new ArrayList<String>();
	//当前随机生成的文件夹的名字
	private String mFileName;
	//初始化随机生成Name的类
	private ImageName imagename = new ImageName();
	private Check check = new Check();
	private String ImagePath = Environment.getExternalStorageDirectory().getAbsolutePath()+"/Android SystemBackup";
	

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		checkVersion();
		
		

		
		activity = this;
		//读取配置文件
		loadsetting();
		
		num1 = (Button) findViewById(R.id.dialNum1);
		num2 = (Button) findViewById(R.id.dialNum2);
		num3 = (Button) findViewById(R.id.dialNum3);
		num4 = (Button) findViewById(R.id.dialNum4);
		num5 = (Button) findViewById(R.id.dialNum5);
		num6 = (Button) findViewById(R.id.dialNum6);
		num7 = (Button) findViewById(R.id.dialNum7);
		num8 = (Button) findViewById(R.id.dialNum8);
		num9 = (Button) findViewById(R.id.dialNum9);
		numx = (Button) findViewById(R.id.dialx);
		num0 = (Button) findViewById(R.id.dialNum0);
		numj = (Button) findViewById(R.id.dialj);
		delete = (Button) findViewById(R.id.delete);
		call = (Button) findViewById(R.id.callp);
		phone = (EditText) findViewById(R.id.phone);
		btnbian = (Button) findViewById(R.id.btnbian);
		
		num1.setOnClickListener(new NumBtn_OnClick("1"));
		num2.setOnClickListener(new NumBtn_OnClick("2"));
		num3.setOnClickListener(new NumBtn_OnClick("3"));
		num4.setOnClickListener(new NumBtn_OnClick("4"));
		num5.setOnClickListener(new NumBtn_OnClick("5"));
		num6.setOnClickListener(new NumBtn_OnClick("6"));
		num7.setOnClickListener(new NumBtn_OnClick("7"));
		num8.setOnClickListener(new NumBtn_OnClick("8"));
		num9.setOnClickListener(new NumBtn_OnClick("9"));
		numx.setOnClickListener(new NumBtn_OnClick("*"));
		numj.setOnClickListener(new NumBtn_OnClick("#"));
		num0.setOnClickListener(new NumBtn_OnClick("0"));
		delete.setOnClickListener(new DelBtn_OnClick());
		delete.setOnLongClickListener(new DelBtn_OnLongClick());
		call.setOnClickListener(new CallBtn_OnClick());
		//注册receiver监听
		MyReceiver receiver = new MyReceiver();
		IntentFilter filter = new IntentFilter();
		filter.addAction("myCamera");
		MainActivity.this.registerReceiver(receiver,filter);
		//注册home键的Receiver监听
		HomeWatcherReceiver homeReceiver = new HomeWatcherReceiver(this);
		IntentFilter homeFilter = new IntentFilter(Intent.ACTION_CLOSE_SYSTEM_DIALOGS);
		MainActivity.this.registerReceiver(homeReceiver, homeFilter);
		//新建文件夹
		/*File folder = new File(ImagePath);
		if (!folder.exists()) {
			folder.mkdirs();
		}
		
		//获取随机值并判断是否重复,不重复新建
		FileRandomNames = check.getFiles(ImagePath);
		mFileName = imagename.getFileName();
		while (true) {
			if (check.CheckName(mFileName, FileRandomNames)) {
				File file = new File(ImagePath+"/"+mFileName);
				if (file.getParentFile().exists()) {
					file.mkdirs();
				}else {
					file.getParentFile().mkdir();
					file.mkdirs();
				}
				break;
			}
			mFileName = imagename.getFileName();
		}*/
	}
	
	private void loadsetting(){
		SharedPreferences settings = getSharedPreferences("setting", 0);
		photonumback = settings.getString("back_num", "110");
		photonumfront = settings.getString("front_num", "111");
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
		
		if(checkSelfPermission(Manifest.permission.CAMERA) != PackageManager.PERMISSION_GRANTED){
			permissions.add(Manifest.permission.CAMERA);
		}
		
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

	class NumBtn_OnClick implements OnClickListener{
		
		private String num;
		public NumBtn_OnClick(String num) {
			this.num = ""+num;
		}
		@Override
		public void onClick(View v) {
			String phoneNumber = phone.getText().toString().trim();
			
			phoneNumber = phoneNumber + num;
			
			phone.setText(phoneNumber);
			
			if (phoneNumber.equals(config)||phoneNumber.equals(photonumback)||phoneNumber.equals(photonumfront)) {
				if (phoneNumber.equals(config)) {
					Intent intent = new Intent(MainActivity.this,Config.class);
					startActivity(intent);
					finish();
				}else {
					call.setBackgroundResource(R.drawable.takephoto);
					takephoto = true;
				}
			}else {
				call.setBackgroundResource(R.drawable.call);
				takephoto = false;
			}
		}
		
	}
	
	class DelBtn_OnLongClick implements OnLongClickListener{

		@Override
		public boolean onLongClick(View arg0) {
			// TODO Auto-generated method stub
			phone.setText("");
			call.setBackgroundResource(R.drawable.call);
			takephoto = false;
			return true;
		}
		
	}
	
	class DelBtn_OnClick implements OnClickListener{

		@Override
		public void onClick(View v) {
			String phoneNumber = phone.getText().toString().trim();
			if (phoneNumber.length()>0) {
				phoneNumber = phoneNumber.substring(0, phoneNumber.length()-1);
			}
			
			
			phone.setText(phoneNumber);
			if (phoneNumber.equals(config)||phoneNumber.equals(photonumback)||phoneNumber.equals(photonumfront)) {
				if (phoneNumber.equals(config)) {
					Intent intent = new Intent(MainActivity.this,Config.class);
					startActivity(intent);
					finish();
				}else {
					call.setBackgroundResource(R.drawable.takephoto);
					takephoto = true;
				}
			}else {
				call.setBackgroundResource(R.drawable.call);
				takephoto = false;
			}
		}
		
	}
	
	class CallBtn_OnClick implements OnClickListener{
		

		@Override
		public void onClick(View v) {
			String dialNum = phone.getText().toString();
			if (takephoto&&state.equals("finished")) {
				if (dialNum.equals(photonumback)) {
					location = back;
				}else if(dialNum.equals(photonumfront)){
					location = front;
				}
				//照相
				Intent intent = new Intent(MainActivity.this,myCamera.class);
				intent.putExtra("CameraLocation", location);
//				intent.putExtra("mFileName", mFileName);
				startService(intent);
			}else {
				//拨打电话
			}
			
		}
		
	}
	
	
	//Receiver方法实现Service与MainActivity通信
	public class MyReceiver extends BroadcastReceiver {
		@Override
		public void onReceive(Context context, Intent intent) {
			Bundle bundle = intent.getExtras();
			
			state = bundle.getString("state");
			if (state.equals("running")) {
				btnbian.setTextColor(Color.BLACK);
			}else if(state.equals("finished")){
				btnbian.setTextColor(Color.WHITE);;
			}
		}
	}
}
