#ifndef HAL_TIMING_H
#define HAL_TIMING_H

#include <cstdint>

namespace hal {

// Get the current timestamp in microseconds
uint32_t get_current_micros();

// Get delta time since last timestamp recorded
double compute_delta_seconds(uint32_t& last_micros);

}

#endif 
