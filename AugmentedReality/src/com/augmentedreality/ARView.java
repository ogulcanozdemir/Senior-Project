package com.augmentedreality;

import android.content.Context;
import android.graphics.Bitmap;

public class ARView extends ARViewBase {

	public ARView(Context context) {
		super(context);
	}

	@Override
	protected Bitmap processFrame(byte[] data) {
		int frameSize = getFrameWidth() * getFrameHeight();
		int[] rgba = new int[frameSize];

		//Native.FindFeatures(getFrameWidth(), getFrameHeight(), data, rgba);
		Native.applyCanny(getFrameWidth(), getFrameHeight(), data, rgba);
		
		Bitmap bmp = Bitmap.createBitmap(getFrameWidth(), getFrameHeight(), Bitmap.Config.ARGB_8888);
		bmp.setPixels(rgba, 0, getFrameWidth(), 0, 0, getFrameWidth(), getFrameHeight());

		return bmp;
	}
}
