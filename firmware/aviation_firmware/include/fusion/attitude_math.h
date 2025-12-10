#ifndef FUSION_ATTITUDE_MATH_H
#define FUSION_ATTITUDE_MATH_H

#include "config/types.h"

namespace fusion {

// Calculate pitch and roll from accelerometer data
bool get_angles_accel(const types::ScaledAccelSample&);

// Complimentary filter - integrate gyro into estimations
bool update_state_by_gyro(const types::ScaledGyroSample&, types::AttitudeState&, double dt_seconds);

}

#endif
