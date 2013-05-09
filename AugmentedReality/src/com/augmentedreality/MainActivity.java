package com.augmentedreality;

import org.opencv.android.BaseLoaderCallback;
import org.opencv.android.CameraBridgeViewBase;
import org.opencv.android.CameraBridgeViewBase.CvCameraViewFrame;
import org.opencv.android.CameraBridgeViewBase.CvCameraViewListener2;
import org.opencv.android.LoaderCallbackInterface;
import org.opencv.android.OpenCVLoader;
import org.opencv.core.Mat;

import android.os.Bundle;
import android.app.Activity;
import android.util.Log;
import android.view.WindowManager;

public class MainActivity extends Activity implements CvCameraViewListener2 {
	
	public static final String 		TAG = "AugmentedReality:MainActivity";
	
	private Mat						mRgba;
	private Mat						mGray;
	private ARMarkerDetector		mMarkerDetector;
	private CameraBridgeViewBase	mCameraView;
	
	private BaseLoaderCallback mLoaderCallBack = new BaseLoaderCallback(this) {
		@Override
		public void onManagerConnected(int status) {
			switch (status) {
				case LoaderCallbackInterface.SUCCESS:
				{
					Log.i(TAG, "opencv loaded");
					System.loadLibrary("augmentedreality");
					System.loadLibrary("opencv_java");
					
					mMarkerDetector = new ARMarkerDetector();
					
					mCameraView.enableFpsMeter();
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
		
		getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
		setContentView(R.layout.activity_main);
		
		mCameraView = (CameraBridgeViewBase)findViewById(R.id.surface_view);
		mCameraView.setCvCameraViewListener(this);
	}

	@Override
	public void onPause() {
		super.onPause();
		if (mCameraView != null) {
			mCameraView.disableView();
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
			mMarkerDetector.detect(mGray, output);
		
		return mGray;
	}
	
}
