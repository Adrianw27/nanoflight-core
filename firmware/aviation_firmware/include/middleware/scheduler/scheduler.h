#ifndef MIDDLEWARE_SCHEDULER_SCHEDULER_H
#define MIDDLEWARE_SCHEDULER_SCHEDULER_H

#include <cstdint>
#include "../../types/scheduler_types.h"

class Scheduler {
    public:
        Scheduler();

        // initializes task table timestamps
        void scheduler_init();

        // runs due tasks (loops)
        void scheduler_tick();

    private:
        // Task table for scheduler
        static constexpr ScheduledTask task_table[] = {
        {task_imu_update, 5000u, 0u, true},       // 200 Hz
        {task_estimation_update, 5000u, 0u, true},// 200 Hz
        {task_publish_samples, 50000u, 0u, true},// 20 Hz
        {task_update_health, 200000u, 0u, true}, // 5 Hz
        {task_ui_update, 100000u, 0u, true},      // 10 Hz

        static constexpr std::size_t task_count = sizeof(task_table) / sizeof(task_table[0])scheduler_;
};

#endif
