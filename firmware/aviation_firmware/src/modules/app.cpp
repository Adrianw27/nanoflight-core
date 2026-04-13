#include <Arduino.h>
#include "app/app.h"
#include "app/mode.h"
#include "app/scheduler.h"
#include "app/tasks.h"

namespace app {

void app_init() {
	app::set_mode(app::Mode::Init);
	task_init_system();
	scheduler_init();
}

void app_loop_tick() {
	scheduler_tick();
}

}
