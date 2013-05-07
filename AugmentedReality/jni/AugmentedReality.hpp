#include <jni.h>
#include <vector>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/imgproc/imgproc_c.h>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <android/log.h>

#include <ARMarker.hpp>
#include <ARUtils.hpp>
#include <ARGraphics.h>
#include <ARLocal.h>

#ifndef AUGMENTEDREALITY_HPP_
#define AUGMENTEDREALITY_HPP_

#ifdef __cplusplus

using namespace std;
using namespace cv;

//////////////////// Type definitions //////////////////////
////////////////////////////////////////////////////////////
typedef vector<Point> PointsVector;
typedef vector<PointsVector> ContoursVector;
typedef vector<ARMarker> MarkerVector;

///////////////////// Definitions //////////////////////////
////////////////////////////////////////////////////////////
#define DEBUG 0
#define LOG_TAG "AugmentedReality/native"
#define LOGD(...) ((void)__android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__))

//////////////////// Global variables //////////////////////
////////////////////////////////////////////////////////////
std::vector<cv::Point3f> markerCorners3d;
Size markerSize(100, 100);
std::vector<cv::Point2f> markerCorners2d;
Mat canonicalMarkerImage;

Mat camMatrix;
Mat distCoeff;

//////////////////// Function Prototypes ///////////////////
////////////////////////////////////////////////////////////

// performs threshold with THRESH_BINARY_INV
//		or adaptive threshold with ADAPTIVE_THRESH_GAUSSIAN_C and THRESH_BINARY_INV
// to grayscale image
void performThreshold(const Mat& grayscale, Mat& threshold);

// finds contours of thresholded image with CV_RETR_LIST and CV_CHAIN_APPROX_NONE
void findContours(const Mat& threshold, ContoursVector& contours, int minContourPointsAllowed);

// finds marker candidates from contours
void findCandidates(const ContoursVector& contours, MarkerVector& detectedMarkers);

// recognize markers from grayscale image with detected markers
void recognizeMarkers(const cv::Mat& grayscale, MarkerVector& detectedMarkers);

// perform pose estimation to markers
void estimatePosition(MarkerVector& detectedMarkers);

//////////////////// JNI methods ///////////////////////////
////////////////////////////////////////////////////////////
extern "C" {
#endif

JNIEXPORT void JNICALL Java_com_augmentedreality_ARMarkerDetector_nativeMarkerDetect(JNIEnv *, jclass, jlong, jlong);

#ifdef __cplusplus
}
#endif



#endif /* ARMARKERDETECTOR_HPP_ */
