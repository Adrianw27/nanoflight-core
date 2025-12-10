# Scheduler Design

## 1. Goals

The scheduler is responsible for running all firmware tasks at predictable, fixed rates on a single-core MCU (Arduino Uno), without an RTOS.

Design goals:

- **Deterministic timing** for time-critical tasks (IMU sampling, state estimation, control).
- **Cooperative, non-preemptive** execution (simple to reason about, RTOS-like structure without an OS).
- **No dynamic memory allocation** in the scheduler or tasks.
- **Clear separation of concerns**: the scheduler decides when to run tasks; tasks themselves decide what to do.

The design should be portable to a future RTOS-based implementation (e.g., STM32 + FreeRTOS) by mapping tasks to threads with similar periods.

---

## 2. Task Model

The system uses a **periodic task model**: each task has a fixed period and runs as often as possible at that rate.

Planned tasks:

| Task Name         | Responsibility                          | Target Rate |
|-------------------|-----------------------------------------|-------------|
| `task_imu_read`     | Read raw IMU sample from HAL            | 200–500 Hz  |
| `task_estimation`  | Run Kalman filter update                | 200–500 Hz  | 
| `task_telemetry`   | Send telemetry over serial              |  20–50 Hz   |
| `task_ui`          | Update LEDs/LCD/encoder/buzzer          |   5–20 Hz   |

---

## 3. Timing Model

### 3.1 Time Source

- The scheduler uses microsecond timing from HAL:
  - `hal::get_current_micros()` 
  - `hal::compute_delta_seconds()`

### 3.2 Periods and Jitter

Each task has:

- `period_micros` – desired period in microseconds
- `last_run_micros` – last time the task executed

Effective rate (when a task runs):

```cpp
if (now - last_run_micros >= period_micros) {
    run_task();
    last_run_micros = now;
}

