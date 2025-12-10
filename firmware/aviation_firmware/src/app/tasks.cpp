#include <Arduino.h>
#include <cstdint>
#include "app/tasks.h"
#include "config/project_config.h"
#include "config/types.h"
#include "fusion/attitude_math.h"
#include "hal/imu_driver.h"
#include "hal/timing.h"
#include "comms/publish_telemetry.h"

using std::uint32_t;

namespace app {

static types::ScaledAccelSample accel{};
static types::ScaledGyroSample gyro{};
static types::AttitudeState attitude{};
static types::HealthState health{};
static float temp_cel = 0.0f;

static uint32_t last_micros = hal::get_current_micros();

void task_init_system() {
	comms::telemetry_init();
	bool imu_init = hal::imu_init();

	if (imu_init) {
		app::set_status_flag(app::STATUS_IMU_OK);
	}
	else {
		app::clear_status_flag(app::STATUS_IMU_OK);
		app::set_status_flag(app::GLOBAL_FAULT);
		app::set_mode(app::Mode::Standby);
	}
}

void task_imu_update() {
	bool imu_scaled = hal::imu_read_scaled(accel, gyro, temp_cel);

	if (imu_scaled) {
		app::clear_status_flag(app::STATUS_IMU_OK);
		app::set_status_flag(app::STATUS_IMU_OK);
	}
	else {
		app::set_status_flag(app::GLOBAL_FAULT);
		app::set_mode(app::Mode::Standby);
	}
}

void task_estimation_update() {
	bool set_angles = fusion::get_angles_from_accel(accel);
	double dt_sec = hal::compute_delta_seconds(last_micros);
	bool fusion = fusion::update_state_by_gyro(gyro, attitude, dt_sec);

	if (set_angles && fusion) {
		app::set_status_flag(app::STATUS_FUSION_OK);
	}
	else {
		app::set_status_flag(app::GLOBAL_FAULT);
		app::set_mode(app::Mode::Standby);
	}
	return;

}

void task_publish_samples() {
	bool write = comms::publish_samples(accel, gyro, temp_cel, attitude);
	
	if (write) {
		app::clear_status_flag(app::STATUS_SERIAL_ERROR);
	}
	else {
		app::set_status_flag(app::STATUS_SERIAL_ERROR);
	}
	return;
}

void task_update_health() {
	comms::update_health(health);
	bool write = comms::publish_health(health);

	if (write) {
		app::clear_status_flag(app::STATUS_SERIAL_ERROR);
	}
	else {
		app::set_status_flag(app::STATUS_SERIAL_ERROR);
	}
	return;
}

void task_ui_update() {
    // Placeholder for ground control 
}

}  
