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
ARCameraCalibration calibration = ARCameraCalibration(6.24860291e+02 * (320./176.),
														6.24860291e+02 * (240./144.),
														320 * 0.5f,
														240 * 0.5);

#endif /* ARLOCAL_H_ */
