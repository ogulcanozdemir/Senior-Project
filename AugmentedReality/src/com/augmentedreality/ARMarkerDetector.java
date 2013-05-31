package com.augmentedreality;

import org.opencv.core.Mat;

public class ARMarkerDetector {
	
	public ARMarkerDetector() {
		
	}
	
	public void detect(Mat imageRgba, Mat imageGray, Mat output) {
		nativeMarkerDetect(imageRgba.getNativeObjAddr(), imageGray.getNativeObjAddr(), output.getNativeObjAddr());
	}
	
	private static native void nativeMarkerDetect(long rgbaImage, long inputImage, long output);
	
}
