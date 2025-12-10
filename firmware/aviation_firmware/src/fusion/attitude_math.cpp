#include "fusion/attitude_math.h"
#include <cmath>

#define PI 3.14159265358979323846

namespace fusion {

double get_pitch_from_accel(const types::ScaledAccelSample& accel){
	double pitch = 180 * atan2((- accel.ax_s) , (accel.ay_s * accel.ay_s + accel.az_s * accel.az_s)) / PI;
	return pitch;
}

double get_roll_from_accel(const types::ScaledAccelSample& accel){
	double roll = 180 * atan2(accel.ay_s , accel.az_s) / PI;
	return roll;
}

void update_state_by_gyro(const types::ScaledGyroSample& gyro, types::AttitudeState& state, double dt);
	double dt_x = gyro.gx_dps * dt;
	double dt_y = gyro.gy_dps * dt;
	state.roll += dt_x;
	state.pitch += dt_y;
	return;
}

}
