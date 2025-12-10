#ifndef COMMS_HEALTH_TELEMETRY_H
#define COMMS_HEALTH_TELEMETRY_H

#include "config/types.h"

namespace comms {

// publish health data
void publish_health(const config::HealthState&);

}

#endif
