#ifndef LIB_FUSION_KALMAN_H
#define LIB_FUSION_KALMAN_H

#include "config/types.h"

namespace fusion {

void reset_kalman_filter(types::KalmanFilterState&, double process_noise_angle, double process_noise_bias, double measurement_noise);
bool update_state_by_kalman(const types::ScaledAccelSample&, const types::ScaledGyroSample&, types::KalmanFilterState&, types::AttitudeState&, double dt_seconds);

}

#endif
