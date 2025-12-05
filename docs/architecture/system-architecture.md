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

## System Architecture Diagram

## Firmware Architectural Layers 

### Hardware Abstraction Layer (HAL)

- **Implementation Details:**
    - No STL
    - Static allocation
    - Deterministic timing

### Scheduler Layer

- **Implementaion Details:**
    - Deterministic
    - No dynamic allocation
    - Tasks run in precise frequency bins (1000 Hz IMU, 100 Hz fusion, 50 Hz control)

### Sensor Layer  

### State Estimation Layer

### Control Layer

### Communication Layer

### Application Layer

## Ground Station Architecture (Python)

- **Backend:**
    - Telemetry
    - FastAPI
- **Serial Client:**
- **UI/Dashboard:**
    - matplotlib visualization

## Block Diagrams
