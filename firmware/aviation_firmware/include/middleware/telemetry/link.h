#ifndef COMMS_LINK_H
#define COMMS_LINK_H

#include "config/types.h"

namespace comms {

// initialize serial
void serial_init();

// write frame schemas to serial
bool serial_write_schema();

// write sample telemetry frame to serial
bool serial_write_sample(const types::SampleTelemetry&);

// write health data to serial
bool serial_write_health(const types::HealthTelemetry&);

}

#endif
