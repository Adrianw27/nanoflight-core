#ifndef HAL_TIMING_H
#define HAL_TIMING_H

#include <stdint.h>

namespace hal {

uint32_t nowMicros();
float computeDeltaSeconds(uint32_t& lastTimestampMicros);

}

#endif 
