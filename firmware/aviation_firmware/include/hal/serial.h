#ifndef HAL_SERIAL_H
#define HAL_SERIAL_H

#include <cstdint>
#include <cstddef> // for size_t

namespace HAL {
    namespace Serial {
        void init(uint32_t baud_rate);
        void print(const char* str);
        void println(const char* str);

        // Useful for sending raw telemetry packets
        void write(const uint8_t* data, size_t len);
    }
}

#endif
