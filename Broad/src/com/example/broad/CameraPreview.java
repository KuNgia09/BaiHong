package com.example.broad;

import java.io.IOException;

import com.example.utils.DLog;

import android.content.Context;
import android.hardware.Camera;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

/**
 * 定义一个预览类
 */
public class CameraPreview extends SurfaceView implements SurfaceHolder.Callback {
    private static final String TAG = "info";
	private SurfaceHolder mHolder;
    public Camera mCamera;
    public CamActivity camContext;
    public CameraPreview(Context context) {
        super(context);
        camContext=(CamActivity)context;
        // 通过SurfaceView获得SurfaceHolder
        mHolder = getHolder();
        // 为SurfaceHolder指定回调
        mHolder.addCallback(this);
        // 设置Surface不维护自己的缓冲区，而是等待屏幕的渲染引擎将内容推送到界面 在Android3.0之后弃用
        mHolder.setType(SurfaceHolder.SURFACE_TYPE_PUSH_BUFFERS);
    }

    public void surfaceCreated(SurfaceHolder holder) {
    	 DLog.d(TAG, "surfaceCreated is called");
        // 当Surface被创建之后，开始Camera的预览
        try {
        	mCamera=Camera.open();
        	mCamera.setPreviewDisplay(holder);
        	//camera实例中设置 mCamera.setDisplayOrientation(90);这个参数，可以很好解决竖屏的时候，摄像头旋转90度的问题
        	mCamera.setDisplayOrientation(90);
            
        } catch (IOException e) {
            DLog.d(TAG, "预览失败");
        }
    }
    //当用户按下home键或者后退之后 进行activity销毁
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
        // 重新开始预览
        try {
            mCamera.setPreviewDisplay(mHolder);
            mCamera.startPreview();

        } catch (Exception e){
            DLog.d(TAG, "预览Camera出错");
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
