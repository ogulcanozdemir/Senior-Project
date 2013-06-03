package com.augmentedreality;

import java.io.BufferedReader;
import java.io.InputStream;
import java.io.InputStreamReader;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.content.Context;
import android.opengl.GLSurfaceView;

public class ARRenderer implements GLSurfaceView.Renderer {

	public float mAngleX;
	public float mAngleY;
	private Context mContext;
	
	public ARRenderer(Context pContext) {
		super();
		mContext = pContext;
	}
	
	@Override
	public void onDrawFrame(GL10 gl) {
		nativeDrawGraphics(mAngleX, mAngleY);
	}

	@Override
	public void onSurfaceChanged(GL10 gl, int width, int height) {
		nativeSurfaceChanged(width, height);
	}

	@Override
	public void onSurfaceCreated(GL10 gl, EGLConfig config) {
		nativeInitGL20();
	}
	
	private static native void nativeInitGL20();	
	private static native void nativeSurfaceChanged(int width, int height);
	private static native void nativeDrawGraphics(float angleX, float angleY);
	
}
