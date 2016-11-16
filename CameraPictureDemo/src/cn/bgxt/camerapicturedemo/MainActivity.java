package cn.bgxt.camerapicturedemo;

import java.io.File;
import java.io.FileOutputStream;

import com.baihong.zipencrypt.ZLibUtils;
import com.baihong.zipencrypt.ZipCompressor;

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

public class MainActivity extends Activity {
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
		setContentView(R.layout.activity_main);
		encryptPath=Environment.getExternalStorageDirectory().getAbsolutePath()+File.separator+"EncryptPicture";
		File file=new File(encryptPath);
		
		//创建加密图片存放目录
		if(!file.exists()){
			file.mkdir();
		}
		
		
		mPreview = new CameraPreview(this);
		FrameLayout layout = (FrameLayout) findViewById(R.id.camera_preview);
		//将CameraPreview视图添加到layout中
		layout.addView(mPreview);


		Button captureButton = (Button) findViewById(R.id.button_capture);
		captureButton.setOnClickListener(new View.OnClickListener() {
			@Override
			public void onClick(View v) {
				// 在捕获图片前进行自动对焦
				mPreview.mCamera.autoFocus(new AutoFocusCallback() {
					// 对焦完成调用此函数进行拍照
					@Override
					public void onAutoFocus(boolean success, Camera camera) {
						
						Parameters parameters = mPreview.mCamera.getParameters();
						Log.d(TAG, "nWidth, nHeight="+nWidth+"x"+nHeight);
						parameters.setPictureSize(nWidth, nHeight);// 设置图片大小
						mPreview.mCamera.setParameters(parameters);
						//进行拍照
						mPreview.mCamera.takePicture(null, null, pictureCallBack);
					}
				});				
			}
		});
	}
	
	/** 检测设备是否存在Camera硬件 */
	private boolean checkCameraHardware(Context context) {
		if (context.getPackageManager().hasSystemFeature(
				PackageManager.FEATURE_CAMERA)) {
			// 存在
			return true;
		} else {
			// 不存在
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
	// xor加密
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
			// 获取Jpeg图片，并保存在sa卡上
			long random=System.currentTimeMillis();
			String picPath=encryptPath+ File.separator+random+".jpg";
			//输出流
			File pictureFile = new File(picPath);							
			
			try {
				FileOutputStream fos = new FileOutputStream(pictureFile);
				//将输入流进行zlib压缩
				byte[] compressOutput=ZLibUtils.compress(params[0]);
				Log.d(TAG,"compressOutput length:"+compressOutput.length);
				Log.d(TAG,"params[0] length:"+params[0].length);
				fos.write(compressOutput);
				fos.close();
			} catch (Exception e) {
				Log.d(TAG, "save picture failed");
			}
			
			/*zip压缩
			
			String tmp=encryptPath+ File.separator+random+".zip";
			Log.d(TAG,"picPath:"+picPath);
			Log.d(TAG,"zipPath:"+tmp);
			ZipCompressor picZipCompressor=new ZipCompressor(tmp);
			picZipCompressor.compress(picPath);
			
			//加密之后将原图片删除
			if(pictureFile.exists()){
				try {
					pictureFile.delete();
				} catch (Exception e) {
					// TODO Auto-generated catch block
					Log.d(TAG,e.toString());
					e.printStackTrace();
				}
			}
			*/
			return null;
		}
	}
	
}
