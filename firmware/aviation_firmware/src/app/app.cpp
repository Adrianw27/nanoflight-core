#include <Arduino.h>
#include "app/app.h"
#include "app/scheduler.h"
#include "hal/imu_driver.h"

namespace app {

void app_init() {
	Serial.begin(115200);
	hal::imu_init();
	scheduler_init();
}

void app_loop_tick() {
	scheduler_tick();
}

}
