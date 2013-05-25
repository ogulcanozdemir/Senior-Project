package com.augmentedreality;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.opengl.GLSurfaceView.Renderer;
import android.opengl.GLU;

public class ARRenderer implements Renderer {
	
	private ARCube mCube = new ARCube();
	private float mCubeRotation;

	@Override
	public void onDrawFrame(GL10 gl) {
		nativeOnDrawFrame();
		
		/*
		gl.glClear(GL10.GL_COLOR_BUFFER_BIT | GL10.GL_DEPTH_BUFFER_BIT);
		gl.glLoadIdentity();
		
		gl.glTranslatef(0.0f, 0.0f, -10.0f);
		gl.glRotatef(mCubeRotation, 1.0f, 1.0f, 1.0f);
		
		mCube.draw(gl);
		gl.glLoadIdentity();
		
		mCubeRotation -= 0.15f;
		*/
	}

	@Override
	public void onSurfaceChanged(GL10 gl, int width, int height) {
		nativeOnSurfaceChanged(width, height);
		
		/*
		gl.glViewport(0, 0, width, height);
		gl.glMatrixMode(GL10.GL_PROJECTION);
		gl.glLoadIdentity();
		GLU.gluPerspective(gl, 45.0f, (float)width / (float)height, 0.1f, 100.0f);
		gl.glViewport(0, 0, width, height);
		
		gl.glMatrixMode(GL10.GL_MODELVIEW);
		gl.glLoadIdentity();
		*/
	}

	@Override
	public void onSurfaceCreated(GL10 gl, EGLConfig config) {
		nativeOnSurfaceCreated();
		
		/*
		gl.glDisable(GL10.GL_DITHER);
		gl.glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		gl.glEnable(GL10.GL_CULL_FACE);
		gl.glShadeModel(GL10.GL_SMOOTH);
		gl.glEnable(GL10.GL_DEPTH_TEST);
		
		gl.glHint(GL10.GL_PERSPECTIVE_CORRECTION_HINT, GL10.GL_FASTEST);
		*/
	}
	
	private static native void nativeOnSurfaceCreated();
	private static native void nativeOnSurfaceChanged(int w, int h);
	private static native void nativeOnDrawFrame();
	
}
