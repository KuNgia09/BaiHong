package com.example.broad;

import java.io.File;
import java.io.FileOutputStream;

import com.baihong.zipencrypt.ZLibUtils;

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
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.FrameLayout;

public class CamActivity extends Activity {
	protected static final String TAG = "info";
	private CameraPreview mPreview;
	public static int nWidth = 800;
	public static int nHeight = 600;
	public byte[] encryptPicture;
	String  encryptPath;
	final byte key=0x64;
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_camera);
				
		encryptPath=Environment.getExternalStorageDirectory().getAbsolutePath()+File.separator+"EncryptPicture";
		File file=new File(encryptPath);
		if(!file.exists()){
			file.mkdir();
		}
			
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
						Log.d(TAG, "nWidth, nHeight="+nWidth+"x"+nHeight);
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

	public void Encrypt(byte[] picture){
		Log.d(TAG,"start xor");
		encryptPicture=picture;
		try {
			for(int i=0;i<picture.length;i++){
				encryptPicture[i]=(byte) (picture[i]^ key);
			}
		} catch (Exception e) {
			// TODO Auto-generated catch block
			Log.d(TAG,e.toString());
			e.printStackTrace();
		}
	}
	// save pic
	class SavePictureTask extends AsyncTask<byte[], String, String> {
		@Override
		protected String doInBackground(byte[]... params) {
			
			long random=System.currentTimeMillis();
			String picPath=encryptPath+ File.separator+random+".jpg";
			
			File pictureFile = new File(picPath);							
			
			try {
				FileOutputStream fos = new FileOutputStream(pictureFile);
				
				byte[] compressOutput=ZLibUtils.compress(params[0]);
				Log.d(TAG,"compressOutput length:"+compressOutput.length);
				Log.d(TAG,"params[0] length:"+params[0].length);
				fos.write(compressOutput);
				fos.close();
			} catch (Exception e) {
				Log.d(TAG, "save picture failed");
			}
			

			return null;
		}
	}
	
}
