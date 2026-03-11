#include <cstddef>
#include <cstdint>
#include "app/mode.h"
#include "app/scheduler.h"
#include "app/status_flags.h"
#include "app/tasks.h"
#include "config/types.h"
#include "hal/timing.h"

namespace app {

static types::ScheduledTask task_table[] = {
	{task_imu_update, 5000u, 0u, true},         // 200 Hz
	{task_estimation_update, 5000u, 0u, true},  // 200 Hz
	{task_publish_samples, 50000u, 0u, true},   // 20 Hz
	{task_update_health, 200000u, 0u, true},    // 5 Hz
	{task_ui_update, 100000u, 0u, true},        // 10 Hz
};

static constexpr std::size_t task_count = sizeof(task_table) / sizeof(task_table[0]);

void scheduler_init() {
	app::set_mode(app::Mode::Init);
	std::uint32_t now = hal::get_current_micros();
	for (std::size_t i = 0; i < task_count; ++i) {
		task_table[i].last_run_micros = now;
	}
}

void scheduler_tick() {
	app::set_mode(app::Mode::Active);

	std::uint32_t now = hal::get_current_micros();

	for (std::size_t i = 0; i < task_count; ++i) {
		types::ScheduledTask& t = task_table[i];
		if (!t.enabled) {
			continue;
		}

		std::uint32_t elapsed = now - t.last_run_micros;
		if (elapsed >= t.period_micros) {
			t.task();
			t.last_run_micros = now;
		}

		if (elapsed > (t.period_micros * 2u)) {
			app::set_status_flag(app::STATUS_SCHEDULER_WARN);
		} else {
			app::clear_status_flag(app::STATUS_SCHEDULER_WARN);
		}
	}
}

}
