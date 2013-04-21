package com.augmentedreality;

import android.os.Bundle;
import android.app.Activity;
import android.util.Log;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.ViewGroup.LayoutParams;
import android.view.Window;
import android.view.WindowManager;
import android.widget.FrameLayout;
import android.widget.ImageView;
import android.widget.ImageView.ScaleType;

public class MainActivity extends Activity {
	
	public static final String TAG = "bitirme::MainActivity";
	
	private CameraPreview camPreview;
	private ImageView myCameraPreview = null;
	private FrameLayout mainLayout;
	private int previewSizeWidth = 640;
	private int previewSizeHeight = 480;
	private int mode = 0;

	public MainActivity() {
		Log.i(TAG, "Instantiated new " + this.getClass());
		this.mode = 0;
	}

	public void setMode(int x) {
		this.mode = x;
	}
	
	public int getMode() {
		return mode;
	}
	
	@SuppressWarnings("deprecation")
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		Log.i(TAG, "onCreate");
		super.onCreate(savedInstanceState);

		//setContentView(R.layout.activity_main);
		
		/*
		Native.loadLibs();
		requestWindowFeature(Window.FEATURE_NO_TITLE);
		setContentView(new ARView(this));
		*/
		
		getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN, WindowManager.LayoutParams.FLAG_FULLSCREEN);
		requestWindowFeature(Window.FEATURE_NO_TITLE);
		setContentView(R.layout.activity_main);
		
		myCameraPreview = new ImageView(this);
		myCameraPreview.setLayoutParams(new FrameLayout.LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.MATCH_PARENT));
		myCameraPreview.setScaleType(ScaleType.FIT_XY);
		
		SurfaceView camView = new SurfaceView(this);
		camView.setLayoutParams(new FrameLayout.LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.MATCH_PARENT));
		
		SurfaceHolder camHolder = camView.getHolder();
		camPreview = new CameraPreview(previewSizeWidth, previewSizeHeight, myCameraPreview);
		camHolder.addCallback(camPreview);
		camHolder.setType(SurfaceHolder.SURFACE_TYPE_PUSH_BUFFERS);
		camHolder.setSizeFromLayout();
		
		mainLayout = (FrameLayout) findViewById(R.id.frameLayout);
		mainLayout.addView(camView, new FrameLayout.LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.MATCH_PARENT));
		mainLayout.addView(myCameraPreview, new FrameLayout.LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.MATCH_PARENT));
	}
	
	protected void onPause() {
		if (camPreview != null) {
			camPreview.onPause();
		}
		
		super.onPause();
	}
}
