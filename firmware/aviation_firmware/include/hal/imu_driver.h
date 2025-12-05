#ifndef HAL_IMU_DRIVER_H
#define HAL_IMU_DRIVER_H

#include "config/types.h"

namespace hal {

bool imuInit();
bool imuReadRaw(types::ImuRawSample& out);
bool imuReadScaled(types::ImuScaledSample& out);

}


#endif
