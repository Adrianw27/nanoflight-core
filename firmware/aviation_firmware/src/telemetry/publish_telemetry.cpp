#include "comms/link.h"
#include "comms/publish_telemetry.h"

namespace comms {

bool telemetry_init() {
	comms::serial_init();
	return comms::serial_write_schema();
}

bool publish_samples(const types::SampleTelemetry& sample) {
	return serial_write_sample(sample);
}

bool publish_health(const types::HealthTelemetry& health) {
	return serial_write_health(health);
}

}
