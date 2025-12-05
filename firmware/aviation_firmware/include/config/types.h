#ifndef CONFIG_TYPES_H
#define CONFIG_TYPES_H

#include <stdint.h>

namespace types {

struct ImuRawSample {
    int16_t ax;
    int16_t ay;
    int16_t az;
    int16_t t;
    int16_t gx;
    int16_t gy;
    int16_t gz;
};

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
