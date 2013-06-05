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

float de[4] = {0.066925085064801, -0.152828243358234, 0.008534850212039, 0.011852057848444};
ARCameraCalibration calibration = ARCameraCalibration(1071.892471410041700,
		1071.255128598984400,
		664.596696775594860,
		393.459433499745050, de);

/*
float de[4] = {0.066925085064801, -0.152828243358234, 0.008534850212039, 0.011852057848444};

ARCameraCalibration calibration = ARCameraCalibration(528.016493196638860,
														526.245965375651740,
														324.810905533526350,
														242.777645011713330, de);
*/
#endif /* ARLOCAL_H_ */
