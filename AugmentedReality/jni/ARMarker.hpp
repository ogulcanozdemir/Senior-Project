/*
 * ARMarker.hpp
 *
 *  Created on: May 5, 2013
 *      Author: oulcan
 */

#ifndef ARMARKER_HPP_
#define ARMARKER_HPP_

#include <vector>
#include <iostream>
#include <opencv2/opencv.hpp>

#include "ARUtils.hpp"

class ARMarker
{
public:
	ARMarker();

	friend bool operator<(const ARMarker &M1, const ARMarker &M2);
	friend std::ostream &operator<<(std::ostream &str, const ARMarker &M);

	static cv::Mat rotate(cv::Mat in);
	static int hammDistMarker(cv::Mat bits);
	static int mat2id(const cv::Mat &bits);
	static int getMarkerId(cv::Mat &in, int &nRotations);

public:
	int id;
	std::vector<cv::Point2f> points;
	Transformation transformation;
	void drawContour(cv::Mat& image, cv::Scalar color = CV_RGB(250, 0, 0)) const;
};

#endif /* ARMARKER_HPP_ */
