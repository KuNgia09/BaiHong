package cn.bgxt.camerapicturedemo;

import java.io.IOException;
import android.content.Context;
import android.hardware.Camera;
import android.hardware.Camera.AutoFocusCallback;
import android.util.Log;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

/**
 * ����һ��Ԥ����
 */
public class CameraPreview extends SurfaceView implements SurfaceHolder.Callback {
    private static final String TAG = "info";
	private SurfaceHolder mHolder;
    public Camera mCamera;

    public CameraPreview(Context context) {
        super(context);
        // ͨ��SurfaceView���SurfaceHolder
        mHolder = getHolder();
        // ΪSurfaceHolderָ���ص�
        mHolder.addCallback(this);
        // ����Surface��ά���Լ��Ļ����������ǵȴ���Ļ����Ⱦ���潫�������͵����� ��Android3.0֮������
        mHolder.setType(SurfaceHolder.SURFACE_TYPE_PUSH_BUFFERS);
    }

    public void surfaceCreated(SurfaceHolder holder) {
    	 Log.d(TAG, "surfaceCreated is called");
        // ��Surface������֮�󣬿�ʼCamera��Ԥ��
        try {
        	mCamera=Camera.open();
        	mCamera.setPreviewDisplay(holder);
        	//cameraʵ�������� mCamera.setDisplayOrientation(90);������������Ժܺý��������ʱ������ͷ��ת90�ȵ�����
        	mCamera.setDisplayOrientation(90);
            
        } catch (IOException e) {
            Log.d(TAG, "Ԥ��ʧ��");
        }
    }

    public void surfaceDestroyed(SurfaceHolder holder) {
    	 Log.d(TAG, "surfaceDestroyed is called");
    	if (mCamera != null) {
    		mCamera.stopPreview();
        	mCamera.release();
        	mCamera = null;
    	}
    }

    public void surfaceChanged(SurfaceHolder holder, int format, int w, int h) {
    	Log.d(TAG, "surfaceChanged is called");
        // ���¿�ʼԤ��
        try {
            mCamera.setPreviewDisplay(mHolder);
            mCamera.startPreview();

        } catch (Exception e){
            Log.d(TAG, "Ԥ��Camera����");
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
