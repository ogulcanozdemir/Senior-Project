/*
 * ARUtils.hpp
 *
 *  Created on: May 5, 2013
 *      Author: oulcan
 */

#ifndef ARUTILS_HPP_
#define ARUTILS_HPP_

#include <vector>
#include <opencv2/opencv.hpp>

float perimeter(const std::vector<cv::Point2f> &a);
bool isInto(cv::Mat &contour, std::vector<cv::Point2f> &b);

struct Matrix44
{
	union {
		float data[16];
		float matrix[4][4];
	};

	Matrix44 getTransposed() const;
	Matrix44 getInvertedRT() const;
	static Matrix44 identity();
};

struct Matrix33
{
	union {
		float data[9];
		float matrix[3][3];
	};

	Matrix33 getTransposed() const;
	static Matrix33 identity();
};

struct Vector4
{
	float data[4];
};

struct Vector3
{
	float data[3];

	static Vector3 zero();
	Vector3 operator-() const;
};

struct Transformation
{
	Transformation();
	Transformation(const Matrix33& r, const Vector3& t);

	Matrix33& r();
	Vector3& t();

	const Matrix33& r() const;
	const Vector3& t() const;

	Matrix44 getMat44() const;

	Transformation getInverted() const;

private:
	Matrix33 m_rotation;
	Vector3 m_translation;
};

#endif /* ARUTILS_HPP_ */
