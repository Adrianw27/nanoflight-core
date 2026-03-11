#ifndef TYPES_STATUS_FLAG_TYPES_H
#define TYPES_STATUS_FLAG_TYPES_H

#include <cstdint>

namespace Types {
    namespace StatusFlags {

        enum StatusFlag : std::uint8_t {
            STATUS_IMU_OK = 0x01,
            STATUS_FUSION_OK = 0x02,
            STATUS_SERIAL_ERROR = 0x04,
            STATUS_GLOBAL_FAULT = 0x10,
        };

        void set_status_flag(StatusFlag flag);
        void clear_status_flag(StatusFlag flag);
        std::uint8_t get_status_flags();

    }
}

#endif
