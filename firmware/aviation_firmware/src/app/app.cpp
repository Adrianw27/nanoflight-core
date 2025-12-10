#include <Arduino.h>
#include "app/mode.h"

namespace app {

void app_init() {
	app::set_mode(app::Mode::Init);
	scheduler_init();
	return;
}

void app_loop_tick() {
	app::set_mode(app::Mode::Active);
	scheduler_tick();
	return;
}

}
