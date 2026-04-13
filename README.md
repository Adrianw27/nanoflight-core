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

## Current Status

The repo is set up for Arduino Uno hardware bring-up with:

- a buildable firmware layout under `firmware/aviation_firmware/`
- MPU-6050 initialization and scaled IMU reads over I2C
- complementary and Kalman attitude estimation
- serial telemetry with schema, sample, and health frames
- a Python ground station with live 3D view and attitude-history plots

The remaining work after bring-up is tuning and extending behavior, not wiring basic structure together.

## Repository Layout

- `firmware/`  
  - `aviation_firmware/`
    - `include/`
      - `config/` – global configuration (constants, pin mappings, common types).
      - `hal/` – hardware abstraction layer headers (I²C, IMU, timing, serial, etc.).
      - `fusion/` – Kalman filters and other sensor-fusion interfaces.
      - `app/` – high-level application interfaces (scheduler, modes, app entry points).
      - `comms/` and `middleware/telemetry/` – telemetry protocol and communication interfaces.
    - `src/`
      - `hal/` – implementations of hardware abstraction (IMU driver, timing utilities, serial helpers).
      - `fusion/` – Kalman filter and fusion algorithm implementations.
      - `telemetry/` – telemetry packet formatting/parsing implementations.
      - `modules/` – application logic, cooperative scheduler, mode state machine.
      - `main.cpp` – firmware entry point tying all modules together.

- `ground_station/`
  - `python/`
    - `telemetry_client/` – code that connects to the MCU over serial and consumes telemetry.
    - `ui/` – 3D visualization and plotting components.

- `docs/`
  - `architecture/` – system architecture, scheduler design, and high-level design notes.
  - `requirements/` – functional/non-functional requirements and project schedule.
  - `diagrams/` – block diagrams, timing diagrams, and other visual documentation.

## Firmware Build

The firmware uses PlatformIO from the repository root:

```bash
pio run -e uno
pio run -e uno -t upload
pio device monitor -b 115200
```

Wiring for Arduino Uno + MPU-6050:

- `VCC` -> `5V`
- `GND` -> `GND`
- `SDA` -> `A4`
- `SCL` -> `A5`

## Ground Station

From `ground_station/python/`:

```bash
python3 -m venv .venv
source .venv/bin/activate
pip install -r requirements.txt
python3 visualizer_3d.py --port /dev/ttyACM0 --baud 115200
```
