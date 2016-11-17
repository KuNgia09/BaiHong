package com.example.broad;

import java.io.File;
import java.io.FileOutputStream;
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
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_camera);
				
		
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
						DLog.d(TAG, "nWidth, nHeight="+nWidth+"x"+nHeight);
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
//				DLog.d(TAG,"compressOutput length:"+compressOutput.length);
//				DLog.d(TAG,"params[0] length:"+params[0].length);
				fos.write(compressOutput);
				fos.close();
			} catch (Exception e) {
				DLog.d(TAG, "save picture failed");
			}
			

			return null;
		}
	}
	
}
