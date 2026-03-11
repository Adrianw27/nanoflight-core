#ifndef MIDDLEWARE_HEALTH_HEALTH_H
#define MIDDLEWARE_HEALTH_HEALTH_H

#include "config/types.h"

class Health {
    public:
        Health();

        // publish health data
        void update_health();

    private:
        static constexpr HealthState healthState;
};

#endif
