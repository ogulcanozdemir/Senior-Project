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


ARCameraCalibration calibration = ARCameraCalibration(600.069422695812023,
														610.677104860846953,
														306.625485055892511,
														246.996780029507590);

/*
float de[4] = {0.073258700888605, -0.182417087413563, -0.003800524631021, 0.004064504740826};

ARCameraCalibration calibration = ARCameraCalibration(528.016493196638860,
														526.245965375651740,
														324.810905533526350,
														242.777645011713330, de);
*/
#endif /* ARLOCAL_H_ */
