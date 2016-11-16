package com.example.decryptpicture;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.ArrayList;

import android.app.Activity;
import android.os.Bundle;
import android.os.Environment;
import android.util.Log;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;

public class MainActivity extends Activity {
	protected static final String TAG = "info";
	Button Decrypt;

	String  encryptPath;
	String  decryptPath;
	final byte key=0x64;
	byte[] decryptPicture;
	private static ArrayList<String> filelist = new ArrayList<String>();
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		Decrypt=(Button)findViewById(R.id.decrypt);
		encryptPath=Environment.getExternalStorageDirectory().getAbsolutePath()+File.separator+"EncryptPicture";
		decryptPath=Environment.getExternalStorageDirectory().getAbsolutePath()+File.separator+"DecryptPicture";
		
		File file=new File(encryptPath);
		if(!file.exists()){
			// encryptPath not existed exit
			Log.d(TAG,"encryptPath not existed exit");
		}
		file=new File(decryptPath);
		if(!file.exists()){
			file.mkdir();
			Log.d(TAG,"create decryptPath success ");
		}
		Decrypt.setOnClickListener(new OnClickListener() {
			
			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
				//列举目录文件
				getFiles(encryptPath);
				Log.d(TAG,"filelist size is"+filelist.size());
				//开启线程批量解密图片
				new Thread(new Runnable() {
					
					@Override
					public void run() {
						// TODO Auto-generated method stub
						String tmp;
						for(int i=0;i<filelist.size();i++){
							tmp=filelist.get(i);
							Log.d(TAG, "encryptPic is"+tmp);
							//解密图片
							DecryptPicture(tmp);
						}
					}
				}).start();
			}
		});
	}
	public void getFiles(String filePath){
		 File root = new File(filePath);
		 
/*		 Returns an array of files contained in the directory represented by this file. 
		 The result is null if this file is not a directory. 
		 The paths of the files in the array are absolute if the path of this file is absolute, 
		 they are relative otherwise.*/
		 File[] files=root.listFiles();
		 for(File file:files){     
		     if(file.isDirectory()){
			      // 递归调用
			     getFiles(file.getAbsolutePath());
			     Log.d(TAG,"file directory:"+file.getAbsolutePath());
		     }
		     else{
		    	 filelist.add(file.getAbsolutePath());
		    	 Log.d(TAG,"ListFile:"+file.getAbsolutePath());
		     }     
		 }
	}
	// xor加密
	public void DecryptByte(byte[] picture){
		Log.d(TAG,"start xor");
		decryptPicture=picture;
		try {
			for(int i=0;i<picture.length;i++){
				decryptPicture[i]=(byte) (picture[i]^ key);
			}
		} catch (Exception e) {
			// TODO Auto-generated catch block
			Log.d(TAG,e.toString());
			e.printStackTrace();
		}
	}
	
	private void DecryptPicture(String enPicPath){//enPicPath加密图片路径
			//加密图片路径	
			String fileName=new File(enPicPath).getName();
			//解密图片路径
			String dePicPath=decryptPath+File.separator+fileName.substring(0,fileName.lastIndexOf("."))+".jpg";
			Log.d(TAG,"dePicPath:"+dePicPath);
					
		    try {
		    	File picfile=new File(enPicPath);
				long filesize=picfile.length();
				FileInputStream fis = new FileInputStream(picfile);  
				byte[] buffer = new byte[(int)filesize];  
				int offset = 0;  
				int numRead = 0;  
				//将加密文件读取到buffer数组
				while (offset < buffer.length  
				&& (numRead = fis.read(buffer, offset, buffer.length - offset)) >= 0) {  
				    offset += numRead;  
				}
				//进行xor解密
				DecryptByte(buffer);
				//写入解密的图片内容到文件
				File deFile=new File(dePicPath);
				//如果解密过了 跳过
				if(!deFile.exists()){
					FileOutputStream fos=new FileOutputStream(deFile);
					fos.write(decryptPicture);
					fos.close();
					Log.d(TAG,"decrypt success");
				}
				else{
					Log.d(TAG,"have decrypt donothing");
				}
				fis.close();	
				
				Log.d(TAG,"filesize is:"+filesize+" numRead is:"+numRead);
			} catch (FileNotFoundException e) {
				// TODO Auto-generated catch block
				Log.d(TAG,e.toString());
				e.printStackTrace();
			} catch (IOException e) {
				// TODO Auto-generated catch block
				Log.d(TAG,e.toString());
				e.printStackTrace();
			}
	}


}
