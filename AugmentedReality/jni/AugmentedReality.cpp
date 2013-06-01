#include <AugmentedReality.hpp>

jint JNI_OnLoad(JavaVM* pVm, void *reserved)
{
	JNIEnv* env;
	if (pVm->GetEnv((void **)&env, JNI_VERSION_1_6) != JNI_OK) {
		return -1;
	}

	JNINativeMethod nm[4];
	nm[0].name = "nativeInitGL20";
	nm[0].signature = "(Ljava/lang/String;Ljava/lang/String;)V";
	nm[0].fnPtr = (void *)nativeInitGL20;
	nm[1].name = "nativeDrawGraphics";
	nm[1].signature = "(FF)V";
	nm[1].fnPtr = (void *)nativeDrawGraphics;
	nm[2].name = "nativeSurfaceChanged";
	nm[2].signature = "(II)V";
	nm[2].fnPtr = (void *)nativeSurfaceChanged;

	jclass cls = env->FindClass("com/augmentedreality/ARRenderer");
	env->RegisterNatives(cls, nm, 3);
	return JNI_VERSION_1_6;
}

void nativeInitGL20(JNIEnv* env, jclass clazz, jstring vertexShaderStr, jstring fragmentShaderStr) {
	glGenTextures(1, &m_backgroundTextureId);
	glBindTexture(GL_TEXTURE_2D, m_backgroundTextureId);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glDisable(GL_DITHER);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClearDepthf(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glViewport(0, 0, width, height);
}

void nativeSurfaceChanged(JNIEnv* env, jclass clazz, int gwidth, int gheight) {
	glViewport(0, 0, gwidth, gheight);
	width = gwidth;
	height = gheight;
}

void drawFrame()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glViewport(0, 0, width, height);
	drawBackground();
	drawAR();

	int glErCode = glGetError();
	if (glErCode != GL_NO_ERROR) {
		LOGD("error code: %x", glErCode);
	}
}

void nativeDrawGraphics(JNIEnv* env, jclass clazz, float pAngleX, float pAngleY) {
	updateBackground(rgbaMat);
	drawFrame();
}

JNIEXPORT void JNICALL Java_com_augmentedreality_ARMarkerDetector_nativeMarkerDetect(
		JNIEnv *jenv, jclass,jlong imageRgba, jlong imageGray, jlong output) {
	//LOGD("Java_com_augmentedreality_ARMarkerDetector_nativeMarkerDetect enter");
	Mat &inputMat = *(Mat *) imageGray;
	Mat &outputMat = *(Mat *) output;
	rgbaMat = *(Mat *) imageRgba;

	// image binarization
	Mat threshold(inputMat.size(), inputMat.type());
	performThreshold(inputMat, threshold);

	// contour finding
	ContoursVector contours;
	findContours(threshold, contours, threshold.cols / 5);

//	drawContours(outputMat, contours, -1, Scalar(255), 2, CV_AA);

	// finding marker candidates in calculated contours
	MarkerVector detectedMarkers;
	findCandidates(contours, detectedMarkers);

	// markerCorners3d initialization
	markerCorners3d.push_back(Point3f(-0.5f, -0.5f, 0));
	markerCorners3d.push_back(Point3f(+0.5f, -0.5f, 0));
	markerCorners3d.push_back(Point3f(+0.5f, +0.5f, 0));
	markerCorners3d.push_back(Point3f(-0.5f, +0.5f, 0));

	// markerCorners2d initialization
	markerCorners2d.push_back(Point2f(0, 0));
	markerCorners2d.push_back(Point2f(markerSize.width - 1, 0));
	markerCorners2d.push_back(
			Point2f(markerSize.width - 1, markerSize.height - 1));
	markerCorners2d.push_back(Point2f(0, markerSize.height - 1));

	// recognize marker from marker candidates
	recognizeMarkers(inputMat, detectedMarkers);
	outputMat = rgbaMat.clone();
	for (size_t i = 0; i < detectedMarkers.size(); i++) {
		detectedMarkers[i].drawContour(outputMat, cvScalar(250, 0, 0));
		//detectedMarkers[i].drawContour(inputMat, cvScalar(250, 0, 0));
	}

	// pose estimation
	estimatePosition(detectedMarkers);

	sort(detectedMarkers.begin(), detectedMarkers.end());
/*
	if (detectedMarkers.size() != 0) {

		LOGD("rotation matrix.....");
		LOGD(
				"%f - %f - %f", detectedMarkers[0].transformation.r().matrix[0][0], detectedMarkers[0].transformation.r().matrix[0][1], detectedMarkers[0].transformation.r().matrix[0][2]);
		LOGD(
				"%f - %f - %f", detectedMarkers[0].transformation.r().matrix[1][0], detectedMarkers[0].transformation.r().matrix[1][1], detectedMarkers[0].transformation.r().matrix[1][2]);
		LOGD(
				"%f - %f - %f", detectedMarkers[0].transformation.r().matrix[2][0], detectedMarkers[0].transformation.r().matrix[2][1], detectedMarkers[0].transformation.r().matrix[2][2]);

		LOGD("translation matrix...");
		LOGD(
				"%f - %f - %f", detectedMarkers[0].transformation.t().data[0], detectedMarkers[0].transformation.t().data[1], detectedMarkers[0].transformation.t().data[2]);

	}
*/
	m_transformation.clear();
	for (size_t i = 0; i < detectedMarkers.size(); i++)
		m_transformation.push_back(detectedMarkers[i].transformation);

	// clearing vectors for next frame
	detectedMarkers.clear();
	markerCorners2d.clear();
	markerCorners3d.clear();

	//LOGD( "Java_com_augmentedreality_ARMarkerDetector_nativeMarkerDetect exit");
}

