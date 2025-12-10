#ifndef COMMS_PUBLISH_TELEMETRY_H
#define COMMS_PUBLISH_TELEMETRY_H

#include "config/types.h"

namespace comms {

// initialize telemetry
bool telemetry_init();

// publish all data
bool publish_samples(const config::ScaledAccelSample&, const config::ScaledGyroSample&, const float& temp_cel, const config::AttitudeState&);

// publish health();
bool publish_health(const config::HealthState&);

}

#endif
