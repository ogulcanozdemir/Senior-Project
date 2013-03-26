#include <jni.h>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgproc/imgproc_c.h>
#include <opencv2/core/core.hpp>
#include <vector>

using namespace std;
using namespace cv;

extern "C" {
//JNIEXPORT void JNICALL Java_com_augmentedreality_Native_applyCanny(JNIEnv* env, jobject, jint width, jint height, jbyteArray NV21FrameData, jintArray outPixels);
//JNIEXPORT void JNICALL Java_com_augmentedreality_Native_FindFeatures(JNIEnv* env, jobject, jint width, jint height, jbyteArray yuv, jintArray rgba);
JNIEXPORT void JNICALL Java_com_augmentedreality_CameraPreview_applyCanny(JNIEnv* env, jobject, jint width, jint height, jbyteArray NV21FrameData, jintArray outPixels);
JNIEXPORT void JNICALL Java_com_augmentedreality_CameraPreview_applySobel(JNIEnv* env, jobject, jint width, jint height, jbyteArray NV21FrameData, jintArray outPixels);

Mat *mCanny = NULL;
Mat *mSobel = NULL;

/*
JNIEXPORT void JNICALL Java_com_augmentedreality_Native_FindFeatures(JNIEnv* env, jobject, jint width, jint height, jbyteArray yuv, jintArray rgba)
{
	jbyte* _yuv = env->GetByteArrayElements(yuv, 0);
	jint* _rgba = env->GetIntArrayElements(rgba, 0);

	Mat myuv(height + height/2, width, CV_8UC1, (unsigned char *)_yuv);
	Mat mrgba(height, width, CV_8UC4, (unsigned char *)_rgba);
	Mat mgray(height, width, CV_8UC1, (unsigned char *)_yuv);

	cvtColor(myuv, mrgba, CV_YUV420sp2BGRA, 4);
	vector<KeyPoint> v;

	FastFeatureDetector detector(50);
	detector.detect(mgray, v);
	for( size_t i = 0; i < v.size(); i++ )
		circle(mrgba, Point(v[i].pt.x, v[i].pt.y), 10, Scalar(0,0,255,255));

	env->ReleaseIntArrayElements(rgba, _rgba, 0);
	env->ReleaseByteArrayElements(yuv, _yuv, 0);
}
*/

JNIEXPORT void JNICALL Java_com_augmentedreality_CameraPreview_applyCanny(JNIEnv* env, jobject, jint width, jint height, jbyteArray NV21FrameData, jintArray outPixels)
{
	jbyte *pNV21FrameData = env->GetByteArrayElements(NV21FrameData, 0);
	jint *poutPixels = env->GetIntArrayElements(outPixels, 0);

	if (mCanny == NULL) {
		mCanny = new Mat(height, width, CV_8UC1);
	}

	Mat mGray(height, width, CV_8UC1, (unsigned char *)pNV21FrameData);
	Mat mResult(height, width, CV_8UC4, (unsigned char *)poutPixels);

	IplImage srcImg = mGray;
	IplImage cannyImg = *mCanny;
	IplImage resultImg = mResult;

	cvCanny(&srcImg, &cannyImg, 80, 100, 3);
	cvCvtColor(&cannyImg, &resultImg, CV_GRAY2BGRA);

	env->ReleaseByteArrayElements(NV21FrameData, pNV21FrameData, 0);
	env->ReleaseIntArrayElements(outPixels, poutPixels, 0);
}

JNIEXPORT void JNICALL Java_com_augmentedreality_CameraPreview_applySobel(JNIEnv* env, jobject, jint width, jint height, jbyteArray NV21FrameData, jintArray outPixels)
{
	jbyte *pNV21FrameData = env->GetByteArrayElements(NV21FrameData, 0);
	jint *poutPixels = env->GetIntArrayElements(outPixels, 0);

	if (mSobel == NULL) {
			mSobel = new Mat(height, width, CV_8UC1);
		}

	Mat mGray(height, width, CV_8UC1, (unsigned char *)pNV21FrameData);
	Mat mResult(height, width, CV_8UC4, (unsigned char *)poutPixels);

	IplImage srcImg = mGray;
	IplImage sobelImg = *mSobel;
	IplImage resultImg = mResult;

	cvSobel(&srcImg, &sobelImg, 1.0, 1.0, 3);
	cvCvtColor(&sobelImg, &resultImg, CV_GRAY2BGRA);

	env->ReleaseByteArrayElements(NV21FrameData, pNV21FrameData, 0);
	env->ReleaseIntArrayElements(outPixels, poutPixels, 0);
}

}
