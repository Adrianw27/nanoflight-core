#ifndef HAL_IMU_DRIVER_H
#define HAL_IMU_DRIVER_H

#include "config/types.h"

namespace hal {

// Initialize i2c connection to imu
bool imu_init();

// Read raw sensor data through i2c (fill reference struct with data)
bool imu_read_raw(types::ImuRawSample& out);

// Calculate unit scaled data values (fill reference struct with scaled data)
bool imu_read_scaled(types::ImuScaledSample& out);

}


#endif
