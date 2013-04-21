package com.augmentedreality;

import java.io.IOException;

import android.graphics.Bitmap;
import android.graphics.ImageFormat;
import android.hardware.Camera;
import android.hardware.Camera.Parameters;
import android.os.Handler;
import android.os.Looper;
import android.util.Log;
import android.view.SurfaceHolder;
import android.widget.ImageView;

public class CameraPreview implements SurfaceHolder.Callback, Camera.PreviewCallback {

	private Camera mCamera = null;
	private ImageView myCameraPreview = null;
	private Bitmap bitmap = null;
	private int[] pixels = null;
	private byte[] frameData = null;
	private int imageFormat;
	private int previewSizeWidth;
	private int previewSizeHeight;
	private boolean bProcessing = false;
	private int mode;
	
	Handler mHandler = new Handler(Looper.getMainLooper());
	
	public CameraPreview(int previewSizeWidth, int previewSizeHeight, ImageView cameraPreview) {
		this.previewSizeWidth = previewSizeWidth;
		this.previewSizeHeight = previewSizeHeight;
		this.myCameraPreview = cameraPreview;
		bitmap = Bitmap.createBitmap(previewSizeWidth, previewSizeHeight, Bitmap.Config.ARGB_8888);
		pixels = new int[previewSizeWidth * previewSizeHeight];		
	}

	@Override
	public void onPreviewFrame(byte[] arg0, Camera arg1) {
		if (imageFormat == ImageFormat.NV21) {
			if (!bProcessing) {
				pixels = new int[previewSizeWidth * previewSizeHeight];
				frameData = arg0;
				mHandler.post(processFrame);
			}
		}
	}

	public void onPause() {
		if (mCamera != null)
			mCamera.stopPreview();
	}

	@Override
	public void surfaceChanged(SurfaceHolder holder, int format, int width,
			int height) {
		Parameters parameters;
		
		parameters = mCamera.getParameters();
		parameters.setPreviewSize(previewSizeWidth, previewSizeHeight);
		
		imageFormat = parameters.getPreviewFormat();
		mCamera.setParameters(parameters);
		mCamera.startPreview();
	}

	@Override
	public void surfaceCreated(SurfaceHolder holder) {
		mCamera = Camera.open();
		
		try {
			mCamera.setPreviewDisplay(holder);
			mCamera.setPreviewCallback(this);
		} catch (IOException e) {
			mCamera.release();
			mCamera = null;
		}
	}

	@Override
	public void surfaceDestroyed(SurfaceHolder holder) {
		mCamera.setPreviewCallback(null);
		mCamera.stopPreview();
		mCamera.release();
		mCamera = null;
	}
	
	
	// define of native functions
	public static native void applyCanny(int width, int height, byte[] NV21FrameData, int[] pixels);
	public static native void applySobel(int width, int height, byte[] NV21FrameData, int[] pixels);
	public static native void detectMarkers(int width, int height, byte[] NV21FrameData, int[] pixels);
	
	static {
		System.loadLibrary("opencv_java");
		System.loadLibrary("augmentedreality");
	} 
	
	private Runnable processFrame = new Runnable() {
		
		@Override
		public void run() {
			Log.i("frame processing", "processFrame runnable ");
			bProcessing = true;
			
			//applyCanny(previewSizeWidth, previewSizeHeight, frameData, pixels);
			//applySobel(previewSizeWidth, previewSizeHeight, frameData, pixels);
			detectMarkers(previewSizeWidth, previewSizeHeight, frameData, pixels);
			
			bitmap.setPixels(pixels, 0, previewSizeWidth, 0, 0, previewSizeWidth, previewSizeHeight);
			myCameraPreview.setImageBitmap(bitmap);
			bProcessing = false;
		}
	};
}
