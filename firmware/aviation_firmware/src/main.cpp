#include <Arduino.h>
#include "config/project_config.h"
#include "config/types.h"
#include "hal/imu_driver.h"
#include "hal/timing.h"
#include "fusion/altitude_math.h"
#include <cstdint>

using std::uint32_t;
using types::RawAccelSample;
using types::RawGyroSample;
using types::ScaledAccelSample;
using types::ScaledGyroSample;
using types::AccelTiltAngles

void setup() {
	Serial.begin(115200);
	hal::imu_init();
}

void loop() {
	static uint32_t last_micros = hal::get_current_micros();

	ScaledAccelSample accel;
	ScaledGyroSample gyro;
	float temp;
	AccelTiltAngles angles;

	if (hal::imu_read_scaled(accel, gyro, temp)) {
		if (!fusion::get_angles_from_accel(accel, angles)){
			Serial.println("Error calculating roll and pitch");	
		}
	}
	else {
		Serial.println("Error reading sensor data");
		continue;
	}

	// print sensor data and pitch angles
	uint32_t current_micros = hal::get_current_micros();
	if (current_micros - last_micros > 20000) { // e.g., print at 50 Hz
		last_micros = current_micros;

		Serial.println("Accelerometer data (X, Y, Z): ");
		Serial.print(accel.ax_s);
		Serial.print(',');
		Serial.print(accel.ay_s);
		Serial.print(',');
		Serial.println(accel.az_s);
		
		Serial.println("Gyroscope data (X, Y, Z): ");
		Serial.print(gyro.gx_s);
		Serial.print(',');
		Serial.print(gyro.gy_s);
		Serial.print(',');
		Serial.println(gyro.gz_s);

		Serial.print("Roll: ");
		Serial.println(angles.roll);
		Serial.print("Pitch: ");
		Serial.println(angles.pitch);
		Serial.print("Temperature: ");
		Serial.println(temp);
	}

}
}

