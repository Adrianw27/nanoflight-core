#include "app/status_flags.h"
#include <cstdint>

namespace app {

static std::uint8_t g_status_flags = 0;

void set_status_flag(StatusFlag flag) {
    g_status_flags |= static_cast<std::uint8_t>(flag);
}

void clear_status_flag(StatusFlag flag) {
    g_status_flags &= static_cast<std::uint8_t>(~static_cast<std::uint8_t>(flag));
}

std::uint8_t get_status_flags() {
    return g_status_flags;
}

}
