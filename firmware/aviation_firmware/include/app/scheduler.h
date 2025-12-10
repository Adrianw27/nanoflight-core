#ifndef APP_SCHEDULER_H
#define APP_SCHEDULER_H

#include <cstdint>
#include "config/types.h"

namespace app {

// Task table for scheduler
static config::ScheduledTask task_table[] = {
	{task_imu_update, 5000u, 0u, true},       // 200 Hz
	{task_estimation_update, 5000u, 0u, true},// 200 Hz
	{task_publish_samples, 50000u, 0u, true},// 20 Hz
	{task_update_health, 200000u, 0u, true}, // 5 Hz	
	{task_ui_update, 100000u, 0u, true},      // 10 Hz
};

static constexpr std::size_t task_count = sizeof(task_table) / sizeof(task_table[0]);

// initializes task table timestamps
void scheduler_init();

// runs due tasks (loops)
void scheduler_tick();

} 

#endif
