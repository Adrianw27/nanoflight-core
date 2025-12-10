#include "config/types.h"
#include "comms/link.h"
#include "comms/publish_telemetry.h"

namespace comms {

void telemetry_init() {
	comms::serial_init();
	return;
}

bool publish_samples(const config::ScaledAccelSample& accel, const config::ScaledGyroSample& gyro, const float& temp_cel, const config::AttitudeState& attitude) {
	bool success = serial_write_accel(accel);
	success = success && serial_write_gyro(gyro);
	success = success && serial_write_temp(temp_cel);
	success = success && serial_write_attitude(attitude);
	return success;
}

bool publish_health(const config::HealthState& health) {
	bool success = serial_write_health(health);
	return success;
}

// later implement read telemetry and also publish raw data

}

#endif
