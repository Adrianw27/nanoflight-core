#ifndef HAL_IMU_DRIVER_H
#define HAL_IMU_DRIVER_H

#include "config/types.h"
#include <cstdint>

namespace hal {

bool imu_init();
bool imu_read_raw(types::RawAccelSample& accel, types::RawGyroSample& gyro, std::int16_t& temp);
bool imu_read_scaled(types::ScaledAccelSample& accel, types::ScaledGyroSample& gyro, float& temp_cel);

}

#endif
