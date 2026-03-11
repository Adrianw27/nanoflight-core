#include "comms/link.h"
#include "comms/publish_telemetry.h"

namespace comms {

bool telemetry_init() {
	comms::serial_init();
	return true;
}

bool publish_samples(const types::ScaledAccelSample& accel,
                     const types::ScaledGyroSample& gyro,
                     const float& temp_cel,
                     const types::AttitudeState& attitude) {
	bool success = serial_write_accel(accel);
	success = success && serial_write_gyro(gyro);
	success = success && serial_write_temp(temp_cel);
	success = success && serial_write_attitude(attitude);
	return success;
}

bool publish_health(const types::HealthTelemetry& health) {
	return serial_write_health(health);
}

}
