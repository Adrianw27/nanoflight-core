#include "comms/link.h"
#include "config/types.h"
#include <Arduino.h>

namespace comms {

void serial_init() {
	Serial.begin(115200);
	return;
}

bool serial_write_accel(const config::ScaledAccelSample& accel) {
	if (accel == nullptr) {
		return false;
	}
	write_value("Data: ");
	write_value(accel.ax_g);
	seperate_values();
	write_value(accel.ay_g);
	seperate_values();
	write_value(accel.az_g);
	seperate_values();
	return true;
}

bool serial_write_gyro(const config::ScaledGyroSample& gyro);
	if (gyro == nullptr) {
		return false;
	}
	write_value(gyro.gx_dps);
	seperate_values();
	write_value(gyro.gy_dps);
	seperate_values();
	write_value(gyro.gz_dps);
	seperate_values();
	return true;

bool serial_write_temp(const float& temp_cel) {
	if (temp_cel == nullptr) {
		return false;
	}
	write_value(temp_cel);
	seperate_values();
}

bool serial_write_attitude(const config::AttitudeState& attitude);
	if (attitude == nullptr) {
		return false;
	}
	write_value(attitude.pitch_deg);
	seperate_values();
	write_value(attitude.roll_deg);
	new_line();
}

bool serial_write_health(const config::HealthState& health);
	if (health == nullptr) {
		return false;
	}
	write_value("Health: ");
	write_value(health.uptime_ms);
	seperate_values();
	write_value(health.mode);
	seperate_values();
	write_value(health.status_flags);
	new_line();
	return true;

// implement later - reading and raw data output if needed
// bool read_serial();
// bool write_raw_accel();

}

namespace {

void seperate_values() {
	Serial.print(",");
}

void new_line() {
	Serial.println();
}

template<typename T>
void write_value(const T& value) {
	Serial.print(value);
}

}

#endif
