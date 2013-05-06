/*
 * ARMarker.cpp
 *
 *  Created on: May 5, 2013
 *      Author: oulcan
 */

#include "ARMarker.hpp"

#include <android/log.h>
#define LOG_TAG "ARMarker"
#define LOGD(...) ((void)__android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__))

ARMarker::ARMarker(): id(-1)
{
	// constructor
}

bool operator<(const ARMarker &M1, const ARMarker &M2)
{
	return M1.id<M2.id;
}

cv::Mat ARMarker::rotate(cv::Mat in)
{
	cv::Mat out;
	in.copyTo(out);
	for (int i = 0; i < in.rows; i++) {
		for (int j = 0; j < in.cols; j++) {
			out.at<uchar>(i,j) = in.at<uchar>(in.cols-j-1, i);
		}
	}

	return out;
}

int ARMarker::hammDistMarker(cv::Mat bits)
{
	int ids[4][5] =
	{
			{1,0,0,0,0},
			{1,0,1,1,1},
			{0,1,0,0,1},
			{0,1,1,1,0}
	};

	int dist = 0;

	for (int y = 0; y < 5; y++) {
		int minSum = 1e5;

		for (int p = 0; p < 4; p++) {
			int sum = 0;

			for (int x = 0; x < 5; x++)
				sum += bits.at<uchar>(y,x) == ids[p][x] ? 0 : 1;

			if (minSum > sum)
				minSum = sum;
		}

		dist += minSum;
	}

	return dist;
}

int ARMarker::mat2id(const cv::Mat &bits)
{
	int val = 0;
	for (int y = 0; y < 5

	; y++) {
		val <<= 1;

		if (bits.at<uchar>(y,1))
			val |= 1;

		if (bits.at<uchar>(y,3))
			val |= 1;
	}

	return val;
}

int ARMarker::getMarkerId(cv::Mat &markerImage, int &nRotations)
{
	assert(markerImage.rows == markerImage.cols);
	assert(markerImage.type() == CV_8UC1);

	cv::Mat gray = markerImage;
	cv::threshold(gray, gray, 125, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);

	int cellSize = markerImage.rows / 7;
	for (int y = 0; y < 7; y++) {
		int inc = 6;

		if (y == 0 || y == 6)
			inc = 1;

		for (int x = 0; x < 7; x+= inc) {
			int cellX = x * cellSize;
			int cellY = y * cellSize;

			cv::Mat cell = gray(cv::Rect(cellX, cellY, cellSize, cellSize));

			int numberZeros = cv::countNonZero(cell);
			if (numberZeros > (cellSize * cellSize) / 2)
				return -1;
		}
	}

	// get info
	cv::Mat bitMatrix = cv::Mat::zeros(5, 5, CV_8UC1);
	for (int y = 0; y < 5; y++) {
		for (int x = 0; x < 5; x++) {
			int cellX = (x+1) * cellSize;
			int cellY = (y+1) * cellSize;

			cv::Mat cell = gray(cv::Rect(cellX, cellY, cellSize, cellSize));

			int numberZeros = cv::countNonZero(cell);
			if (numberZeros > (cellSize * cellSize) / 2)
				bitMatrix.at<uchar>(y,x) = 1;
		}
	}

	// possible rotations
	cv::Mat rotations[4];
	int distances[4];

	rotations[0] = bitMatrix;
	distances[0] = hammDistMarker(rotations[0]);
	std::pair<int, int> minDist(distances[0], 0);
	for (int i = 1; i < 4; i++) {
		rotations[i] = rotate(rotations[i-1]);
		distances[i] = hammDistMarker(rotations[i]);

		if (distances[i] < minDist.first) {
			minDist.first = distances[i];
			minDist.second = i;
		}
	}

	nRotations = minDist.second;
	if (minDist.first == 0)
		return mat2id(rotations[minDist.second]);

	return -1;
}

void ARMarker::drawContour(cv::Mat& image, cv::Scalar color) const
{
	float thickness = 2;

	for (int i = 0; i < 4; i++) {
		cv::line(image, points[i % 4], points[(i+1) % 4], color, thickness, CV_AA);
	}
}




