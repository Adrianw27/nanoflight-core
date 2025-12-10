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
	float ax_s;
	float ay_s;
	float az_s;
};

// Holds unit scaled gyro output for each data point
struct ScaledGyroSample {
	float gx_s;
	float gy_s;
	float gz_s;
};

// Holds pitch, roll angles
struct AccelTiltAngles {
	float pitch;
	float roll;
};

}

#endif
