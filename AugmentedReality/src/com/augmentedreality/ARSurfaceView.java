package com.augmentedreality;

import android.content.Context;
import android.graphics.PixelFormat;
import android.opengl.GLSurfaceView;
import android.view.MotionEvent;

public class ARSurfaceView extends GLSurfaceView {
	
	private ARRenderer mRenderer;
	private float mPreviousX;
	private float mPreviousY;
	private final float TOUCH_SCALE_FACTOR = 180.0f / 320;

	public ARSurfaceView(Context context) {
		super(context);
		
		this.setEGLConfigChooser(8, 8, 8, 8, 16, 0);
		this.setZOrderOnTop(true);
		mRenderer = new ARRenderer(context);
		this.setRenderer(mRenderer);
		this.setRenderMode(GLSurfaceView.RENDERMODE_WHEN_DIRTY);
		this.getHolder().setFormat(PixelFormat.TRANSLUCENT);
	}
	
	
	public boolean onTouchEvent(final MotionEvent event) {
		float x = event.getX();
		float y = event.getY();
		
		switch (event.getAction()) {
		case MotionEvent.ACTION_MOVE:
			float dx = x - mPreviousX;
			float dy = y - mPreviousY;
			mRenderer.mAngleX += dx * TOUCH_SCALE_FACTOR;
			mRenderer.mAngleY += dy * TOUCH_SCALE_FACTOR;
			requestRender();
		}
		
		mPreviousX = x;
		mPreviousY = y;
		return true;
	}

}
