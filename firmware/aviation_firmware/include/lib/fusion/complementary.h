#ifndef LIB_FUSION_COMPLEMENTARY_H
#define LIB_FUSION_COMPLEMENTARY_H

#include "config/types.h"

namespace fusion {

void reset_complementary_filter(types::ComplementaryFilterState&, double alpha);
bool update_state_by_complementary(const types::ScaledAccelSample&, const types::ScaledGyroSample&, types::ComplementaryFilterState&, types::AttitudeState&, double dt_seconds);

}

#endif
