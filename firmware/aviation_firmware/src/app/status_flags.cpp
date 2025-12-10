#include "app/status_flags.hpp"
#include <cstdint>

namespace app {

static std::uint8_t g_status_flags = 0;

void set_status_flag(StatusFlag flag)
{
    g_status_flags |= static_cast<uint8_t>(flag);
}

void clear_status_flag(StatusFlag flag)
{
    g_status_flags &= ~static_cast<uint8_t>(flag);
}

uint8_t get_status_flags()
{
    return g_status_flags;
}

} 
