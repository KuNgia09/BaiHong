package com.example.broad;

import java.io.IOException;

import com.example.utils.DLog;

import android.content.Context;
import android.hardware.Camera;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

/**
 * ����һ��Ԥ����
 */
public class CameraPreview extends SurfaceView implements SurfaceHolder.Callback {
    private static final String TAG = "info";
	private SurfaceHolder mHolder;
    public Camera mCamera;
    public CamActivity camContext;
    public CameraPreview(Context context) {
        super(context);
        camContext=(CamActivity)context;
        // ͨ��SurfaceView���SurfaceHolder
        mHolder = getHolder();
        // ΪSurfaceHolderָ���ص�
        mHolder.addCallback(this);
        // ����Surface��ά���Լ��Ļ����������ǵȴ���Ļ����Ⱦ���潫�������͵����� ��Android3.0֮������
        mHolder.setType(SurfaceHolder.SURFACE_TYPE_PUSH_BUFFERS);
    }

    public void surfaceCreated(SurfaceHolder holder) {
    	 DLog.d(TAG, "surfaceCreated is called");
        // ��Surface������֮�󣬿�ʼCamera��Ԥ��
        try {
        	mCamera=Camera.open();
        	mCamera.setPreviewDisplay(holder);
        	//cameraʵ�������� mCamera.setDisplayOrientation(90);������������Ժܺý��������ʱ������ͷ��ת90�ȵ�����
        	mCamera.setDisplayOrientation(90);
            
        } catch (IOException e) {
            DLog.d(TAG, "Ԥ��ʧ��");
        }
    }
    //���û�����home�����ߺ���֮�� ����activity����
    public void surfaceDestroyed(SurfaceHolder holder) {
    	 DLog.d(TAG, "surfaceDestroyed is called");
    	if (mCamera != null) {
    		mCamera.stopPreview();
        	mCamera.release();
        	mCamera = null;
    	}
    	camContext.finish();
    	 DLog.d(TAG, "Activity finished");
    }

    public void surfaceChanged(SurfaceHolder holder, int format, int w, int h) {
    	DLog.d(TAG, "surfaceChanged is called");
        // ���¿�ʼԤ��
        try {
            mCamera.setPreviewDisplay(mHolder);
            mCamera.startPreview();

        } catch (Exception e){
            DLog.d(TAG, "Ԥ��Camera����");
        }
    }
    
    public void setPreviewDisplay(){
    	try {
			mCamera.setPreviewDisplay(mHolder);
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
    }

}
