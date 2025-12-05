#ifndef CONFIG_PROJECT_CONFIG_H
#define CONFIG_PROJECT_CONFIG_H

#include <stdint.h>

namespace config {
	// I 2 c addresses
	constexpr uint8_t kImuI2cAddr = 0x68;
	constexpr uint8_t kPwrMgmt1Addr = 0x6B;
	constexpr uint8_t kSensorOutputAddr = 0x3B;

	// raw data / LSB per unit =  unit
	constexpr float kAccelLsbPerG = 16384.0f;
	constexpr float kTempLsbPerC = 340.0f;
	constexpr float kTempOffset = 36.53f;
	constexpr float kGyroLsbPerDps = 131.0f;
}

#endif
