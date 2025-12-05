#include <Arduino.h>
#include "config/project_config.h"
#include "config/types.h"
#include "hal/imu_driver.h"
#include "hal/timing.h"

using types::ImuScaledSample;

void setup() {
    Serial.begin(115200);
    hal::imu_init();
}

void loop() {
    static uint32_t last_micros = hal::get_current_micros();

    ImuScaledSample sample;
    if (hal::imu_read_scaled(sample)) {

        float pitch_deg = pitch_from_accel(
            sample.ax_g,
            sample.ay_g,
            sample.az_g
        );

        // print rate limit
	uint32_t current_micros = hal::get_current_micros();
        if (current_micros - last_micros > 20000) { // e.g., print at 50 Hz
            last_micros = current_micros;

            Serial.print(sample.ax_g);
            Serial.print(',');
            Serial.print(sample.ay_g);
            Serial.print(',');
            Serial.print(sample.az_g);
            Serial.print(',');
            Serial.println(pitchDeg);
        }
    }
}

