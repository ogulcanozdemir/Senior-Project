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
/*
ARCameraCalibration calibration = ARCameraCalibration(564.069422695812023,
														568.677104860846953,
														306.625485055892511,
														246.996780029507590);
*/
float de[4] = {0.024203911405420, -0.23906865978367, -0.010398966376716, 0.002359776777142};
ARCameraCalibration calibration = ARCameraCalibration(542.99853964032322,
														545.28532834228171,
														292.85272408698438,
														233.10051081071729, de);
#endif /* ARLOCAL_H_ */
