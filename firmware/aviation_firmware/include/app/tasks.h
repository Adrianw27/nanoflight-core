#ifndefine APP_TASKS_H
#define APP_TASKS_H

namespace App {

// task to read IMU and store latest scaled sample
void task_imu_update();

// task to update attitude using gyro and/or accel
void task_attitude_estimation();

// task to emit telemetry
void task_telemetry_update();

// task to refresh any LEDs / UI elements
void task_ui_update();

}

#endif
