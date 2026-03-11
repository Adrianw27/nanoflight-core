#ifndef HAL_I2C_H
#define HAL_I2C_H

#include "config/types.h"
#include <cstdint>

namespace HAL {
    namespace I2C {

        void init();

        void writeByte(uint8_t address, uint8_t reg, uint8_t data);
        bool readBytes(uint8_t address, uint8_t reg, uint8_t *buffer, uint8_t length);

    }
}

#endif
