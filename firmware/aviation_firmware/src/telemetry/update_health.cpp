#include "comms/update_health.h"
#include "app/mode.h"
#include "app/status_flags.h"
#include "hal/timing.h"

namespace comms {

void update_health(types::HealthTelemetry& health) {
	health.uptime_ms = hal::get_uptime();
	health.mode = static_cast<std::uint8_t>(app::get_mode());
	health.status_flags = app::get_status_flags();
}

}
