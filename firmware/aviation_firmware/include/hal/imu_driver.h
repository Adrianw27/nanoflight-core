#ifndef IMU_DRIVER
#define IMU_DRIVER

#include "config/types.h"

namespace hal {

bool imuInit();
bool imuReadRaw(types::ImuRawSample& out);
bool imuReadScaled(types::ImuScaledSample& out);

}


#endif
