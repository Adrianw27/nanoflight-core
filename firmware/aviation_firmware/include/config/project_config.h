#ifndef PROJECT_CONFIG
#define PROJECT_CONFIG

#include <stdint.h>

namespace config {
	constexpr uint8_t kImuI2cAddr = 0x68;

	// raw data / LSB per unit =  unit
	constexpr float kAccelLsbPerG = 16384.0f;
	constexpr float kGyroLsbPerDps = 131.0f;
}

#endif
