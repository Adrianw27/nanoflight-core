#include <cstdint>
#include <cstddef>
#include "app/scheduler.h"
#include "app/tasks.h"
#include "hal/timing.h"

namespace app {

static ScheduledTask task_table[] = {
	{task_imu_update, 5000u, 0u, true},       // 200 Hz
	{task_estimation_update, 5000u, 0u, true},// 200 Hz
	{task_telemetry_update, 50000u, 0u, true},// 20 Hz
	{task_ui_update, 100000u, 0u, true},      // 10 Hz
};

static constexpr std::size_t task_count = sizeof(task_table) / sizeof(task_table[0]);

void scheduler_init() {
    std::uint32_t now = hal::get_current_micros();
    for (std::size_t i = 0; i < task_count; ++i) {
        task_table[i].last_run_micros = now;
    }
}

void scheduler_tick() {
    std::uint32_t now = hal::get_current_micros();

    for (std::size_t i = 0; i < task_count; ++i) {
        ScheduledTask& t = task_table[i];
        if (!t.enabled) {
            continue;
        }
        std::uint32_t elapsed = now - t.last_run_micros;
        if (elapsed >= t.period_micros) {
            t.task();
            t.last_run_micros = now;
        }
    }
}

}
