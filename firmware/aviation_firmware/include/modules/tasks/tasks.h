#ifndef APP_TASKS_H
#define APP_TASKS_H

namespace Modules {
    namespace Tasks {

// task to initialize system components
void task_init_system();

// task to read IMU and store latest scaled sample
void task_imu_update();

// task to update attitude using gyro and/or accel
void task_estimation_update();

// task to emit telemetry
void task_publish_samples();

// task to update and publish health
void task_update_health();

// task to refresh any LEDs / UI elements
void task_ui_update();

    }
}

#endif
