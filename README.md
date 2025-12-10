# nanoflight-core
NanoFlight Core is a custom-built mini avionics and guidance computer running on an Arduino Uno and a 6-DOF MPU-6050 IMU with I2C telemetry. The project implements a custom RTOS-like task scheduler running on a fixed-rate.

![System Architecture](https://github.com/Adrianw27/nanoflight-core/blob/main/docs/diagrams/system_architecture.png?raw=true)

## Overview

This project implements a miniature avionics and guidance computer on an Arduino Uno using an MPU-6050 IMU, plus a PC-based ground station for telemetry visualization and analysis.

The goals are to:
- Perform real-time attitude estimation using a Kalman filter.
- Implement a custom non-blocking firmware task scheduler running at fixed frequencies.
- Run guidance/control logic on an 8-bit MCU under tight resource constraints.
- Stream telemetry to a Python-based ground station for 3D visualization and plotting.
- Practice embedded C++ architecture, scheduling, and sensor fusion.
- May implement: PID servo control based on fused orientation.

## Repository Layout

- `firmware/`  
  - `avionics_firmware/`
    - `include/`
      - `config/` – global configuration (constants, pin mappings, common types).
      - `hal/` – hardware abstraction layer headers (I²C, IMU, timing, serial, etc.).
      - `fusion/` – Kalman filters and other sensor-fusion interfaces.
      - `controllers/` – guidance and control algorithm interfaces (e.g., attitude controller).
      - `comms/` – telemetry protocol and communication interfaces.
      - `app/` – high-level application interfaces (scheduler, modes, app entry points).
    - `src/`
      - `hal/` – implementations of hardware abstraction (IMU driver, timing utilities, serial helpers).
      - `fusion/` – Kalman filter and fusion algorithm implementations.
      - `controllers/` – control law implementations (P/PID, guidance logic, etc.).
      - `comms/` – telemetry packet formatting/parsing implementations.
      - `app/` – application logic, cooperative scheduler, mode state machine.
      - `main.cpp` – firmware entry point tying all modules together.
  - `tests/`
    - `unit/` – unit tests for fusion, controllers, etc. (using offline data or simulated inputs).
    - `hardware_sim/` – replay tests that feed recorded IMU logs into the algorithms.
    - `integration/` – end-to-end tests of firmware behavior and telemetry formats.

- `ground_station/`
  - `python/`
    - `telemetry_client/` – code that connects to the MCU over serial and consumes telemetry.
    - `ui/` – 3D visualization and plotting components.
    - `backend/` – optional REST/WebSocket backend for more advanced ground-station setups.

- `docs/`
  - `architecture/` – system architecture, scheduler design, and high-level design notes.
  - `requirements/` – functional/non-functional requirements and project schedule.
  - `diagrams/` – block diagrams, timing diagrams, and other visual documentation.
  - `devlog.md` – chronological development log.

- `media/` – screenshots, wiring diagrams, setup photos, and demo images/videos.


