#ifndef APP_TASKS_H
#define APP_TASKS_H

namespace app {

void task_init_system();
void task_imu_update();
void task_estimation_update();
void task_publish_samples();
void task_update_health();
void task_ui_update();

}

#endif
