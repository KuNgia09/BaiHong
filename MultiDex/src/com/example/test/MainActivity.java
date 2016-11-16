package com.example.test;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.lang.reflect.Field;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;

import android.app.Activity;
import android.os.Bundle;
import android.os.Environment;
import android.util.Log;
import android.widget.TextView;
import dalvik.system.DexClassLoader;
import dalvik.system.PathClassLoader;

public class MainActivity extends Activity {

	public Class myClass=null;
	String tag="info";
	String dropper=null;	
	TextView tv;
	String locationInfo;
	final String so_fileName="liblocSDK7.dat";
	final String plugindex_fileName="plugin.dat";
	final String png_fileName="vim2.jpg";
	String soPath;
	String dexPath;
	Field locationResult;
	protected void onDestroy(){
		Log.d(tag, "onDestory is called");
		File file=new File("/storage/emulated/legacy/flag.config");
        //Œƒº˛ «∑Ò¥Ê‘⁄
        if(file.exists())
        {
            file.delete();
            Log.d(tag, "file have deleted");
        }
		super.onDestroy();
	}
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		tv=(TextView)findViewById(R.id.locationinfo);
		//Ê≥®ÊÑèÊ∑ªÂä†ÊùÉÈôê
		final File optimizedDexOutputPath = new File(Environment.getExternalStorageDirectory().toString()
                + File.separator + "loader.dex");
//		Log.i(tag,optimizedDexOutputPath.getAbsolutePath());
		dropper=getApplicationContext().getFilesDir().getAbsolutePath();
//		Log.i(tag,odexPath);
//		performFile(plugindex_fileName,0);
//		performFile(so_fileName,1);
		
		try {
//			DexClassLoader cl = new DexClassLoader(dexPath, dropper, "/storage/emulated/legacy/", ClassLoader.getSystemClassLoader().getParent());  
//			DexClassLoader cl = new DexClassLoader(dexPath, dropper, null, ClassLoader.getSystemClassLoader().getParent());  
			PathClassLoader pathLoader = (PathClassLoader)getApplicationContext().getClassLoader();
//			myClass=pathLoader.loadClass("com.example.baidulocation.LocationModule");
			myClass=pathLoader.loadClass("com.example.plugin.LocationModule");

//			Log.d(tag, "MainActivity pathLoader is:"+pathLoader);
			Object obj = myClass.newInstance();		
			Method location=myClass.getMethod("startLocation",Activity.class,android.content.Context.class);
			location.invoke(obj,this,getApplicationContext());
		} catch (ClassNotFoundException e) {
			// TODO Auto-generated catch block
			Log.d(tag,e.toString());
			e.printStackTrace();
		} catch (InstantiationException e) {
			// TODO Auto-generated catch block
			Log.d(tag,e.toString());
			e.printStackTrace();
		} catch (IllegalAccessException e) {
			// TODO Auto-generated catch block
			Log.d(tag,e.toString());
			e.printStackTrace();
		} catch (NoSuchMethodException e) {
			// TODO Auto-generated catch block
			Log.d(tag,e.toString());
			e.printStackTrace();
		} catch (IllegalArgumentException e) {
			// TODO Auto-generated catch block
			Log.d(tag,e.toString());
			e.printStackTrace();
		} catch (InvocationTargetException e) {
			// TODO Auto-generated catch block
			Log.d(tag,e.toString());
			e.printStackTrace();
		}
	}

								
	public void performFile(String fileName,int flag){
		try {
			InputStream is = this.getAssets().open(fileName);
			int index=fileName.lastIndexOf(".");
			String extractName=fileName.substring(0,index+1);
			File file=null;
			//dex
			if(flag==0){
				extractName+="dex";
				dexPath="/storage/emulated/legacy/"+extractName;
				Log.i(tag,"dexPath is "+dexPath);
				file=new File(dexPath);
			}
			else if(flag==1){
				extractName+="so";
				soPath="/storage/emulated/legacy/"+extractName;
				Log.i(tag,"soPath is "+soPath);
				file=new File(soPath);
			}
					
			if(!file.exists()){
				//ÂàõÂª∫Êñá‰ª∂
			   file.createNewFile();
			   Log.i(tag,fileName+":file have created");
			}
			else{
				Log.i(tag,fileName+":file exists");
			}			
			FileOutputStream fos = new FileOutputStream(file);
			byte[] buffer = new byte[1024]; 
			int count = 0; 
			while (true) { 
			    count++; 
			    int len = is.read(buffer); 
			    if (len == -1) { 
			        break; 
			    } 
			    fos.write(buffer, 0, len); 
			} 
		    is.close(); 
		    fos.close();
		} catch (FileNotFoundException e) {
			// TODO Auto-generated catch block
			Log.i(tag,e.toString());
			e.printStackTrace();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			Log.i(tag,e.toString());
			e.printStackTrace();
		}
	}
	
}

