#ifndef TYPES_TELEMETRY_TYPES_H
#define TYPES_TELEMETRY_TYPES_H

#include "types/attitude_types.h"
#include "types/imu_types.h"
#include <cstdint>

namespace Types {
    namespace Telemetry {

        // Holds one telemetry sample frame for the ground station
        struct SampleTelemetry {
            std::uint32_t time_ms;
            Types::Imu::ScaledAccelSample accel;
            Types::Imu::ScaledGyroSample gyro;
            float temp_cel;
            Types::Attitude::AttitudeState accel_attitude;
            Types::Attitude::AttitudeState complementary_attitude;
            Types::Attitude::AttitudeState kalman_attitude;
        };

        // Holds health data
        struct HealthTelemetry {
            std::uint32_t uptime_ms;
            std::uint8_t mode;
            std::uint8_t status_flags; // bitfield for errors/warnings
        };

    }
}

#endif
