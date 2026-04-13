#ifndef HAL_TIMING_H
#define HAL_TIMING_H

#include <cstdint>

namespace hal {

std::uint32_t get_current_micros();
std::uint32_t get_uptime();
double compute_delta_seconds(std::uint32_t& last_micros);

}

#endif
