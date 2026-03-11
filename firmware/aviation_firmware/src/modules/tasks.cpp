#include <Arduino.h>
#include <cstdint>
#include "app/mode.h"
#include "app/status_flags.h"
#include "app/tasks.h"
#include "comms/publish_telemetry.h"
#include "comms/update_health.h"
#include "config/types.h"
#include "fusion/attitude_math.h"
#include "hal/imu_driver.h"
#include "hal/timing.h"

using std::uint32_t;

namespace app {

static types::ScaledAccelSample accel{};
static types::ScaledGyroSample gyro{};
static types::AttitudeState attitude{};
static types::AttitudeState accel_attitude{};
static types::AttitudeState complementary_attitude{};
static types::SampleTelemetry sample_telemetry{};
static types::HealthTelemetry health{};
static float temp_cel = 0.0f;
static types::ComplementaryFilterState complementary_filter{};
static types::KalmanFilterState kalman_filter{};

static uint32_t last_micros = 0;

void task_init_system() {
	comms::telemetry_init();
	bool imu_init_ok = hal::imu_init();
	last_micros = hal::get_current_micros();
	fusion::reset_complementary_filter(complementary_filter, 0.98);
	fusion::reset_kalman_filter(kalman_filter, 0.001, 0.003, 0.03);

	if (imu_init_ok) {
		app::set_status_flag(app::STATUS_IMU_OK);
	} else {
		app::clear_status_flag(app::STATUS_IMU_OK);
		app::set_status_flag(app::STATUS_GLOBAL_FAULT);
		app::set_mode(app::Mode::Standby);
	}
}

void task_imu_update() {
	bool imu_scaled = hal::imu_read_scaled(accel, gyro, temp_cel);

	if (imu_scaled) {
		app::set_status_flag(app::STATUS_IMU_OK);
	} else {
		app::clear_status_flag(app::STATUS_IMU_OK);
		app::set_status_flag(app::STATUS_GLOBAL_FAULT);
		app::set_mode(app::Mode::Standby);
	}
}

void task_estimation_update() {
	bool set_angles = fusion::get_angles_from_accel(accel, accel_attitude);
	double dt_sec = hal::compute_delta_seconds(last_micros);
	bool complementary_ok = fusion::update_state_by_complementary(accel, gyro, complementary_filter, complementary_attitude, dt_sec);
	bool fused = fusion::update_state_by_kalman(accel, gyro, kalman_filter, attitude, dt_sec);

	if (set_angles && complementary_ok && fused) {
		app::set_status_flag(app::STATUS_FUSION_OK);
	} else {
		app::clear_status_flag(app::STATUS_FUSION_OK);
		app::set_status_flag(app::STATUS_GLOBAL_FAULT);
		app::set_mode(app::Mode::Standby);
	}
}

void task_publish_samples() {
	sample_telemetry.time_ms = hal::get_uptime();
	sample_telemetry.accel = accel;
	sample_telemetry.gyro = gyro;
	sample_telemetry.temp_cel = temp_cel;
	sample_telemetry.accel_attitude = accel_attitude;
	sample_telemetry.complementary_attitude = complementary_attitude;
	sample_telemetry.kalman_attitude = attitude;

	bool write_ok = comms::publish_samples(sample_telemetry);

	if (write_ok) {
		app::clear_status_flag(app::STATUS_SERIAL_ERROR);
	} else {
		app::set_status_flag(app::STATUS_SERIAL_ERROR);
	}
}

void task_update_health() {
	comms::update_health(health);
	bool write_ok = comms::publish_health(health);

	if (write_ok) {
		app::clear_status_flag(app::STATUS_SERIAL_ERROR);
	} else {
		app::set_status_flag(app::STATUS_SERIAL_ERROR);
	}
}

void task_ui_update() {
    // Placeholder for ground control
}

}
