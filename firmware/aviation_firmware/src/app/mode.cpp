#include "app/mode.hpp"

namespace app {

static Mode g_mode = Mode::Boot;

void set_mode(Mode mode)
{
    g_mode = mode;
}

Mode get_mode()
{
    return g_mode;
}

} 

