#ifndef TYPES_TELEMETRY_TYPES_H
#define TYPES_TELEMETRY_TYPES_H

#include <cstdint>

namespace Types {
    namespace Telemetry {

        // Holds health data
        struct HealthTelemetry {
            std::uint32_t uptime_ms;
            std::uint8_t mode;
            std::uint8_t status_flags; // bitfield for errors/warnings
        };

    }
}

#endif
