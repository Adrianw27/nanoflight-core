#ifndef DRIVERS_MPU6050_H
#define DRIVERS_MPU6050_H

#include "../types/imu_types.h"
#include <cstdint>

class MPU650 {
    public:
        MPU6050();

        // true on succesful connection
        bool init();

        // update scaled vals
        bool update();

        // getters for math
        float getAccelX() const { return _accelX; }
        float getAccelY() const { return _accelY; }
        float getAccelZ() const { return _accelZ; }

        float getGyroX() const { return _gyroX; }
        float getGyroY() const { return _gyroY; }
        float getGyroZ() const { return _gyroZ; }

    private:
        // raw data / LSB per unit =  unit
        static constexpr float ACCEL_LSB_PER_G = 16384.0f;
        static constexpr float TEMP_LSB_PER_DEG = 340.0f;
        static constexpr float TEMP_OFFSET = 36.53f;
        static constexpr float GYRO_LSB_PER_DPS = 131.0f;

        RawAccelSample rawAccelSample;
        RawGyroSample rawGyroSample;
        float rawTempSample;
        scaledAccelSample scaledAccelSample;
        scaledGyroSample scaledGyroSample;
        float scaledTempSample;

        // Read raw sensor data through i2c (fill reference struct with data)
        bool readRawData();

        // Calculate unit scaled data values (fill reference struct with scaled data)
        bool scaleRawData();
};

// global
extern MPU6050 imu;

#endif
