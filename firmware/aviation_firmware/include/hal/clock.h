#ifndef HAL_CLOCK_H
#define HAL_CLOCK_H

#include <cstdint>

namespace HAL {
    namespace Clock {

        // Get microseconds since boot
        uint32_t getMicros();

        // get milliseconds since boot
        uint32_t getMillis();

        // Get delta time since last timestamp recorded
        double getDeltaSec (uint32_t& last_micros);

    }
}

#endif
