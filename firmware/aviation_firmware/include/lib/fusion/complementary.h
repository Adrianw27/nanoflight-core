#ifndef LIB_FUSION_COMPLEMENTARY_H
#define LIB_FUSION_COMPLEMENTARY_H

#include "../../types/imu_types.h"

namespace Lib {
    namespace Math {

        bool update_state_by_gyro(const types::ScaledGyroSample *gyro, types::AttitudeState *state, const double *dt_seconds);

    }
}

#endif
