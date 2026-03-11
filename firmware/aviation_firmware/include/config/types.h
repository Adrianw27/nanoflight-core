#ifndef CONFIG_TYPES_H
#define CONFIG_TYPES_H

#include "types/attitude_types.h"
#include "types/imu_types.h"
#include "types/mode_types.h"
#include "types/scheduler_types.h"
#include "types/status_flag_types.h"
#include "types/telemetry_types.h"

namespace types {

using RawAccelSample = Types::Imu::RawAccelSample;
using RawGyroSample = Types::Imu::RawGyroSample;
using ScaledAccelSample = Types::Imu::ScaledAccelSample;
using ScaledGyroSample = Types::Imu::ScaledGyroSample;

using AttitudeState = Types::Attitude::AttitudeState;
using ComplementaryAxisState = Types::Attitude::ComplementaryAxisState;
using ComplementaryFilterState = Types::Attitude::ComplementaryFilterState;
using KalmanAxisState = Types::Attitude::KalmanAxisState;
using KalmanFilterState = Types::Attitude::KalmanFilterState;

using Mode = Types::Mode::Mode;
using ScheduledTask = Types::Task::ScheduledTask;
using StatusFlag = Types::StatusFlags::StatusFlag;
using HealthTelemetry = Types::Telemetry::HealthTelemetry;

}

#endif
