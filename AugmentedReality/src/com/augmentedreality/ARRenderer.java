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
		String vertexShaderStr = LoadShaderStr(mContext, R.raw.vshader);
		String fragmentShaderStr = LoadShaderStr(mContext, R.raw.fshader);
		nativeInitGL20(vertexShaderStr, fragmentShaderStr);
	}
	
	private String LoadShaderStr(Context context, int resId) {
		StringBuffer strBuf = new StringBuffer();
		try {
			InputStream inputStream = context.getResources().openRawResource(resId);
			BufferedReader in = new BufferedReader(new InputStreamReader(inputStream));
			String read = in.readLine();
			while (read != null) {
				strBuf.append(read + "\n");
				read = in.readLine();
			}
			
			strBuf.deleteCharAt(strBuf.length() - 1);
		} catch (Exception e) {
			e.printStackTrace();
		}
		
		return strBuf.toString();
	}
	
	private static native void nativeInitGL20(String vertexShaderStr, String fragmentShaderStr);	
	private static native void nativeSurfaceChanged(int width, int height);
	private static native void nativeDrawGraphics(float angleX, float angleY);
	
}
