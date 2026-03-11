#include "fusion/attitude_math.h"
#include <cmath>

namespace {
constexpr double RAD_TO_DEG = 180.0 / 3.14159265358979323846;

bool get_accel_angles(const types::ScaledAccelSample& accel, double& pitch_deg, double& roll_deg) {
	double lateral_magnitude = std::sqrt(
		static_cast<double>(accel.ay_g) * static_cast<double>(accel.ay_g) +
		static_cast<double>(accel.az_g) * static_cast<double>(accel.az_g)
	);
	double gravity_magnitude = std::sqrt(
		static_cast<double>(accel.ax_g) * static_cast<double>(accel.ax_g) +
		static_cast<double>(accel.ay_g) * static_cast<double>(accel.ay_g) +
		static_cast<double>(accel.az_g) * static_cast<double>(accel.az_g)
	);

	if (gravity_magnitude <= 0.0) {
		return false;
	}

	pitch_deg = RAD_TO_DEG * std::atan2(-static_cast<double>(accel.ax_g), lateral_magnitude);
	roll_deg = RAD_TO_DEG * std::atan2(static_cast<double>(accel.ay_g), static_cast<double>(accel.az_g));
	return true;
}

void reset_complementary_axis(types::ComplementaryAxisState& axis) {
	axis.angle_deg = 0.0;
	axis.initialized = false;
}

void reset_kalman_axis(types::KalmanAxisState& axis) {
	axis.angle_deg = 0.0;
	axis.bias_dps = 0.0;
	axis.rate_dps = 0.0;
	axis.error_covariance_00 = 0.0;
	axis.error_covariance_01 = 0.0;
	axis.error_covariance_10 = 0.0;
	axis.error_covariance_11 = 0.0;
	axis.initialized = false;
}

double update_complementary_axis(double accel_angle_deg, double gyro_rate_dps, types::ComplementaryAxisState& axis, double alpha, double dt_seconds) {
	if (!axis.initialized) {
		axis.angle_deg = accel_angle_deg;
		axis.initialized = true;
		return axis.angle_deg;
	}

	double predicted_angle = axis.angle_deg + (gyro_rate_dps * dt_seconds);
	axis.angle_deg = (alpha * predicted_angle) + ((1.0 - alpha) * accel_angle_deg);
	return axis.angle_deg;
}

double update_kalman_axis(double measured_angle_deg, double measured_rate_dps, types::KalmanAxisState& axis, const types::KalmanFilterState& filter, double dt_seconds) {
	if (!axis.initialized) {
		axis.angle_deg = measured_angle_deg;
		axis.initialized = true;
	}

	axis.rate_dps = measured_rate_dps - axis.bias_dps;
	axis.angle_deg += dt_seconds * axis.rate_dps;

	axis.error_covariance_00 += dt_seconds * (dt_seconds * axis.error_covariance_11 - axis.error_covariance_01 - axis.error_covariance_10 + filter.process_noise_angle);
	axis.error_covariance_01 -= dt_seconds * axis.error_covariance_11;
	axis.error_covariance_10 -= dt_seconds * axis.error_covariance_11;
	axis.error_covariance_11 += filter.process_noise_bias * dt_seconds;

	double innovation = measured_angle_deg - axis.angle_deg;
	double innovation_covariance = axis.error_covariance_00 + filter.measurement_noise;
	if (innovation_covariance <= 0.0) {
		return axis.angle_deg;
	}

	double kalman_gain_0 = axis.error_covariance_00 / innovation_covariance;
	double kalman_gain_1 = axis.error_covariance_10 / innovation_covariance;

	axis.angle_deg += kalman_gain_0 * innovation;
	axis.bias_dps += kalman_gain_1 * innovation;

	double covariance_00 = axis.error_covariance_00;
	double covariance_01 = axis.error_covariance_01;

	axis.error_covariance_00 -= kalman_gain_0 * covariance_00;
	axis.error_covariance_01 -= kalman_gain_0 * covariance_01;
	axis.error_covariance_10 -= kalman_gain_1 * covariance_00;
	axis.error_covariance_11 -= kalman_gain_1 * covariance_01;

	return axis.angle_deg;
}
}

namespace fusion {

bool get_angles_from_accel(const types::ScaledAccelSample& accel, types::AttitudeState& state) {
	double pitch = 0.0;
	double roll = 0.0;
	bool valid = get_accel_angles(accel, pitch, roll);
	if (!valid) {
		return false;
	}

	state.pitch_deg = pitch;
	state.roll_deg = roll;
	return true;
}

void reset_complementary_filter(types::ComplementaryFilterState& filter, double alpha) {
	filter.alpha = alpha;
	reset_complementary_axis(filter.roll);
	reset_complementary_axis(filter.pitch);
}

void reset_kalman_filter(types::KalmanFilterState& filter, double process_noise_angle, double process_noise_bias, double measurement_noise) {
	filter.process_noise_angle = process_noise_angle;
	filter.process_noise_bias = process_noise_bias;
	filter.measurement_noise = measurement_noise;
	reset_kalman_axis(filter.roll);
	reset_kalman_axis(filter.pitch);
}

bool update_state_by_complementary(const types::ScaledAccelSample& accel, const types::ScaledGyroSample& gyro, types::ComplementaryFilterState& filter, types::AttitudeState& state, double dt_seconds) {
	if (dt_seconds < 0.0 || filter.alpha < 0.0 || filter.alpha > 1.0) {
		return false;
	}

	double pitch_deg = 0.0;
	double roll_deg = 0.0;
	bool valid = get_accel_angles(accel, pitch_deg, roll_deg);
	if (!valid) {
		return false;
	}

	state.roll_deg = update_complementary_axis(roll_deg, static_cast<double>(gyro.gx_dps), filter.roll, filter.alpha, dt_seconds);
	state.pitch_deg = update_complementary_axis(pitch_deg, static_cast<double>(gyro.gy_dps), filter.pitch, filter.alpha, dt_seconds);
	return true;
}

bool update_state_by_kalman(const types::ScaledAccelSample& accel, const types::ScaledGyroSample& gyro, types::KalmanFilterState& filter, types::AttitudeState& state, double dt_seconds) {
	if (dt_seconds < 0.0 || filter.measurement_noise <= 0.0 || filter.process_noise_angle < 0.0 || filter.process_noise_bias < 0.0) {
		return false;
	}

	double pitch_deg = 0.0;
	double roll_deg = 0.0;
	bool valid = get_accel_angles(accel, pitch_deg, roll_deg);
	if (!valid) {
		return false;
	}

	state.roll_deg = update_kalman_axis(roll_deg, static_cast<double>(gyro.gx_dps), filter.roll, filter, dt_seconds);
	state.pitch_deg = update_kalman_axis(pitch_deg, static_cast<double>(gyro.gy_dps), filter.pitch, filter, dt_seconds);
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
