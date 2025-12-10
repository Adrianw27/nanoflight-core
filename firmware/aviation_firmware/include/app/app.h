#ifndefine APP_APP_H
#define APP_APP_H

namespace App {

// initializes HAL and scheduler
void app_init();

// ticks scheduler
void app_loop_tick();

}

#endif
