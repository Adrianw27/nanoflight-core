#ifndef MODULES_TASKS_IMU_TASK_H
#define MODULES_TASKS_IMU_TASK_H

namespace Modules {
    namespace Tasks {

        // task to read IMU and store latest scaled sample
        void task_imu_update();

    }
}

#endif
