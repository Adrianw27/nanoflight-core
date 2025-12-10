#include "app/status_flags.hpp"
#include <cstdint>

using std::uint8_t

namespace app {

static uint8_t g_status_flags = 0;

void set_status_flag(StatusFlag flag) {
    g_status_flags |= static_cast<uint8_t>(flag);
    return;
}

void clear_status_flag(StatusFlag flag) {
    g_status_flags &= ~static_cast<uint8_t>(flag);
    return;
}

uint8_t get_status_flags() {
    return g_status_flags;
}

} 
