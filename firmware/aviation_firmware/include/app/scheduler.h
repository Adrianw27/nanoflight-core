#ifndefine APP_SCHEDULER_H
#define APP_SCHEDULER_H

namespace App {

// initializes task table timestamps
void scheduler_init();

// runs due tasks (loops)
void scheduler_tick();

}

#endif