void performThreshold(const Mat& grayscale, Mat& threshold) {
	cv::threshold(grayscale, threshold, 127, 255, cv::THRESH_BINARY_INV);
	// or with adaptive threshold
	//cv::adaptiveThreshold(grayscale, threshold, 255, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY_INV, 7, 7);
}

void findContours(const Mat& threshold, ContoursVector& contours,
		int minContourPointsAllowed) {
	ContoursVector allContours;
	cv::findContours(threshold, allContours, CV_RETR_LIST,
			CV_CHAIN_APPROX_NONE);

	// optimizing contour vector
	// takes contours that bigger than minContoursPointsAllowed
	contours.clear();
	for (size_t i = 0; i < allContours.size(); i++) {
		int contourSize = allContours[i].size();
		if (contourSize > minContourPointsAllowed)
			contours.push_back(allContours[i]);
	}
}

void findCandidates(const ContoursVector& contours,
		MarkerVector& detectedMarkers) {
	PointsVector approxCurve;
	MarkerVector possibleMarkers;

	for (size_t i = 0; i < contours.size(); i++) {

		// polygon approximation
		double eps = contours[i].size() * 0.05;
		cv::approxPolyDP(contours[i], approxCurve, eps, true);

		// polygon should contains only 4 points and have to be convex
		if (approxCurve.size() == 4 && isContourConvex(approxCurve)) {

			// the distance between consecutive points
			float minDist = numeric_limits<float>::max();

			for (int j = 0; j < 4; j++) {
				Point side = approxCurve[j] - approxCurve[(j + 1) % 4];
				float squaredSideLength = side.dot(side);
				minDist = min(minDist, squaredSideLength);
			}

			// if distance is not very small
			if (minDist >= 100) {

				// saving marker candidate
				ARMarker markerCandidate;

				for (int j = 0; j < 4; j++)
					markerCandidate.points.push_back(
							Point2f(approxCurve[j].x, approxCurve[j].y));

				// sorting points in anti-clockwise order
				Point v1 = markerCandidate.points[1]
						- markerCandidate.points[0];
				Point v2 = markerCandidate.points[2]
						- markerCandidate.points[0];

				double o = (v1.x * v2.y) - (v1.y * v2.x);

				if (o < 0.0)
					swap(markerCandidate.points[1], markerCandidate.points[3]);

				possibleMarkers.push_back(markerCandidate);
			} // end of minDist if
		}
	} // end of loop

	// removing elements that if corners are too close to each other
	// detecting candidates that are too close
	vector<pair<int, int> > tooNearCandidates;
	for (size_t i = 0; i < possibleMarkers.size(); i++) {
		const ARMarker& marker1 = possibleMarkers[i];

		for (size_t j = i + 1; j < possibleMarkers.size(); j++) {
			const ARMarker& marker2 = possibleMarkers[j];

			float distSquared = 0;

			for (int c = 0; c < 4; c++) {
				Point v = marker1.points[c] - marker2.points[c];
				distSquared += v.dot(v);
			}

			distSquared /= 4;

			if (distSquared < 100)
				tooNearCandidates.push_back(pair<int, int>(i, j));
		}
	} // end of loop

	// mark for removal element
	vector<bool> removalMask(possibleMarkers.size(), false);
	for (size_t i = 0; i < tooNearCandidates.size(); i++) {
		float perimeter1 = perimeter(
				possibleMarkers[tooNearCandidates[i].first].points);
		float perimeter2 = perimeter(
				possibleMarkers[tooNearCandidates[i].second].points);

		size_t removalIndex;
		if (perimeter1 > perimeter2)
			removalIndex = tooNearCandidates[i].second;
		else
			removalIndex = tooNearCandidates[i].first;

		removalMask[removalIndex] = true;
	}

	// return candidates
	detectedMarkers.clear();
	for (size_t i = 0; i < possibleMarkers.size(); i++) {
		if (!removalMask[i])
			detectedMarkers.push_back(possibleMarkers[i]);
	}
}

