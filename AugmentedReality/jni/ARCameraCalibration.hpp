/*
 * ARCameraCalibration.hpp
 *
 *  Created on: May 6, 2013
 *      Author: oulcan
 */

#ifndef ARCAMERACALIBRATION_HPP_
#define ARCAMERACALIBRATION_HPP_

#include "ARUtils.hpp"

class ARCameraCalibration
{
public:
	ARCameraCalibration();
	ARCameraCalibration(float fx, float fy, float cx, float cy);
	ARCameraCalibration(float fx, float fy, float cx, float cy, float distorsionCoeff[4]);

	void getMatrix34(float cparam[3][4]) const;

	const Matrix33& getIntrinsic() const;
	const Vector4& getDistorsion() const;

private:

	Matrix33 m_intrinsic;
	Vector3 m_distorsion;
};


#endif /* ARCAMERACALIBRATION_HPP_ */
