#ifndef APP_APP_H
#define APP_APP_H

namespace app {

// initializes HAL and scheduler
void app_init();

// ticks scheduler
void app_loop_tick();

}  // namespace app

#endif
