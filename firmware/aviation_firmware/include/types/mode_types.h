#ifndef TYPES_MODE_TYPES_H
#define TYPES_MODE_TYPES_H

#include <cstdint>

namespace Types {
    namespace Mode {

        // for now hust use boot and active, once pid implemented to aim missile we use more modes
        enum class Mode : std::uint8_t {
            Boot    = 0,
            Init    = 1,
            Standby = 2,
            Armed   = 3,
            Active  = 4,
            Fault   = 5,
        };

        void set_mode(Mode mode);

        Mode get_mode();

    }
}

#endif

