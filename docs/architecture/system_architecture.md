# System Architecture

## High-Level Overview

The system consists of two major parts:

- **Avionics firmware** running on an Arduino Uno R3:
  - Reads IMU data (MPU-6050) at a fixed rate.
  - Runs a Kalman filter to estimate attitude (initially 1D pitch).
  - Executes guidance/control logic.
  - Uses a cooperative scheduler to run tasks with defined periods.
  - Streams telemetry over serial.

- **Ground station** running on a PC:
  - Receives telemetry from the firmware.
  - Visualizes attitude in 3D.
  - Plots raw and filtered sensor data.
  - (Future) Sends commands and configuration back to the avionics computer.
  - Stores logs and configuration.

More detailed block diagrams, timing diagrams, and details will be added as the implementation progresses.

---

## System Architecture Diagram

![System Architecture](https://github.com/Adrianw27/nanoflight-core/blob/main/docs/diagrams/system_architecture.png?raw=true)

---

## Firmware Architectural Layers 

### Hardware Abstraction Layer (HAL)

**Responsibilities:**
- Provide a testable interface to all hardware:
  - IMU (MPU-6050) over I²C
  - Timing utilities (micros/millis, dt computation)
  - Serial port (USB serial) for telemetry
  - (Future) GPIOs for buttons, encoder, LCD, buzzer, servo, RTC, etc.
- Hide Arduino-specific details from higher layers so the system can be ported to other MCUs (e.g., STM32 + RTOS) with minimal changes.

**Implementation Details:**
- No STL 
- Static allocation only
- Deterministic timing: no blocking I/O in HAL calls used by real-time tasks.

---

### Scheduler Layer

**Responsibilities:**
- Provide a cooperative, time-driven scheduler that:
  - Runs tasks at fixed, predictable rates.
  - Avoids blocking calls in time-critical code.
  - Centralizes timing so the control/estimation layers can assume stable dt.
- Encapsulate when tasks run so higher-level layers focus on what tasks do.

**Implementation Details:**
- Deterministic, cooperative.
- No dynamic allocation.
- Tasks run in fixed frequency bins. 

---

### Sensor Layer 

**Responsibilities:**
- Deal with **sensor-domain data**, built on top of HAL:
  - Convert raw IMU readings into physical units.

**Implementation Details:**
- Resides between HAL and fusion:
  - HAL knows about registers and I²C.
  - Sensor layer knows about physical units and sensor semantics.
- No dynamic allocation.

### State Estimation Layer

**Responsibilities:**
- Take sensor-layer data and estimate the system state:
  - Initially: 1D pitch angle (and gyro bias) via a Kalman filter.
  - Later: add roll, maybe full attitude if extended.
- Fuse accelerometer (absolute but noisy) and gyro (smooth but drift-prone).

**Implementation Details:**
- No STL, no dynamic allocation.
- All state stored in the filter object as plain members.
- Tunable Q/R parameters exposed via constants or configuration structure.

---

### Communication Layer

**Responsibilities:**
- Define and implement the **telemetry protocol** between avionics and ground station:
  - CSV format.
- (Future) Handle command uplink from ground station.

**Implementation Details:**
- Uses HAL serial functions to send bytes/strings.
- No dynamic allocation.

--- 

### Application Layer

**Responsibilities:**
- Implement the **high-level avionics behavior**:
  - Mode state machine: STANDBY, ARMED, ACTIVE, FAULT.
  - Own and configure tasks in the scheduler.
- Tie together HAL, sensors, fusion, control, and comms.
- Define the boot sequence and safety checks.

**Implementation Details:**
- No blocking logic in core loop.
- Transitions between modes based on:
  - Sensor health
  - External input (button/encoder/command)
  - Internal diagnostics (future)

---

### Control Layer (Future)

- PID + servo in future work

---

## Ground Station Architecture (Python)

### Serial Client

**Responsibilities:**
- Connect to the Arduino over serial.
- Read telemetry frames (CSV).
- Parse into Python objects or dicts.
- Forward parsed data to:
  - UI/visualization
  - (Future) FastAPI backend.

---

### UI / Dashboard

**Responsibilities:**
- Provide a live visual window into the system:
  - 3D visualization of estimated attitude (matplotlib).
  - Time-series plots of raw accelerometer, gyro, Kalman angle, and control output.
- Optionally integrate with backend to:
  - Show system status.
  - Replay logs.
  - Display configuration and mode.

---

### Backend

**Responsibilities:**
- (Future) Optional FastAPI-based backend service to:
  - Expose telemetry and status via HTTP APIs.
  - Store telemetry in a simple database or log format.

- **Serial Client:**
- **UI/Dashboard:**
    - matplotlib visualization

---

## Block Diagrams

### Initial Block Diagram

![Hardware Block Diagram](https://github.com/Adrianw27/nanoflight-core/blob/main/docs/diagrams/hardware_block_diagram.png?raw=true)

- **Arduino Uno R3** acts as the avionics computer.
- **GY-521 (MPU-6050)** is connected to the Arduino via I²C (SDA/SCL) and provides 3-axis accelerometer and gyroscope data.
- **PC Ground Station** is connected over USB serial to receive telemetry and visualize the estimated attitude.

High-level signal flow:

IMU (MPU-6050) → Arduino (Kalman fusion + control + telemetry) → PC (Ground station visualization)
