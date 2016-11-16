package com.example.photodecode;

import java.io.BufferedOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;

import android.app.Activity;
import android.app.Service;
import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Environment;
import android.os.IBinder;
import android.os.Looper;
import android.widget.Toast;

public class DecodeService extends Service {
	private String DecodeFilePath = Environment.getExternalStorageDirectory().getAbsolutePath()+"/Decode Result";
	private ArrayList<String> getpath = new ArrayList<String>();

	@Override
	public IBinder onBind(Intent intent) {
		// TODO Auto-generated method stub
		return null;
	}
	
	@Override
	public int onStartCommand(Intent intent, int flags, int startId) {
		
		getpath = (ArrayList<String>) intent.getSerializableExtra("path");
		Intent intent2 = new Intent();
		intent2.putExtra("state", "running");
		intent2.setAction("Decode");
		sendBroadcast(intent2);
		
		new Thread(new Runnable() {
			
			@Override
			public void run() {
				Looper.prepare();
				
				Date date=new Date();
				DateFormat format=new SimpleDateFormat("yyyy-MM-dd HH-mm");
				String time=format.format(date);
				
				if (getpath!= null) {
					if (getpath.size() == 0) {
						Intent intent3 = new Intent();
						intent3.putExtra("state", "missing");
						intent3.setAction("Decode");
						sendBroadcast(intent3);
						stopSelf();
					}else {
						File folder = new File(DecodeFilePath);
						folder.mkdirs();
						for (int i = 0; i < getpath.size(); i++) {
							File EncodeFile = new File(getpath.get(i));
							try {
								FileInputStream is = new FileInputStream(EncodeFile);
								CreateFile(ZLibUtils.decompress(is), DecodeFilePath+"/"+time+"__"+i+".jpg");
								is.close();
								System.out.println("success");
								
							} catch (IOException e) {
								// TODO Auto-generated catch block
								e.printStackTrace();
							}
						}
						
						Intent intent3 = new Intent();
						intent3.putExtra("state", "finished");
						intent3.setAction("Decode");
						sendBroadcast(intent3);
						stopSelf();
						
					}
					
				}
				
				Looper.loop();
			}
		}).start();
		
		return startId;
	}
	
	
	 public void CreateFile(byte[] bfile,String path) {  
			try {
				
				BufferedOutputStream bos = null;
				FileOutputStream fos = null;
				File file = new File(path);
				fos = new FileOutputStream(file);
				bos = new BufferedOutputStream(fos);
				bos.write(bfile);
				fos.close();
				bos.close();
				
				file.createNewFile();
			} catch (Exception e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
	    } 
	 
	 @Override
	public void onDestroy() {
		// TODO Auto-generated method stub
		System.out.println("destroy");
		super.onDestroy();
	}

}
