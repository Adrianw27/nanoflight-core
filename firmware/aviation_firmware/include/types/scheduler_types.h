#ifndef TYPES_TASK_TYPES_H
#define TYPES_TASK_TYPES_H

#include <cstdint>

namespace Types {
    namespace Task {

        // Holds task table with funcs and timings for scheduler
        struct ScheduledTask {
            void (*task)();
            std::uint32_t period_micros;
            std::uint32_t last_run_micros;
            bool enabled;
        };

    }
}

#endif

