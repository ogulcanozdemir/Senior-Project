#include <AugmentedReality.hpp>

#define FOCAL_LENGTH 250
#define CUBE_SIZE 10

JNIEXPORT void JNICALL Java_com_augmentedreality_ARMarkerDetector_nativeMarkerDetect(
		JNIEnv *jenv, jclass, jlong imageGray, jlong output) {
	LOGD("Java_com_augmentedreality_ARMarkerDetector_nativeMarkerDetect enter");
	Mat &inputMat = *(Mat *) imageGray;
	Mat &outputMat = *(Mat *) output;

	// image binarization
	Mat threshold(inputMat.size(), inputMat.type());
	performThreshold(inputMat, threshold);

	// contour finding
	ContoursVector contours;
	findContours(threshold, contours, threshold.cols / 5);

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
	outputMat = inputMat.clone();
	for (size_t i = 0; i < detectedMarkers.size(); i++) {
		//detectedMarkers[i].drawContour(outputMat, cvScalar(250, 0, 0));
		detectedMarkers[i].drawContour(inputMat, cvScalar(250, 0, 0));
	}

	if (detectedMarkers.size() != 0) {
		CvPOSITObject *positObject;

		vector<CvPoint3D32f> modelPoints;
		modelPoints.push_back(cvPoint3D32f(0.0f, 0.0f, 0.0f));
		modelPoints.push_back(cvPoint3D32f(0.0f, 0.0f, CUBE_SIZE));
		modelPoints.push_back(cvPoint3D32f(CUBE_SIZE, 0.0f, 0.0f));
		modelPoints.push_back(cvPoint3D32f(0.0f, CUBE_SIZE, 0.0f));
		LOGD("debug-1");


		vector<CvPoint2D32f> imagePoints;
		imagePoints.push_back(
				cvPoint2D32f(detectedMarkers[0].points[0].x,
						detectedMarkers[0].points[0].y));
		imagePoints.push_back(
				cvPoint2D32f(detectedMarkers[0].points[1].x,
						detectedMarkers[0].points[2].y));
		imagePoints.push_back(
				cvPoint2D32f(detectedMarkers[0].points[3].x,
						detectedMarkers[0].points[3].y));
		imagePoints.push_back(
				cvPoint2D32f(detectedMarkers[0].points[4].x,
						detectedMarkers[0].points[4].y));

		LOGD("debug");
		positObject = cvCreatePOSITObject(&modelPoints[0],
				(int) modelPoints.size());
		LOGD("debug2");
		CvMatr32f rotation_matrix = new float[9];
		LOGD("debug3");
		CvVect32f translation_vector = new float[3];
		LOGD("debug4");
		CvTermCriteria criteria = cvTermCriteria(
				CV_TERMCRIT_EPS | CV_TERMCRIT_ITER, 100, 1.0e-4f);
		LOGD("debug5");
		cvPOSIT(positObject, &imagePoints[0], FOCAL_LENGTH, criteria,
				rotation_matrix, translation_vector);
		LOGD("debug6");
		LOGD("source model points...");
		for (size_t p = 0; p < modelPoints.size(); p++)
			LOGD(
					"%f , %f , %f", modelPoints[p].x, modelPoints[p].y, modelPoints[p].z);

		LOGD("source image points...");
		for (size_t p = 0; p < imagePoints.size(); p++)
			LOGD("%f , %f , %f", imagePoints[p].x, imagePoints[p].y);

		LOGD("estimated position...");
		for (size_t p = 0; p < 3; p++)
			LOGD(
					"%f , %f , %f", rotation_matrix[p * 3], rotation_matrix[p * 3 + 1], rotation_matrix[p * 3 + 2]);

		LOGD("estimated translation...");
		LOGD(
				"%f , %f , %f", translation_vector[0], translation_vector[1], translation_vector[2]);

		vector<CvPoint2D32f> projectedPoints;
		for (size_t p = 0; p < modelPoints.size(); p++) {
			CvPoint3D32f point3D;
			point3D.x = rotation_matrix[0] * modelPoints[p].x
					+ rotation_matrix[1] * modelPoints[p].y
					+ rotation_matrix[2] * modelPoints[p].z
					+ translation_vector[0];

			point3D.y = rotation_matrix[3] * modelPoints[p].x
					+ rotation_matrix[4] * modelPoints[p].y
					+ rotation_matrix[5] * modelPoints[p].z
					+ translation_vector[1];

			point3D.z = rotation_matrix[6] * modelPoints[p].x
					+ rotation_matrix[7] * modelPoints[p].y
					+ rotation_matrix[8] * modelPoints[p].z
					+ translation_vector[2];

			CvPoint2D32f point2D = cvPoint2D32f(0.0, 0.0);
			if (point3D.z != 0) {
				point2D.x = FOCAL_LENGTH * point3D.x / point3D.z;
				point2D.y = FOCAL_LENGTH * point3D.y / point3D.z;
			}
			projectedPoints.push_back(point2D);
		}

		int centreX = inputMat.cols / 2;
		int centreY = inputMat.rows / 2;
		for (size_t p = 0; p < modelPoints.size(); p++)
			circle(inputMat,
					cvPoint(centreX + (int) imagePoints[p].x,
							centreY - (int) imagePoints[p].y), 8,
					CV_RGB(255, 0, 0));

		line(inputMat,
				cvPoint(centreX + (int) projectedPoints[0].x,
						centreY - (int) projectedPoints[0].y),
				cvPoint(centreX + (int) projectedPoints[1].x,
						centreY - (int) projectedPoints[1].y),
				CV_RGB(0, 0, 255), 2);
		line(inputMat,
				cvPoint(centreX + (int) projectedPoints[0].x,
						centreY - (int) projectedPoints[0].y),
				cvPoint(centreX + (int) projectedPoints[2].x,
						centreY - (int) projectedPoints[2].y),
				CV_RGB(255, 0, 0), 2);
		line(inputMat,
				cvPoint(centreX + (int) projectedPoints[0].x,
						centreY - (int) projectedPoints[0].y),
				cvPoint(centreX + (int) projectedPoints[3].x,
						centreY - (int) projectedPoints[3].y),
				CV_RGB(0, 0, 255), 2);

		LOGD("estimated image points...");
		for (size_t p = 0; p < projectedPoints.size(); p++) {
			circle(inputMat,
					cvPoint(centreX + (int) projectedPoints[p].x,
							centreY - (int) projectedPoints[p].y), 3,
					CV_RGB(255, 255, 255), -1);
			LOGD("%f - %f", projectedPoints[p].x, projectedPoints[p].y);
		}

		delete rotation_matrix;
		delete translation_vector;
		cvReleasePOSITObject(&positObject);
		modelPoints.clear();
		imagePoints.clear();
		projectedPoints.clear();
	}

	/*
	 // pose estimation
	 estimatePosition(detectedMarkers);

	 sort(detectedMarkers.begin(), detectedMarkers.end());

	 m_transformation.clear();
	 for (size_t i = 0; i < detectedMarkers.size(); i++)
	 m_transformation.push_back(detectedMarkers[i].transformation);

	 drawFrame(inputMat);
	 */
	// clearing vectors for next frame
	//camMatrix.release();
	//distCoeff.release();
	detectedMarkers.clear();
	markerCorners2d.clear();
	markerCorners3d.clear();

	LOGD("Java_com_augmentedreality_ARMarkerDetector_nativeMarkerDetect exit");
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
		LOGD("marker id => %d", id);
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
