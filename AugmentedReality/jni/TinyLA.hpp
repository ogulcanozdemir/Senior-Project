/*
 * TinyLA.hpp
 *
 *  Created on: 21 Nis 2013
 *      Author: oulcan
 */

#ifndef TINYLA_HPP_
#define TINYLA_HPP_

#include <vector>
#include <opencv2/opencv.hpp>

float perimeter(const std::vector<cv::Point2f> &a);

bool isInto(cv::Mat &contour, std::vector<cv::Point2f> &b);

#endif /* TINYLA_HPP_ */
