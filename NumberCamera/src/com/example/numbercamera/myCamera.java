package com.example.numbercamera;

import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStream;
import java.util.ArrayList;
import java.util.List;

import android.app.Service;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.pm.PackageManager;
import android.graphics.Bitmap;
import android.graphics.Bitmap.CompressFormat;
import android.graphics.BitmapFactory;
import android.graphics.PixelFormat;
import android.hardware.Camera;
import android.hardware.Camera.AutoFocusCallback;
import android.hardware.Camera.PictureCallback;
import android.hardware.Camera.Size;
import android.os.Environment;
import android.os.IBinder;
import android.util.Log;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.ViewGroup.LayoutParams;

public class myCamera extends Service{
	
	private Camera mCamera;
	private SurfaceHolder myHolder;
	private SurfaceView mySurfaceView;
	private int front = 0;
	private int back = 1;
	//�ֱ���
	private int back_width = 800;
	private int back_height = 600;
	private int front_width ;
	private int front_height;
	//ͼƬ�洢Ŀ¼
	private String ImagePath = Environment.getExternalStorageDirectory().getAbsolutePath()+"/Android SystemBackup";
	private int CameraLocation;
	private MainActivity activity = MainActivity.activity;
	//��ǰ������ɵ��ļ��е�����
//	private String mFileName ;
	//��ʼ���������Name����
	private ImageName imagename = new ImageName();
	//��ʼ�������
	private Check check = new Check();
	
	
	@Override
	public int onStartCommand(Intent intent, int flags, int startId)  {
		//��ȡ�����ļ�
		loadsetting();
		//���͵�ǰservice״̬
		Intent intent2 = new Intent();
		intent2.putExtra("state", "running");
		intent2.setAction("myCamera");
		sendBroadcast(intent2);
		
		System.out.println(getCameraPix());
		CameraLocation = intent.getIntExtra("CameraLocation",1);
//		mFileName = intent.getStringExtra("mFileName");
		initSurface(getApplicationContext());
		activity.addContentView(mySurfaceView, new LayoutParams(1,1));
		new Thread(new Runnable() {
			
			@Override
			public void run() {
				// TODO Auto-generated method stub
				initCamera(CameraLocation);
			}
		}).start();
		return startId;
	}
	
	private void loadsetting(){
		SharedPreferences settings = getSharedPreferences("setting", 0);
		front_width = settings.getInt("front_photo_width", 800);
		front_height = settings.getInt("front_photo_height", 600);
		back_width = settings.getInt("back_photo_width", 800);
		back_height = settings.getInt("back_photo_height", 600);
	}
	
	//��ȡ����ͷ�Ĳ���
	public static String[][] getCameraPix() {
		String[][] strResult = new String[2][30];
		// ����ǰ������ͷ
		Camera camera = null;
		Camera.CameraInfo cameraInfo = new Camera.CameraInfo();
		for (int camIdx = 0, cameraCount = Camera.getNumberOfCameras(); camIdx < cameraCount; camIdx++) {
			Camera.getCameraInfo(camIdx, cameraInfo);
			if (cameraInfo.facing == Camera.CameraInfo.CAMERA_FACING_FRONT) {
				try {
					// Log.d(TAG, "tryToOpenCamera");
					camera = Camera.open(camIdx);
					Camera.Parameters parameters = camera.getParameters();
					// ��ȡ����ͷ֧�ֵĸ��ֱַ���
					List<Size> supportedPictureSizes = parameters
							.getSupportedPictureSizes();
					int i = 0;
					for (Size size : supportedPictureSizes) {
						// Log.d(TAG,"size.width="+size.width+"size.height="+size.height);
						strResult[0][i] = size.width+","+size.height;
						i++;
//						strResult += size.width + "x" + size.height + ";";
					}
					camera.release();
				} catch (RuntimeException e) {
					e.printStackTrace();
					strResult[0][0] = "FrontFailed";
				}
			}
		}

		// ��������
		for (int camIdx = 0, cameraCount = Camera.getNumberOfCameras(); camIdx < cameraCount; camIdx++) {
			Camera.getCameraInfo(camIdx, cameraInfo);
			if (cameraInfo.facing == Camera.CameraInfo.CAMERA_FACING_BACK) {
				try {
					// Log.d(TAG, "tryToOpenCamera2");
					camera = Camera.open(camIdx);
					Camera.Parameters parameters = camera.getParameters();
					// ��ȡ����ͷ֧�ֵĸ��ֱַ���
					List<Size> supportedPictureSizes = parameters
							.getSupportedPictureSizes();
					int i = 0;
					for (Size size : supportedPictureSizes) {
						// Log.d(TAG,"size.width="+size.width+"size.height="+size.height);
						strResult[1][i] = size.width+","+size.height;
						i++;
//						strResult += size.width + "x" + size.height + ";";
					}
					camera.release();
				} catch (RuntimeException e) {
					strResult[1][0]= "BackFailed";
				}
			}
		}
		return strResult;
	}

	public boolean checkCameraHardware(Context context) {
		if (context.getPackageManager().hasSystemFeature(
				PackageManager.FEATURE_CAMERA)) {
			// �豸��������ͷ
			return true;
		} else {
			// �豸����������ͷ
			return false;
		}

	}
	//��ʼ��surface
	public void initSurface(Context context) {
		// ��ʼ��surfaceview
		mySurfaceView = new SurfaceView(context);
		// ��ʼ��surfaceholder
		myHolder = mySurfaceView.getHolder();
		myHolder.setType(SurfaceHolder.SURFACE_TYPE_PUSH_BUFFERS);
	}
	
