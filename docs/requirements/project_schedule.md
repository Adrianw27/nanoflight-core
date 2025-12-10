
# Mini Avionics & Guidance Computer — Project Schedule
For personal use to track project completion
## Phase 0 – Project Setup (0.5–1 day)
- Create GitHub repository with base structure (`firmware/`, `ground_station/`, `docs/`, `media/`).
- Add top-level `README.md` with:
  - Project name and 2–3 sentence goal.
  - Planned features and high-level description.
- Create empty docs:
  - `docs/system-architecture.md`
  - `docs/math-kalman-filter.md`
  - `docs/data-format.md`
  - `docs/devlog.md`
- Log **Day 0** in `devlog.md` with goals, hardware list, and what you plan to learn.

---

## Phase 1 – Hardware Bring-Up & Raw IMU Data (1–3 days)
- Wire **MPU6050 (GY-521)** to Arduino Uno (VCC, GND, SDA, SCL).
- Implement `imu_driver`:
  - Initialize MPU6050 over I²C.
  - Read raw accelerometer and gyroscope data.
- Print raw data over Serial for inspection.
- Verify behavior:
  - Stationary → ~1 g on one axis of accelerometer.
  - Tilting/moving → reasonable accel/gyro changes.
- Documentation updates:
  - Add a simple hardware block diagram to `system-architecture.md`.
  - Log first successful raw data read in `devlog.md` with example values.

---

## Phase 2 – Simple Angle Estimation & Logging (2–4 days)
- Compute **accel-based tilt angle** (e.g., pitch) from accelerometer using `atan2`.
- Integrate **gyro rate** to estimate angle over time.
- Stream raw and derived data over Serial as CSV:
  - Format: `time_ms,accel_angle_deg,gyro_angle_deg`.
- Create a minimal **Python script** to:
  - Read the CSV stream.
  - Plot accel-angle vs gyro-integrated angle over time.
  - Optionally save logs to disk for offline analysis.
- Documentation updates:
  - Add derivation of accel-based angle and gyro integration to `math-kalman-filter.md`.
  - Include at least one plot (screenshot) in `media/` and reference it from `math-kalman-filter.md`.
  - Log tuning observations in `devlog.md` (e.g., gyro drift, accel noise).

---

## Phase 3 – 1D (then 2D) Kalman Filter (4–7 days)
- Implement a **1D Kalman filter** for pitch angle:
  - State: angle + gyro bias.
  - Process and measurement models.
  - Q and R covariance matrices.
- Replace naive fusion with Kalman output:
  - Stream: `time_ms,kalman_angle_deg,accel_angle_deg,gyro_rate_deg_s`.
- Tune Q/R to achieve:
  - Smooth output with low lag.
  - Reduced drift compared to pure gyro integration.
- Extend to roll (optional at this stage) by instantiating a second Kalman filter.
- Python updates:
  - Plot Kalman vs accel vs gyro for comparison.
- Documentation updates:
  - Fully document the state-space model and Kalman equations in `math-kalman-filter.md`.
  - Include “before vs after” plots showing filter improvement.
  - Log tuning steps and values tried in `devlog.md`.

---

## Phase 4 – Ground Station 3D Visualization (2–4 days)
- Implement **Python ground station** in `ground_station/python/`:
  - `serial_reader.py` to consume telemetry from Arduino.
  - `visualizer_3d.py` to display a 3D cube/vehicle whose attitude follows the IMU:
    - Use `vpython` or another 3D library.
- Integrate:
  - Run firmware + Python visualizer → confirm live motion tracking.
- Documentation updates:
  - Add a data-flow diagram (Arduino → Serial → Ground Station → Visualization) to `system-architecture.md`.
  - Capture screenshots or a GIF of the visualization and store in `media/`.
  - Log the first successful end-to-end demo in `devlog.md`.

---

## Phase 5 – Guidance Logic & Cooperative Scheduler (5–10 days)
- Implement a simple **mode state machine** for the avionics computer:
  - Modes: `STANDBY`, `ARMED`, `ACTIVE` (and optionally `FAULT`).
- Implement **guidance/control logic**:
  - Start simple: target pitch angle (e.g., 0°).
  - Compute error = target – current angle.
  - Generate a “command” (e.g., control output) that could later drive a servo.
