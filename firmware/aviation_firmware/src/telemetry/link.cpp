#include <Arduino.h>
#include "comms/link.h"

namespace {

void separate_values() {
	Serial.print(",");
}

void new_line() {
	Serial.println();
}

template <typename T>
void write_value(const T& value) {
	Serial.print(value);
}

}

namespace comms {

void serial_init() {
	Serial.begin(115200);
}

bool serial_write_accel(const types::ScaledAccelSample& accel) {
	write_value("Data: ");
	write_value(accel.ax_g);
	separate_values();
	write_value(accel.ay_g);
	separate_values();
	write_value(accel.az_g);
	separate_values();
	return true;
}

bool serial_write_gyro(const types::ScaledGyroSample& gyro) {
	write_value(gyro.gx_dps);
	separate_values();
	write_value(gyro.gy_dps);
	separate_values();
	write_value(gyro.gz_dps);
	separate_values();
	return true;
}

bool serial_write_temp(const float& temp_cel) {
	write_value(temp_cel);
	separate_values();
	return true;
}

bool serial_write_attitude(const types::AttitudeState& attitude) {
	write_value(attitude.pitch_deg);
	separate_values();
	write_value(attitude.roll_deg);
	new_line();
	return true;
}

bool serial_write_health(const types::HealthTelemetry& health) {
	write_value("Health: ");
	write_value(health.uptime_ms);
	separate_values();
	write_value(health.mode);
	separate_values();
	write_value(health.status_flags);
	new_line();
	return true;
}

}
