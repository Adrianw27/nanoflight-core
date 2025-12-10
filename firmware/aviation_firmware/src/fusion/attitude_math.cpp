#include "fusion/attitude_math.h"
#include <cmath>

namespace {
    constexpr RAD_TO_DEG = 180.0 / 3.14159265358979323846;
}

namespace fusion {

bool get_angles_from_accel(const types::ScaledAccelSample& accel) {
	if (accel == nullptr) {
		return false;
	}

	double pitch = RAD_TO_DEG * std::atan2(-accel.ax_g, std::sqrt(accel.ay_g * accel.ay_g + accel.az_g * accel.az_g));
	double roll = RAD_TO_DEG * std::atan2(accel.ay_g, accel.az_g);

	return true;
}

bool update_state_by_gyro(const types::ScaledGyroSample& gyro, types::AttitudeState& state, const double& dt_seconds) {
	if (gyro == nullptr || state = nullptr || dt_seconds == nullptr) {
		return false;
	}
	state.roll_deg  += gyro.gx_dps * dt_seconds;
	state.pitch_deg += gyro.gy_dps * dt_seconds;

	return true;
}

}  
