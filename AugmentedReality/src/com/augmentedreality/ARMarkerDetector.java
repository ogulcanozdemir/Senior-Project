package com.augmentedreality;

import org.opencv.core.Mat;

public class ARMarkerDetector {
	
	public ARMarkerDetector() {
		
	}
	
	public void detect(Mat imageGray, Mat output) {
		nativeMarkerDetect(imageGray.getNativeObjAddr(), output.getNativeObjAddr());
	}
	
	private static native void nativeMarkerDetect(long inputImage, long output);
	
}
