#include <Arduino.h>
#include <cstdint>
#include "app/tasks.h"
#include "config/project_config.h"
#include "config/types.h"
#include "fusion/attitude_math.h"
#include "hal/imu_driver.h"
#include "hal/timing.h"

using std::uint32_t;

namespace app {

static types::ScaledAccelSample accel{};
static types::ScaledGyroSample gyro{};
static types::AttitudeState state{};
static float temp_cel = 0.0f;
static uint32_t last_micros = hal::get_current_micros();

void task_imu_update() {
    hal::imu_read_scaled(accel, gyro, temp_cel);
}

void task_estimation_update() {
    state.pitch_deg = fusion::get_pitch_from_accel(accel);
    state.roll_deg = fusion::get_roll_from_accel(accel);

    double dt_sec = hal::compute_delta_seconds(last_micros);
    fusion::update_state_by_gyro(gyro, state, dt_sec);
}

void task_telemetry_update() {
    // Print all data in CSV for ground station
    Serial.print(accel.ax_g);
    Serial.print(',');
    Serial.print(accel.ay_g);
    Serial.print(',');
    Serial.print(accel.az_g);
    Serial.print(',');

    Serial.print(gyro.gx_dps);
    Serial.print(',');
    Serial.print(gyro.gy_dps);
    Serial.print(',');
    Serial.print(gyro.gz_dps);
    Serial.print(',');

    Serial.print(temp_cel);
    Serial.print(',');

    Serial.print(state.pitch_deg);
    Serial.print(',');

    Serial.println(state.roll_deg);
}

void task_ui_update() {
    // Placeholder for ground control 
}

}  
