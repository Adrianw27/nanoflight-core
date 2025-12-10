#include "comms/update_health.h"
#include "config/types.h"
#include "hal/timing.h"
#include <cstdint>

namespace comms {

// publish health data
void publish_health(const config::HealthState& health) {
	health.uptime_ms = hal::get_uptime();
	health.mode = static_cast<std::uint8_t>(app::get_mode());
	health.status_flags = app::get_status_flags();
}

}

#endif
