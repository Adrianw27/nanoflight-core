#ifndef HAL_IMU_DRIVER_H
#define HAL_IMU_DRIVER_H

#include "config/types.h"
#include <cstdint>

namespace hal {

// Initialize i2c connection to imu
bool imu_init();

// Read raw sensor data through i2c (fill reference struct with data)
bool imu_read_raw(types::RawAccelSample&, types::RawGyroSample&, std::int16_t&);

// Calculate unit scaled data values (fill reference struct with scaled data)
bool imu_read_scaled(types::ScaledAccelSample&, types::ScaledGyroSample&, float&);


}

#endif
