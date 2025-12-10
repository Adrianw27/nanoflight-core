#include "fusion/attitude_math.h"
#include <cmath>

namespace {
    constexpr RAD_TO_DEG = 180.0 / 3.14159265358979323846;
}

namespace fusion {

double get_pitch_from_accel(const types::ScaledAccelSample& accel) {
    return RAD_TO_DEG * std::atan2(-accel.ax_g, std::sqrt(accel.ay_g * accel.ay_g + accel.az_g * accel.az_g));
}

double get_roll_from_accel(const types::ScaledAccelSample& accel) {
    return RAD_TO_DEG * std::atan2(accel.ay_g, accel.az_g);
}

void update_state_by_gyro(const types::ScaledGyroSample& gyro, types::AttitudeState& state, double dt_seconds) {
    state.roll_deg  += gyro.gx_dps * dt_seconds;
    state.pitch_deg += gyro.gy_dps * dt_seconds;
}

}  