- Implement a **cooperative task scheduler** on Arduino:
  - Tasks (with example rates):
    - `task_imu_read` @ 200 Hz.
    - `task_kalman_update` @ 200 Hz.
    - `task_guidance` @ 100 Hz.
    - `task_telemetry` @ 50 Hz.
    - `task_ui` (LED/button/LCD) @ 10–20 Hz.
  - Use `micros()`/`millis()` to manage task timing (no RTOS yet).
- If using a small servo (recommended demo):
  - Map control command to servo position for a physical “control surface”/indicator.
- Documentation updates:
  - Describe the scheduler design and each task’s rate and responsibilities in `system-architecture.md`.
  - Add a simple timing diagram (even textual) showing how often each task runs.
  - Log progress and any timing or stability issues in `devlog.md`.

---

## Phase 6 – Polish, UI Enhancements & Future Work (2–5 days + ongoing)
- Polish the **top-level README**:
  - Add clear feature list.
  - Include hardware list with links/specs.
  - Show end-to-end workflow (firmware build + ground station run).
  - Embed diagrams and screenshots from `media/`.
- Create a short **demo video** (if possible) showing:
  - Moving the IMU/rig.
  - Live 3D visualization on the ground station.
  - (Optional) Servo/control surface responding to guidance.

### Future Work & Enhancement Ideas
These are features you can list even if partially implemented, and expand over time:

- **UI Mode Management**
  - Use button/joystick/rotary encoder to:
    - Switch between STANDBY/ARMED/ACTIVE/FAULT modes.
    - Change target angle or guidance mode on the fly.

- **Rotary Encoder Integration**
  - Use the rotary encoder as a precise input device for:
    - Adjusting Kalman filter Q/R parameters.
    - Tuning control gains (P/I/D).
    - Navigating settings menus on the LCD.

- **LCD1602 Integration**
  - Display live telemetry on the device:
    - Mode (STANDBY/ARMED/ACTIVE).
    - Current pitch/roll angle.
    - Key parameters (e.g., Q/R values, control gains).
  - Implement simple menu navigation using buttons/encoder/joystick.

- **Buzzer Feedback**
  - Add audio cues for:
    - Mode transitions (e.g., STANDBY → ARMED → ACTIVE).
    - Fault conditions (sensor failure, out-of-range readings).
    - Startup self-test pass/fail tones.

- **DS3231 RTC Integration**
  - Time-stamp logged data with real date/time for:
    - Better correlation with tests and experiments.
    - More realistic “flight log” behavior.
  - Optionally sync RTC time via ground station.

- **Small Servo + Physical Rig**
  - Build a small physical rig (e.g., a hinged arm or mini “control surface”) actuated by a servo:
    - Visualize control commands as motion.
    - Demonstrate step responses: step input in target angle → servo follows Kalman-estimated angle via control loop.
  - Explore basic closed-loop response metrics (rise time, overshoot, settling time).

- **Extended Guidance & Control**
  - Move from simple proportional control to PID.
  - Add multiple modes:
    - Angle hold.
    - Step response demo.
    - “Simulated trajectory” mode where the vehicle follows a predefined profile.

- **RTOS Migration (Future Platform)**
  - Port the avionics logic to an RTOS-based platform (e.g., FreeRTOS on STM32 or another MCU):
    - Map tasks to RTOS threads.
    - Use queues for telemetry and sensor data.
    - Explore priority assignment and timing guarantees.

- **Advanced Sensor Fusion**
  - Extend from 1D/2D to full 3D attitude (quaternions).
  - Fuse additional sensors in the future (e.g., magnetometer, barometric altitude, GPS).

- **Advanced Ground Station Features**
  - Add multi-graph plots (angle, rates, control output).
  - Add a simple GUI to send commands back to the avionics computer (e.g., set mode/target from PC).
  - Implement logging and playback for “post-flight” analysis.

---

This schedule is a guideline: you can compress or expand phases depending on how deep you go (especially for Kalman tuning and guidance logic). Focus on getting a vertical slice working (sensor → fusion → telemetry → visualization) before adding more features.
