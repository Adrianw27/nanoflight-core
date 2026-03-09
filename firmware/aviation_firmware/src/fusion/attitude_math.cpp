#include "fusion/attitude_math.h"
#include <cmath>

namespace {
constexpr double RAD_TO_DEG = 180.0 / 3.14159265358979323846;
}

namespace fusion {

bool get_angles_from_accel(const types::ScaledAccelSample& accel, types::AttitudeState& state) {
	double pitch = RAD_TO_DEG * std::atan2(-accel.ax_g, std::sqrt(accel.ay_g * accel.ay_g + accel.az_g * accel.az_g));
	double roll = RAD_TO_DEG * std::atan2(accel.ay_g, accel.az_g);

	state.pitch_deg = pitch;
	state.roll_deg = roll;
	return true;
}

bool update_state_by_gyro(const types::ScaledGyroSample& gyro, types::AttitudeState& state, double dt_seconds) {
	if (dt_seconds < 0.0) {
		return false;
	}

	state.roll_deg += static_cast<double>(gyro.gx_dps) * dt_seconds;
	state.pitch_deg += static_cast<double>(gyro.gy_dps) * dt_seconds;

	return true;
}

}
