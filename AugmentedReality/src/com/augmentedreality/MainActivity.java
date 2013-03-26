package com.augmentedreality;

import android.os.Bundle;
import android.app.Activity;
import android.content.Context;
import android.content.SharedPreferences;
import android.content.SharedPreferences.Editor;
import android.util.Log;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.ViewGroup.LayoutParams;
import android.view.Window;
import android.view.WindowManager;
import android.widget.FrameLayout;
import android.widget.ImageView;

public class MainActivity extends Activity {
	
	public static final String TAG = "bitirme::MainActivity";
	
	private CameraPreview camPreview;
	private ImageView myCameraPreview = null;
	private FrameLayout mainLayout;
	private int previewSizeWidth = 1280;
	private int previewSizeHeight = 720;
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
		
		SurfaceView camView = new SurfaceView(this);
		SurfaceHolder camHolder = camView.getHolder();
		camPreview = new CameraPreview(previewSizeWidth, previewSizeHeight, myCameraPreview);
		camHolder.addCallback(camPreview);
		camHolder.setType(SurfaceHolder.SURFACE_TYPE_PUSH_BUFFERS);
		
		mainLayout = (FrameLayout) findViewById(R.id.frameLayout);
		mainLayout.addView(camView, new LayoutParams(previewSizeWidth, previewSizeHeight));
		mainLayout.addView(myCameraPreview, new LayoutParams(previewSizeWidth, previewSizeHeight));
	}
	
	protected void onPause() {
		if (camPreview != null) {
			camPreview.onPause();
		}
		
		super.onPause();
	}
}
