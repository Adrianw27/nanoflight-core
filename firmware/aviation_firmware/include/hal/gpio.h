#ifndef HAL_GPIO_H
#define HAL_GPIO_H

#include <stdint.h>

namespace HAL {
    namespace GPIO {
        // enum to abstract HIGH/LOW
        enum State { LOW_STATE = 0, HIGH_STATE = 1 };
        enum Mode { INPUT_MODE = 0, OUTPUT_MODE = 1 };

        void setMode(uint8_t pin, Mode mode);
        void write(uint8_t pin, State state);
        State read(uint8_t pin);

        // Helper specifically for toggling
        void toggle(uint8_t pin);
    }
}

#endif
