/*
 * ARLocal.h
 *
 *  Created on: May 7, 2013
 *      Author: oulcan
 */

#ifndef ARLOCAL_H_
#define ARLOCAL_H_

#include <ARCameraCalibration.hpp>

std::vector<Transformation> m_transformation;
ARCameraCalibration calibration = ARCameraCalibration(564.069422695812023,
														568.677104860846953,
														306.625485055892511,
														246.996780029507590);

#endif /* ARLOCAL_H_ */
