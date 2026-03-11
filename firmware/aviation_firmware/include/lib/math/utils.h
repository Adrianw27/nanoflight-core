#ifndef LIB_MATH_UTILS_H
#define LIB_MATH_UTILS_H

#include "../../types/imu_types.h"

namespace Lib {
    namespace Math {

        bool getAnglesFromAccel(const types::ScaledAccelSample *accel);

    }
}

#endif
