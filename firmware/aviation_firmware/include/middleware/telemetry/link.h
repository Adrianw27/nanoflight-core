#ifndef COMMS_LINK_H
#define COMMS_LINK_H

#include "config/types.h"

namespace comms {

// initialize serial
void serial_init();

// write accel sample to serial
bool serial_write_accel(const types::ScaledAccelSample&);

// write gyro sample to serial
bool serial_write_gyro(const types::ScaledGyroSample&);

// write temp sample to serial
bool serial_write_temp(const float& temp_cel);

// write attitude data to serial
bool serial_write_attitude(const types::AttitudeState&);

// write health data to serial
bool serial_write_health(const types::HealthTelemetry&);

}

#endif
