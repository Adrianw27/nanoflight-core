#ifndef APP_MODE_H
#define APP_MODE_H

#include "config/types.h"

namespace app {

using Mode = types::Mode;

void set_mode(Mode mode);
Mode get_mode();

}

#endif
