# nanoflight-core
NanoFlight Core is a custom-built mini avionics and guidance computer running on an Arduino Uno and a 6-DOF MPU-6050 IMU with I2C telemetry. All components are designed from scratch, including an RTOS-like task scheduler for rate-determinism, sensor-fusion estimates for more precise data, a telemetry stream to a Python ground station for 3D visualization, and more. 

* Nanoflight Core was structurally designed to mimic the architetcture and implementation of real-world avionics firmware.
*  - All functionality is divided into modular and reusable components
*  - Communication is done by asynchronous message passing

![System Architecture](https://github.com/Adrianw27/nanoflight-core/blob/main/docs/diagrams/system_architecture.png?raw=true)

* The hardware consists of
* The software consists of

## Overview

This project implements a miniature avionics and guidance computer using an MPU-6050 IMU, plus a PC-based ground station for telemetry visualization and analysis. <br>

Most of my inspiration for the project came from the [PX4](https://github.com/PX4/PX4-Autopilot) autopilot solution for drones. It was through the PX4 source code that I learned about the system's software and hardware architecture, including the driver, middleware, flight stack, and ground station layers. PX4 also proved useful for learning various low-level modules and advanced topics such as EKF2 filters, PID flight controllers, and task schedulers. In my next project, I am planning use redeveloping a similar project on an STM32 with more advanced concepts, such as flight controlling, RTOS, ROS2, SLAM, and MavLink.

The goals are to:
- Perform real-time attitude estimation using a Kalman filter.
- Implement a custom non-blocking firmware task scheduler running at fixed frequencies.
- Stream telemetry to a Python-based ground station for 3D visualization and plotting.
- Practice embedded C++ architecture, scheduling, and sensor fusion.
- May implement: guidance/control logic on an 8-bit MCU.

## Repository Layout

- `firmware/`  
  - `avionics_firmware/`
    - `include/`
      - `config/` – global configuration (constants, pin mappings, common types).
      - `hal/` – hardware abstraction layer headers (I²C, IMU, timing, serial, etc.).
      - `fusion/` – Kalman filters and other sensor-fusion interfaces.
      - `controllers/` – guidance and control algorithm interfaces (e.g., attitude controller).
      - `telemetry/` – telemetry protocol and communication interfaces.
      - `app/` – high-level application interfaces (scheduler, modes, app entry points).
    - `src/`
      - `hal/` – implementations of hardware abstraction (IMU driver, timing utilities, serial helpers).
      - `fusion/` – Kalman filter and fusion algorithm implementations.
      - `controllers/` – control law implementations (P/PID, guidance logic, etc.).
      - `telemetry/` – telemetry packet formatting/parsing implementations.
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


