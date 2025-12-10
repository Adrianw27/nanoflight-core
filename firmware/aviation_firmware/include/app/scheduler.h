#ifndef APP_SCHEDULER_H
#define APP_SCHEDULER_H

#include <cstdint>

namespace app {

struct ScheduledTask {
	void (*task)();
	std::uint32_t period_micros;
	std::uint32_t last_run_micros;
	bool enabled;
};

// initializes task table timestamps
void scheduler_init();

// runs due tasks (loops)
void scheduler_tick();

}  // namespace app

#endif
