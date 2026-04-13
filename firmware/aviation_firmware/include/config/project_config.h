#ifndef CONFIG_PROJECT_CONFIG_H
#define CONFIG_PROJECT_CONFIG_H

#include <cstdint>

namespace config {

constexpr std::uint8_t imu_i2c_address = 0x68;
constexpr std::uint8_t pwr_mgmt_address = 0x6B;
constexpr std::uint8_t accel_output_address = 0x3B;

constexpr float accel_lsb_per_g = 16384.0f;
constexpr float gyro_lsb_per_dps = 131.0f;
constexpr float temp_lsb_per_deg = 340.0f;
constexpr float temp_offset = 36.53f;

}

#endif
