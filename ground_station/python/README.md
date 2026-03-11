# Ground Station

Python ground-station tools for NanoFlight.

## Components

- `serial_reader.py` reads CSV telemetry from the firmware and prints parsed frames.
- `visualizer_3d.py` renders the Kalman attitude in a live matplotlib 3D view and shows a short angle history.

## Install

```bash
python3 -m venv .venv
source .venv/bin/activate
pip install -r requirements.txt
```

## Run

Demo mode:

```bash
python3 visualizer_3d.py --demo
```

Live serial mode:

```bash
python3 visualizer_3d.py --port /dev/ttyACM0 --baud 115200
```

Inspect parsed telemetry without the UI:

```bash
python3 serial_reader.py --port /dev/ttyACM0 --baud 115200
```
