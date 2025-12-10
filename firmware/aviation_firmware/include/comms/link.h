#ifndef COMMS_LINK_H
#define COMMS_LINK_H

#include "config/types.h"

namespace comms {

// initialize serial
void serial_init();

// write accel sample to serial
bool serial_write_accel(const config::ScaledAccelSample&);

// write gyro sample to serial
bool serial_write_gyro(const config::ScaledGyroSample&);

// write temp sample to serial
bool serial_write_temp(const float& temp_cel);

// write attitude data to serial
bool serial_write_attitude(const config::AttitudeState&);

// write health data to serial
bool serial_write_health(const config::HealthState&);

//Implement later - reading from serial
// bool read_serial();

}

// internal helpers
namespace {

// seperate vals with comma
void seperate_values();

// new line
void new_line();

template<typename T>
void write_value(const T& value);

}

#endif
