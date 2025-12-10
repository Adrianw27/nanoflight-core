#include <cstdint>
#include <cstddef>
#include "app/scheduler.h"
#include "app/tasks.h"
#include "hal/timing.h"

namespace app {

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
		ScheduledTask& t = task_table[i];
		if (!t.enabled) {
		    continue;
		}

		std::uint32_t elapsed = now - t.last_run_micros;
		if (elapsed >= t.period_micros) {
		    t.task();
		    t.last_run_micros = now;
		}

		if (elapsed > 1000) { 
			app::set_status_flag(app::STATUS_SCHEDULER_WARN);
	    	} 
		else {
			app::clear_status_flag(app::STATUS_SCHEDULER_WARN);
	    	}
	}
}

}
