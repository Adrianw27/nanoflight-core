#ifndef CONFIG_TYPES_H
#define CONFIG_TYPES_H

#include <cstdint>

namespace types {

// Holds raw sensor output for each data point
struct ImuRawSample {
    int16_t ax;
    int16_t ay;
    int16_t az;
    int16_t t;
    int16_t gx;
    int16_t gy;
    int16_t gz;
};

// Holds unit scaled sensor output for each data point
struct ImuScaledSample {
    float ax_g;
    float ay_g;
    float az_g;
    float t_c;
    float gx_dps;
    float gy_dps;
    float gz_dps;
};

}

#endif
