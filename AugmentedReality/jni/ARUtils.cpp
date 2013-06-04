/*
 * Utils.cpp
 *
 *  Created on: May 5, 2013
 *      Author: oulcan
 */

#include "ARUtils.hpp"

float perimeter(const std::vector<cv::Point2f> &a)
{
	float sum = 0, dx, dy;

	for (size_t i = 0; i < a.size(); i++) {
		size_t i2 = (i+1) % a.size();

		dx = a[i].x - a[i2].x;
		dy = a[i].y - a[i2].y;

		sum += sqrt(dx * dx + dy * dy);
	}

	return sum;
}

bool isInto(cv::Mat &contour, std::vector<cv::Point2f> &b)
{
	for (size_t i = 0; i < b.size(); i++)
		if (cv::pointPolygonTest(contour, b[i], false) > 0)
			return true;

	return false;
}

Matrix44 Matrix44::getTransposed() const
{
	Matrix44 m;

	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			m.matrix[i][j] = matrix[j][i];

	return m;
}

Matrix44 Matrix44::identity()
{
	Matrix44 e;

	for (int i = 0; i < 4; i++)
		for (int j = 0; j < 4; j++)
			e.matrix[i][j] = i == j ? 1 : 0;

	return e;
}

Matrix44 Matrix44::getInvertedRT() const
{
	Matrix44 mat = identity();

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++)
			mat.matrix[j][i] = matrix[i][j];

		mat.matrix[3][i] = - matrix[3][i];
	}

	return mat;
}

Matrix33 Matrix33::identity()
{
	Matrix33 e;

	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			e.matrix[i][j] = i == j ? 1 : 0;

	return e;
}

Matrix33 Matrix33::getTransposed() const
{
	Matrix33 mat;

	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			mat.matrix[i][j] = matrix[j][i];

	return mat;
}

Vector3 Vector3::zero()
{
	Vector3 vector = {0,0,0};
	return vector;
}

Vector3 Vector3::operator -() const
{
	Vector3 vector = { -data[0], -data[1], -data[2] };
	return vector;
}

Transformation::Transformation(): m_rotation(Matrix33::identity()), m_translation(Vector3::zero())
{

}

Transformation::Transformation(const Matrix33& r, const Vector3& t): m_rotation(r), m_translation(t)
{

}

Matrix33& Transformation::r()
{
	return m_rotation;
}

Vector3& Transformation::t()
{
	return m_translation;
}

const Matrix33& Transformation::r() const
{
	return m_rotation;
}

const Vector3& Transformation::t() const
{
	return m_translation;
}

Matrix44 Transformation::getMat44() const
{
	Matrix44 mat = Matrix44::identity();

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++)
			mat.matrix[j][i] = m_rotation.matrix[j][i];

		mat.matrix[3][i] = m_translation.data[i];
	}

	return mat;
}

Transformation Transformation::getInverted() const
{
	return Transformation(m_rotation.getTransposed(), -m_translation);
}