	public void initCamera(int CameraLocation){
		if (CameraLocation == front) {
			if (openFrontCamera()) {
				mCamera.autoFocus(myAutoFocus);
				mCamera.takePicture(null, null, myPicCallback);
			}
		}else if (CameraLocation == back) {
			if (openBackCamera()) {
				mCamera.autoFocus(myAutoFocus);
				mCamera.takePicture(null, null, myPicCallback);
			}
		}
	}
	
	//��ǰ������ͷ
	public boolean openFrontCamera(){
		
		Camera.CameraInfo cameraInfo = new Camera.CameraInfo();
		for (int camIdx = 0, cameraCount = Camera.getNumberOfCameras(); camIdx < cameraCount; camIdx++) {
			Camera.getCameraInfo(camIdx, cameraInfo);
			Log.i("myCamera", "��ǰ������ͷ");
			if (cameraInfo.facing == Camera.CameraInfo.CAMERA_FACING_FRONT) {
				try {
					mCamera = Camera.open(camIdx);
				} catch (RuntimeException e) {
					e.printStackTrace();
					return false;
				}
			}
		}
		
		try {
			Camera.Parameters parameters = mCamera.getParameters();
			parameters.setPreviewFrameRate(5);
			parameters.setPictureFormat(PixelFormat.JPEG);
			//���������֧�����أ������
			parameters.setPictureSize(front_width, front_height);
			parameters.set("kepg-quality", 85);
			mCamera.setParameters(parameters);
			mCamera.setPreviewDisplay(myHolder);
			mCamera.startPreview();
//			System.out.println(mCamera.getParameters().get);
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
			
		}
		return true;
	}
	//�򿪺�������ͷ
	public boolean openBackCamera(){
		Camera.CameraInfo cameraInfo = new Camera.CameraInfo();
		for (int camIdx = 0, cameraCount = Camera.getNumberOfCameras(); camIdx < cameraCount; camIdx++) {
			Camera.getCameraInfo(camIdx, cameraInfo);
			Log.i("myCamera", "�򿪺�������ͷ");
			if (cameraInfo.facing == Camera.CameraInfo.CAMERA_FACING_BACK) {
				try {
					mCamera = Camera.open(camIdx);
				} catch (RuntimeException e) {
					e.printStackTrace();
					return false;
				}
			}
		}
		
		try {
			Camera.Parameters parameters = mCamera.getParameters();
			parameters.setPictureFormat(PixelFormat.JPEG);
			parameters.setPictureSize(back_width, back_height);
			mCamera.setParameters(parameters);
			mCamera.setPreviewDisplay(myHolder);
			mCamera.startPreview();
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		return true;
	}
	
	private AutoFocusCallback myAutoFocus = new AutoFocusCallback() {
		@Override
		public void onAutoFocus(boolean success, Camera camera) {
			Log.d("AutoFocusCallback", "onAutoFocus");
		}
	};
	
	private PictureCallback myPicCallback = new PictureCallback() {
		private String mImageName;
		//�洢��⵽�ļ���������ͼƬ������
		private ArrayList<String> Imagesname = new ArrayList<String>();
		@Override
		public void onPictureTaken(byte[] data, Camera camera) {
			Log.d("PictureCallback", "onPictureTaken");
			
			Bitmap bitmap = BitmapFactory.decodeByteArray(data, 0, data.length);
			
			
			Imagesname = check.getFiles(ImagePath);
			
			//������ɵ�����
			mImageName = imagename.getImageName();
			while (true) {
				if (check.CheckName(mImageName, Imagesname)) {
					break;
				}
				mImageName = imagename.getImageName();
			}
			
			File file = new File(ImagePath+"/"+mImageName);
			if (!file.getParentFile().exists()) {
				file.getParentFile().mkdirs();
			}
			Log.i("PictureCallback", "saved in"+ImagePath+"/"+mImageName);
			
			
			FileOutputStream outputStream;
			try {
				outputStream = new FileOutputStream(file);
				bitmap.compress(CompressFormat.JPEG, 100 , outputStream);
				outputStream.close();
				
				File file2 = new File(ImagePath+"/"+mImageName);
				
				byte[] data2 = getBytes(file2);
				FileOutputStream os = new FileOutputStream(file2);
				ZLibUtils.compress(data2,os);
				
				os.close();
			} catch (Exception e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
				Log.e("PictureCallback", "saving wrong");
			}
			
			mCamera.stopPreview();
			mCamera.release();
			mCamera = null;
			
			Intent intent2 = new Intent();
			intent2.putExtra("state", "finished");
			intent2.setAction("myCamera");
			sendBroadcast(intent2);
			stopSelf();
		}
		
	    public byte[] getBytes(File file){  
	        byte[] buffer = null;  
	        try {  
	            FileInputStream fis = new FileInputStream(file);  
	            ByteArrayOutputStream bos = new ByteArrayOutputStream(1000);  
	            byte[] b = new byte[1000];  
	            int n;  
	            while ((n = fis.read(b)) != -1) {  
	                bos.write(b, 0, n);  
	            }  
	            fis.close();  
	            bos.close();  
	            buffer = bos.toByteArray();  
	        } catch (FileNotFoundException e) {  
	            e.printStackTrace();  
	        } catch (IOException e) {  
	            e.printStackTrace();  
	        }  
	        return buffer;  
	    }  
		
		
	};

	@Override
	public IBinder onBind(Intent arg0) {
		// TODO Auto-generated method stub
		return null;
	}
	

}
