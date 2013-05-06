/*
 * ARCameraCalibration.cpp
 *
 *  Created on: May 6, 2013
 *      Author: oulcan
 */

#include "ARCameraCalibration.hpp"

ARCameraCalibration::ARCameraCalibration()
{

}

ARCameraCalibration::ARCameraCalibration(float fx, float fy, float cx, float cy)
{
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			m_intrinsic.matrix[i][j] = 0;

	m_intrinsic.matrix[0][0] = fx;
	m_intrinsic.matrix[1][1] = fy;
	m_intrinsic.matrix[0][2] = cx;
	m_intrinsic.matrix[1][2] = cy;

	for (int i = 0; i < 4; i++)
		m_distorsion.data[i] = 0;
}

ARCameraCalibration::ARCameraCalibration(float fx, float fy, float cx, float cy, float distorsionCoeff[4])
{
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			m_intrinsic.matrix[i][j] = 0;

	m_intrinsic.matrix[0][0] = fx;
	m_intrinsic.matrix[1][1] = fy;
	m_intrinsic.matrix[0][2] = cx;
	m_intrinsic.matrix[1][2] = cy;

	for (int i = 0; i < 4; i++)
		m_distorsion.data[i] = distorsionCoeff[i];
}

void ARCameraCalibration::getMatrix34(float cparam[3][4]) const
{
	for (int j=0; j<3; j++)
	    for (int i=0; i<3; i++)
	      cparam[i][j] = m_intrinsic.matrix[i][j];

	for (int i=0; i<4; i++)
		cparam[3][i] = m_distorsion.data[i];
}

const Matrix33& ARCameraCalibration::getIntrinsic() const
{
	return m_intrinsic;
}

const Vector4& ARCameraCalibration::getDistorsion() const
{
	return m_distorsion;
}
