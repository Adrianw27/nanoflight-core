#ifndef CONFIG_TYPES_H
#define CONFIG_TYPES_H

#include <cstdint>
using std::int16_t;

namespace types {

// Holds raw accelerometer output for each data point
struct RawAccelSample {
	int16_t ax;
	int16_t ay;
	int16_t az;
};

// Holds raw gyro output for each data point
struct RawGyroSample {
	int16_t gx;
	int16_t gy;
	int16_t gz;
};

// Holds unit scaled accelerometer output for each data point
struct ScaledAccelSample {
	float ax_g;
	float ay_g;
	float az_g;
};

// Holds unit scaled gyro output for each data point
struct ScaledGyroSample {
	float gx_dps;
	float gy_dps;
	float gz_dps;
};

// Holds pitch, roll angles
struct AttitudeState {
	double pitch_deg;
	double roll_deg;
};

// Holds health data
struct HealthTelemetry {
    uint32_t uptime_ms;
    uint8_t mode;
    uint8_t status_flags; // bitfield for errors/warnings
};

// Holds task table with funcs and timings for scheduler
struct ScheduledTask {
	void (*task)();
	std::uint32_t period_micros;
	std::uint32_t last_run_micros;
	bool enabled;
};

}

#endif
