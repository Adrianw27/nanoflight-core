# Requirements (Initial Draft)

## Functional Requirements

- F1. The avionics firmware shall read IMU data (accelerometer and gyroscope) at a fixed rate (200 Hz).
- F2. The firmware shall estimate pitch angle using a Kalman filter and provide a real-time estimate at the same rate.
- F3. The firmware shall stream telemetry to the ground station over serial.
- F4. The ground station shall visualize the estimated attitude in 3D.
- F5. The ground station shall plot raw vs filtered sensor data for debugging and tuning.

## Non-Functional Requirements

- NF1. The firmware shall run within the memory and performance constraints of an Arduino Uno.
- NF2. The scheduler shall avoid blocking operations in timing-critical paths.
- NF3. The system shall be structured into clear modules for maintainability.
- NF4. Telemetry format shall be simple to parse and robust enough for debugging (CSV).

## Hardware Bring-Up Baseline

- B1. The firmware shall compile for Arduino Uno from the repository root with PlatformIO.
- B2. The firmware shall boot directly into active telemetry operation when IMU and serial initialization succeed.
- B3. The firmware shall enter `FAULT` mode when IMU reads or estimation updates fail.
- B4. The ground station shall parse firmware telemetry and display accelerometer, complementary, and Kalman attitude traces.
