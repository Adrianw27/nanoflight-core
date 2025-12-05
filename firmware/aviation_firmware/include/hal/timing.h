#ifndef TIMING
#define TIMING

#include <stdint.h>

namespace hal {

uint32_t nowMicros();
float computeDeltaSeconds(uint32_t& lastTimestampMicros);

}

#endif 
