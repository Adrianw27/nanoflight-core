#ifndef APP_TASKS_H
#define APP_TASKS_H

namespace app {

// task to read IMU and store latest scaled sample
void task_imu_update();

// task to update attitude using gyro and/or accel
void task_estimation_update();

// task to emit telemetry
void task_publish_telemetry();

// task to publish health
void task_publish_health();

// task to refresh any LEDs / UI elements
void task_ui_update();

}  // namespace app

#endif
