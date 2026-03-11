#ifndef BOARD_CONFIG_H
#define BOARD_CONFIG_H

#include <cstdint>

// I2c register addresses
constexpr uint8_t MPU6050_I2C_ADDR = 0x68;
constexpr uint8_t PWR_MGMT_ADDR = 0x6B;
constexpr uint8_t ACCEL_OUTPUT_ADDR = 0x3B;

#endif
