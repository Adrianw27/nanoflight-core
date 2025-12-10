#ifndef FUSION_ALTITUDE_MATH_H
#define FUSION_ALTITUDE_MATH_H

#include "config/types.h"

namespace fusion {

// Calculate pitch from accelerometer data
double get_angles_from_accel(types::ScaledAccelSample&, types::AccelTiltAngles&);

}

#endif
