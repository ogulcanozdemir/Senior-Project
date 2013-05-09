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
ARCameraCalibration calibration = ARCameraCalibration(6.24860291e+02 * (640./352.),
														6.24860291e+02 * (480./288.),
														640 * 0.5f,
														480 * 0.5f);

#endif /* ARLOCAL_H_ */