void recognizeMarkers(const cv::Mat& grayscale, MarkerVector& detectedMarkers) {
	MarkerVector goodMarkers;

	// identify the markers
	for (size_t i = 0; i < detectedMarkers.size(); i++) {
		ARMarker& marker = detectedMarkers[i];

		// finding the perspective trans. that brings current marker to Rect
		cv::Mat markerTransform = cv::getPerspectiveTransform(marker.points,
				markerCorners2d);

		// transform image to canonical
		cv::warpPerspective(grayscale, canonicalMarkerImage, markerTransform,
				markerSize);

		int nRotations;
		int id = ARMarker::getMarkerId(canonicalMarkerImage, nRotations);
		//LOGD("marker id => %d", id);
		if (id != -1) {
			marker.id = id;

			// sorting points
			std::rotate(marker.points.begin(),
					marker.points.begin() + 4 - nRotations,
					marker.points.end());
			goodMarkers.push_back(marker);
		}
	}

	// refining markers
	if (goodMarkers.size() > 0) {
		vector<Point2f> preciseCorners(4 * goodMarkers.size());

		for (size_t i = 0; i < goodMarkers.size(); i++) {
			const ARMarker& marker = goodMarkers[i];

			for (int c = 0; c < 4; c++)
				preciseCorners[i * 4 + c] = marker.points[c];
		}
		cv::TermCriteria termCriteria = cv::TermCriteria(
				cv::TermCriteria::MAX_ITER | cv::TermCriteria::EPS, 30, 0.01);
		cv::cornerSubPix(grayscale, preciseCorners, Size(5, 5), Size(-1, -1),
				termCriteria);

		// copy refined markers
		for (size_t i = 0; i < goodMarkers.size(); i++) {
			ARMarker& marker = goodMarkers[i];

			for (int c = 0; c < 4; c++)
				marker.points[c] = preciseCorners[i * 4 + c];
		}
	}
	detectedMarkers = goodMarkers;
}

void estimatePosition(MarkerVector& detectedMarkers) {
	for (size_t i = 0; i < detectedMarkers.size(); i++) {

		ARMarker& marker = detectedMarkers[i];

		Mat Rvec;
		Mat_<float> Tvec;
		Mat raux, taux;

		cv::solvePnP(markerCorners3d, marker.points, camMatrix, distCoeff, raux,
				taux);
		raux.convertTo(Rvec, CV_32F);
		taux.convertTo(Tvec, CV_32F);

		Mat_<float> rotationMat(3, 3);
		cv::Rodrigues(Rvec, rotationMat);

		for (int col = 0; col < 3; col++) {
			for (int row = 0; row < 3; row++) {
				marker.transformation.r().matrix[row][col] = rotationMat(row,
						col);
			}

			marker.transformation.t().data[col] = Tvec(col);
		}

		marker.transformation = marker.transformation.getInverted();
	}
}
