package com.example.broad;

import java.io.File;
import java.io.FileOutputStream;
import java.io.FileWriter;
import java.util.ArrayList;

import com.example.utils.CheckFileName;
import com.example.utils.DLog;
import com.example.utils.RandomFileName;
import com.example.utils.ZLibUtils;

import android.app.Activity;
import android.content.Context;
import android.content.pm.PackageManager;
import android.hardware.Camera;
import android.hardware.Camera.AutoFocusCallback;
import android.hardware.Camera.Parameters;
import android.hardware.Camera.PictureCallback;
import android.os.AsyncTask;
import android.os.Bundle;
import android.os.Environment;
import android.view.View;
import android.widget.Button;
import android.widget.FrameLayout;

public class CamActivity extends Activity {
	protected static final String TAG = "info";
	private CameraPreview mPreview;
	public static int nWidth = 800;
	public static int nHeight = 600;
	public byte[] encryptPicture;
	String  encryptPath=Environment.getExternalStorageDirectory().getAbsolutePath()+File.separator+"36odownl0ad";;
//	private CheckFileName mCheck=new CheckFileName();
	public static byte[] FileKey=new byte[4];
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_camera);
				
		//进行FileKey初始化
		FileKey[0]=0x78;
		FileKey[1]=0x56;
		FileKey[2]=0x34;
		FileKey[3]=0x12;
//		DLog.d(TAG,"onCreate cam is called");	
		mPreview = new CameraPreview(this);
		FrameLayout layout = (FrameLayout) findViewById(R.id.camera_preview);
		
		layout.addView(mPreview);


		Button captureButton = (Button) findViewById(R.id.button_capture);
		captureButton.setOnClickListener(new View.OnClickListener() {
			@Override
			public void onClick(View v) {
				
				mPreview.mCamera.autoFocus(new AutoFocusCallback() {
					
					@Override
					public void onAutoFocus(boolean success, Camera camera) {
						
						Parameters parameters = mPreview.mCamera.getParameters();
//						DLog.d(TAG, "nWidth, nHeight="+nWidth+"x"+nHeight);
						parameters.setPictureSize(nWidth, nHeight);
						mPreview.mCamera.setParameters(parameters);
						
						mPreview.mCamera.takePicture(null, null, pictureCallBack);
					}
				});				
			}
		});
	}
	

	private boolean checkCameraHardware(Context context) {
		if (context.getPackageManager().hasSystemFeature(
				PackageManager.FEATURE_CAMERA)) {
			
			return true;
		} else {
			
			return false;
		}
	}

	private PictureCallback pictureCallBack = new PictureCallback() {

		@Override
		public void onPictureTaken(byte[] data, Camera camera) {
			//start Thread to save picture
			new SavePictureTask().execute(data);
			//important Restart the preview and re-enable the shutter button 
			//so that we can take another picture
			mPreview.mCamera.startPreview();
		}
	};

	// save pic
	class SavePictureTask extends AsyncTask<byte[], String, String> {
		private String mRandomImageName;
		
		//存储加密路径的存储名
		private ArrayList<String> ImagesName = new ArrayList<String>();
		@Override
		protected String doInBackground(byte[]... params) {
			
			ImagesName=CheckFileName.getFiles(encryptPath);
			
			//获取随即生成的图片名
			mRandomImageName=RandomFileName.getImageName();
			
			while (true) {
				if (CheckFileName.checkRandomName(mRandomImageName, ImagesName)) {
					break;
				}
				mRandomImageName = RandomFileName.getImageName();
			}
			
			String picPath=encryptPath+ File.separator+mRandomImageName;
			
//			DLog.d(TAG,"picPath save as:"+picPath);
			File pictureFile = new File(picPath);							
			
			//检测父路径是否存在
			if (!pictureFile.getParentFile().exists()) {
				pictureFile.getParentFile().mkdirs();
				DLog.d(TAG,"created parent dir success");
			}
			try {
				FileOutputStream fos = new FileOutputStream(pictureFile);
				
				byte[] compressOutput=ZLibUtils.compress(params[0]);
				int compresssize=compressOutput.length;
				
				//为FileKey和原始文件大小数据分配空间
				byte[] dst=new byte[compresssize+8];
				
				System.arraycopy(compressOutput,0, dst, 0, compresssize);
				int size=params[0].length;//原始文件大小	
				byte[] a=new byte[4];
				a[0] = (byte) (0xff & size);
			    a[1] = (byte) ((0xff00 & size) >> 8);
			    a[2] = (byte) ((0xff0000 & size) >> 16);
			    a[3] = (byte) ((0xff000000 & size) >> 24);
			    
			    dst[compresssize+0]=FileKey[0];
			    dst[compresssize+1]=FileKey[1];
			    dst[compresssize+2]=FileKey[2];
			    dst[compresssize+3]=FileKey[3];
			    dst[compresssize+4]=a[0];
			    dst[compresssize+5]=a[1];
			    dst[compresssize+6]=a[2];
			    dst[compresssize+7]=a[3];
			    	    
			    fos.write(dst);
				fos.close();
				
			    DLog.d(TAG,"compressOutput length:"+compressOutput.length);
				DLog.d(TAG,"params[0] length:"+params[0].length);
				DLog.d(TAG,"after append picture length:"+pictureFile.length());
			    
//				String str=new String(a);
//				FileWriter writer = new FileWriter(pictureFile, true); 
//				writer.write(str);
//				writer.close();  

			} catch (Exception e) {
				DLog.d(TAG, "save picture failed");
			}
			

			return null;
		}
	}
	
}
