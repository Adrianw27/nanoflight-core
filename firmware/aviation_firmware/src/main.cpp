#include <Arduino.h>
#include "app/app.h"

void setup() {
    app::app_init();
}

void loop() {
    app::app_loop_tick();
}
