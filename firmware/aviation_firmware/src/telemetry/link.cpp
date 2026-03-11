#include <Arduino.h>
#include "comms/link.h"
#include "middleware/telemetry/protocol.h"

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

void write_value(const float& value) {
	Serial.print(value, 6);
}

void write_value(const double& value) {
	Serial.print(value, 6);
}

void write_attitude_values(const types::AttitudeState& attitude) {
	write_value(attitude.pitch_deg);
	separate_values();
	write_value(attitude.roll_deg);
}

}

namespace comms {

void serial_init() {
	Serial.begin(115200);
}

bool serial_write_schema() {
	Serial.println(protocol::kDataSchema);
	Serial.println(protocol::kHealthSchema);
	return true;
}

bool serial_write_sample(const types::SampleTelemetry& sample) {
	write_value(protocol::kDataFrameType);
	separate_values();
	write_value(sample.time_ms);
	separate_values();
	write_value(sample.accel.ax_g);
	separate_values();
	write_value(sample.accel.ay_g);
	separate_values();
	write_value(sample.accel.az_g);
	separate_values();
	write_value(sample.gyro.gx_dps);
	separate_values();
	write_value(sample.gyro.gy_dps);
	separate_values();
	write_value(sample.gyro.gz_dps);
	separate_values();
	write_value(sample.temp_cel);
	separate_values();
	write_attitude_values(sample.accel_attitude);
	separate_values();
	write_attitude_values(sample.complementary_attitude);
	separate_values();
	write_attitude_values(sample.kalman_attitude);
	new_line();
	return true;
}

bool serial_write_health(const types::HealthTelemetry& health) {
	write_value(protocol::kHealthFrameType);
	separate_values();
	write_value(health.uptime_ms);
	separate_values();
	write_value(health.mode);
	separate_values();
	write_value(health.status_flags);
	new_line();
	return true;
}

}
