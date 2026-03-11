#ifndef FUSION_ATTITUDE_MATH_H
#define FUSION_ATTITUDE_MATH_H

#include "config/types.h"

namespace fusion {

// Calculate pitch and roll from accelerometer data
bool get_angles_from_accel(const types::ScaledAccelSample&, types::AttitudeState&);

// Initialize complementary filter state
void reset_complementary_filter(types::ComplementaryFilterState&, double alpha);

// Initialize Kalman filter state
void reset_kalman_filter(types::KalmanFilterState&, double process_noise_angle, double process_noise_bias, double measurement_noise);

// Complementary filter update
bool update_state_by_complementary(const types::ScaledAccelSample&, const types::ScaledGyroSample&, types::ComplementaryFilterState&, types::AttitudeState&, double dt_seconds);

// Kalman filter update
bool update_state_by_kalman(const types::ScaledAccelSample&, const types::ScaledGyroSample&, types::KalmanFilterState&, types::AttitudeState&, double dt_seconds);

// Gyro-only integration
bool update_state_by_gyro(const types::ScaledGyroSample&, types::AttitudeState&, double dt_seconds);

}

#endif
