package com.augmentedreality;

import android.os.Bundle;
import android.app.Activity;
import android.util.Log;
import android.view.Window;

public class MainActivity extends Activity {
	
	public static final String TAG = "bitirme::MainActivity";
	
	public MainActivity() {
		Log.i(TAG, "Instantiated new " + this.getClass());
	}

	@Override
	protected void onCreate(Bundle savedInstanceState) {
		Log.i(TAG, "onCreate");
		super.onCreate(savedInstanceState);
		Native.loadLibs();
		requestWindowFeature(Window.FEATURE_NO_TITLE);
		setContentView(new ARView(this));
		
		
		//setContentView(R.layout.activity_main);
	}
}
