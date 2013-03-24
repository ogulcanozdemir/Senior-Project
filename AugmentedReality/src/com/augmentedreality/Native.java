package com.augmentedreality;

public class Native {
	
	public static void loadLibs() {
		System.loadLibrary("opencv_java");
		System.loadLibrary("augmentedreality");
	}
	
	public static native void FindFeatures(int width, int height, byte[] yuv, int[] rgba);
	public static native void applyCanny(int width, int height, byte[] yuv, int[] rgba);
	public static native void applySobel(int width, int height, byte[] yuv, int[] rgba);
}
