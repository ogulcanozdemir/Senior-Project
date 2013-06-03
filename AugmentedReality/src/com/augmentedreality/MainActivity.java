package com.augmentedreality;

import org.opencv.android.BaseLoaderCallback;
import org.opencv.android.CameraBridgeViewBase;
import org.opencv.android.CameraBridgeViewBase.CvCameraViewFrame;
import org.opencv.android.CameraBridgeViewBase.CvCameraViewListener2;
import org.opencv.android.LoaderCallbackInterface;
import org.opencv.android.OpenCVLoader;
import org.opencv.core.Mat;

import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.app.Activity;
import android.graphics.PixelFormat;
import android.util.Log;
import android.view.ViewGroup.LayoutParams;
import android.view.Window;
import android.view.WindowManager;

public class MainActivity extends Activity implements CvCameraViewListener2 {

	public static final String 		TAG = "AugmentedReality:MainActivity";
	
	private Mat						mRgba;
	private Mat						mGray;
	private ARMarkerDetector		mMarkerDetector;
	private CameraBridgeViewBase	mCameraView;
	private GLSurfaceView 			view;
	private ARRenderer				renderer;
	
	private BaseLoaderCallback mLoaderCallBack = new BaseLoaderCallback(this) {
		@Override
		public void onManagerConnected(int status) {
			switch (status) {
				case LoaderCallbackInterface.SUCCESS:
				{
					Log.i(TAG, "opencv loaded");
					System.loadLibrary("opencv_java");
					System.loadLibrary("augmentedreality");
					
					mMarkerDetector = new ARMarkerDetector();
					mCameraView.enableFpsMeter();
					mCameraView.setKeepScreenOn(true);
					mCameraView.setMaxFrameSize(640, 480);
					mCameraView.enableView();
				} break;

				default:
				{
					super.onManagerConnected(status);
				} break;
			}
		}
	};
	
	public MainActivity() {
		Log.i(TAG, "Instantiated new " + this.getClass());
	}

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		Log.i(TAG, "onCreate");
		super.onCreate(savedInstanceState);
		
		requestWindowFeature(Window.FEATURE_NO_TITLE);
		getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);
		getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
		
		System.loadLibrary("opencv_java");
		System.loadLibrary("augmentedreality");
		
		
		setContentView(R.layout.activity_main);
		
		
		getWindow().setFormat(PixelFormat.TRANSLUCENT);
		view = new ARSurfaceView(getApplication());
		/*
		view.setEGLContextClientVersion(2);
		new Thread(new Runnable() {

			@Override
			public void run() {
				view.setEGLConfigChooser(8, 8, 8, 8, 16, 0);
				view.setZOrderOnTop(true);
				renderer = new ARRenderer(getApplication());
				view.setRenderer(renderer);
				view.setRenderMode(GLSurfaceView.RENDERMODE_WHEN_DIRTY);
				view.getHolder().setFormat(PixelFormat.TRANSLUCENT);
				addContentView(view, new LayoutParams(LayoutParams.WRAP_CONTENT, LayoutParams.WRAP_CONTENT));
			}
		}).start();
		
		*/
		addContentView(view, new LayoutParams(LayoutParams.WRAP_CONTENT, LayoutParams.WRAP_CONTENT));
		
		mCameraView = (CameraBridgeViewBase)findViewById(R.id.surface_view);
		mCameraView.setCvCameraViewListener(this);
		
	}

	@Override
	public void onPause() {
		super.onPause();
		if (mCameraView != null) {
			mCameraView.disableView();
		}
		
		if (view != null) {
			view.onPause();
		}
	}
	
	@Override 
	public void onResume() {
		super.onResume();
		OpenCVLoader.initAsync(OpenCVLoader.OPENCV_VERSION_2_4_5, this, mLoaderCallBack);
	}
	
	@Override 
	public void onDestroy() {
		super.onDestroy();
		mCameraView.disableView();
	}
	
	@Override
	public void onCameraViewStarted(int width, int height) {
		mGray = new Mat();
		mRgba = new Mat();
	}

	@Override
	public void onCameraViewStopped() {
		mGray.release();
		mRgba.release();
	}

	@Override
	public Mat onCameraFrame(CvCameraViewFrame inputFrame) {
		mRgba = inputFrame.rgba();
		mGray = inputFrame.gray();

		Mat output = new Mat(mGray.size(), mGray.type());
		if (mMarkerDetector != null)
			mMarkerDetector.detect(mRgba, mGray, output);
		
		view.requestRender();
				
		return null;
	}
}
