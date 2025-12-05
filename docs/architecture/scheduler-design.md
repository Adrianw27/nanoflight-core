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

The system uses a **periodic task model**: each task has a fixed period and runs as often as possible at that rate, subject to being non-blocking.

Planned tasks (initial set):

| Task Name         | Responsibility                          | Target Rate | Layer(s) Touched           |
|-------------------|-----------------------------------------|------------:|----------------------------|
| `taskImuRead`     | Read raw IMU sample from HAL            | 200–500 Hz  | HAL, Sensor Layer          |
| `taskEstimation`  | Run Kalman filter update                | 200–500 Hz  | State Estimation           |
| `taskTelemetry`   | Send telemetry over serial              |  20–50 Hz   | Comms Layer, App           |
| `taskUi`          | Update LEDs/LCD/encoder/buzzer          |   5–20 Hz   | App, HAL (future UI parts) |

Notes:
- Early versions won't need taskUi
- For simplicity, `taskImuRead` and `taskEstimation` may initially be combined into a single high-rate task.

---

## 3. Timing Model

### 3.1 Time Source

- The scheduler uses microsecond timing from HAL:
  - `hal::Timing::nowMicros()` 
  - `hal::Timing::computeDeltaSeconds()`

### 3.2 Periods and Jitter

Each task has:

- `periodMicros` – desired period in microseconds
- `lastRunMicros` – last time the task executed

Effective rate (when a task runs):

```cpp
if (now - lastRunMicros >= periodMicros) {
    runTask();
    lastRunMicros = now;
}

