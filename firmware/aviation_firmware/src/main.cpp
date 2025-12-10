#include <Arduino.h>
#include "config/project_config.h"
#include "config/types.h"
#include "hal/imu_driver.h"
#include "hal/timing.h"
#include "fusion/attitude_math.h"
#include <cstdint>

using std::uint32_t;
using types::ScaledAccelSample;
using types::ScaledGyroSample;
using types::AttitudeState;

void setup() {
	Serial.begin(115200);
	hal::imu_init();
}

void loop() {
	static uint32_t last_micros = hal::get_current_micros();
	static uint32_t last_print_micros = last_micros;

	ScaledAccelSample accel;
	ScaledGyroSample gyro;
	float temp_cel;
	AttitudeState state;

	uint32_t current_micros = hal::get_current_micros();
	uint32_t dt_micros = current_micros - last_micros;
	double dt_sec = dt_micros / 1000000.0;
	last_micros = current_micros;

	if (hal::imu_read_scaled(accel, gyro, temp_cel)) {

		state.pitch_deg = fusion::get_pitch_from_accel(accel);
		state.roll_deg = fusion::get_roll_from_accel(accel);
		fusion::update_state_by_gyro(gyro, state, dt_sec);

		if (current_micros - last_print_micros > 20000) { // e.g., print at 50 Hz
			last_print_micros = current_micros;
			// Print all data in csv for ground station

			// Accelerometer data (x,y,z) - units g
			Serial.print(accel.ax_g);
			Serial.print(',');
			Serial.print(accel.ay_g);
			Serial.print(',');
			Serial.print(accel.az_g);
			
			// Gyroscope data (x,y,z) - units deg/s
			Serial.print(gyro.gx_dps);
			Serial.print(',');
			Serial.print(gyro.gy_dps);
			Serial.print(',');
			Serial.print(gyro.gz_dps);
			Serial.print(',');;
			
			Serial.print(temp_cel); // Temperature - units deg celcius
			Serial.print(',');

			Serial.print(state.pitch_deg); // Pitch - units deg
			Serial.print(',');

			Serial.print(state.roll_deg); // Roll - units deg
		}
	}
	else {
		Serial.println("Error reading sensor data");
	}
}

