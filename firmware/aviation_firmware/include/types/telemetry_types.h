#ifndef TYPES_TELEMETRY_TYPES_H
#define TYPES_TELEMETRY_TYPES_H

#include <cstdint>
using std::int16_t;

namespace Types {
    namespace Telemetry {

        // Holds health data
        struct HealthTelemetry {
            uint32_t uptime_ms;
            uint8_t mode;
            uint8_t status_flags; // bitfield for errors/warnings
        };

    }
}

#endif
