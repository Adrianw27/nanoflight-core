from __future__ import annotations

import argparse
import math
import queue
from collections import deque
from dataclasses import dataclass
from typing import Optional

import matplotlib.pyplot as plt
import numpy as np
from matplotlib.animation import FuncAnimation

from telemetry_client.serial_reader import (
    DemoTelemetryReader,
    HealthTelemetry,
    SampleTelemetry,
    SerialTelemetryReader,
    TelemetryStreamThread,
)


@dataclass
class VisualizerState:
    latest_sample: Optional[SampleTelemetry] = None
    latest_health: Optional[HealthTelemetry] = None
    last_rendered_time_ms: Optional[int] = None


def rotation_matrix_from_pitch_roll(pitch_deg: float, roll_deg: float) -> np.ndarray:
    pitch_rad = math.radians(pitch_deg)
    roll_rad = math.radians(roll_deg)

    rotate_x = np.array(
        [
            [1.0, 0.0, 0.0],
            [0.0, math.cos(roll_rad), -math.sin(roll_rad)],
            [0.0, math.sin(roll_rad), math.cos(roll_rad)],
        ]
    )
    rotate_y = np.array(
        [
            [math.cos(pitch_rad), 0.0, math.sin(pitch_rad)],
            [0.0, 1.0, 0.0],
            [-math.sin(pitch_rad), 0.0, math.cos(pitch_rad)],
        ]
    )
    return rotate_y @ rotate_x


def build_vehicle_vertices() -> np.ndarray:
    return np.array(
        [
            [-1.0, -0.12, -0.35],
            [1.0, -0.12, -0.35],
            [1.0, 0.12, -0.35],
            [-1.0, 0.12, -0.35],
            [-1.0, -0.12, 0.35],
            [1.0, -0.12, 0.35],
            [1.0, 0.12, 0.35],
            [-1.0, 0.12, 0.35],
            [0.0, 0.0, 0.55],
        ]
    )


def vehicle_edges() -> list[tuple[int, int]]:
    return [
        (0, 1), (1, 2), (2, 3), (3, 0),
        (4, 5), (5, 6), (6, 7), (7, 4),
        (0, 4), (1, 5), (2, 6), (3, 7),
        (4, 8), (5, 8), (6, 8), (7, 8),
    ]


def draw_reference_axes(ax: plt.Axes) -> None:
    ax.plot([0.0, 1.4], [0.0, 0.0], [0.0, 0.0], color="#cf5c36", linewidth=2.0)
    ax.plot([0.0, 0.0], [0.0, 1.4], [0.0, 0.0], color="#2d6a4f", linewidth=2.0)
    ax.plot([0.0, 0.0], [0.0, 0.0], [0.0, 1.4], color="#2b59c3", linewidth=2.0)


def build_plot() -> tuple[plt.Figure, plt.Axes, plt.Axes]:
    plt.style.use("seaborn-v0_8-darkgrid")
    figure = plt.figure(figsize=(12, 7))
    figure.patch.set_facecolor("#f2efe8")

    axes_3d = figure.add_subplot(1, 2, 1, projection="3d")
    axes_3d.set_facecolor("#fff8ec")
    axes_3d.set_title("NanoFlight Attitude", fontsize=14, fontweight="bold")
    axes_3d.set_xlim(-1.5, 1.5)
    axes_3d.set_ylim(-1.5, 1.5)
    axes_3d.set_zlim(-1.5, 1.5)
    axes_3d.set_xlabel("X")
    axes_3d.set_ylabel("Y")
    axes_3d.set_zlabel("Z")
    axes_3d.view_init(elev=22, azim=38)

    axes_plot = figure.add_subplot(1, 2, 2)
    axes_plot.set_facecolor("#fff8ec")
    axes_plot.set_title("Attitude History", fontsize=14, fontweight="bold")
    axes_plot.set_xlabel("Time (s)")
    axes_plot.set_ylabel("Angle (deg)")
    axes_plot.set_ylim(-90.0, 90.0)

    return figure, axes_3d, axes_plot


def main() -> int:
    parser = argparse.ArgumentParser(description="NanoFlight ground station 3D visualizer.")
    parser.add_argument("--port", help="Serial port path, for example /dev/ttyACM0")
    parser.add_argument("--baud", type=int, default=115200, help="Serial baud rate")
    parser.add_argument("--demo", action="store_true", help="Run without serial hardware using generated demo data")
    parser.add_argument("--history", type=int, default=300, help="Number of samples to keep in the history plot")
    args = parser.parse_args()

    if not args.demo and not args.port:
        parser.error("either --demo or --port is required")

    source = DemoTelemetryReader() if args.demo else SerialTelemetryReader(args.port, args.baud)
    frame_queue: "queue.Queue[object]" = queue.Queue(maxsize=8)
    stream_thread = TelemetryStreamThread(source, frame_queue)
    stream_thread.start()

    figure, axes_3d, axes_plot = build_plot()
    state = VisualizerState()
    vertices = build_vehicle_vertices()
    edges = vehicle_edges()

    edge_lines = []
    for _ in edges:
        line, = axes_3d.plot([], [], [], color="#1f1f1f", linewidth=2.2)
        edge_lines.append(line)

    draw_reference_axes(axes_3d)

    pitch_history: deque[float] = deque(maxlen=args.history)
    roll_history: deque[float] = deque(maxlen=args.history)
    accel_pitch_history: deque[float] = deque(maxlen=args.history)
    accel_roll_history: deque[float] = deque(maxlen=args.history)
    complementary_pitch_history: deque[float] = deque(maxlen=args.history)
    complementary_roll_history: deque[float] = deque(maxlen=args.history)
    time_history: deque[float] = deque(maxlen=args.history)

    accel_pitch_line, = axes_plot.plot([], [], color="#8c8c8c", linewidth=1.4, linestyle="--", label="Accel Pitch")
    accel_roll_line, = axes_plot.plot([], [], color="#b4b4b4", linewidth=1.4, linestyle="--", label="Accel Roll")
    complementary_pitch_line, = axes_plot.plot([], [], color="#dd8452", linewidth=1.8, label="Complementary Pitch")
    complementary_roll_line, = axes_plot.plot([], [], color="#4c72b0", linewidth=1.8, label="Complementary Roll")
    pitch_line, = axes_plot.plot([], [], color="#cf5c36", linewidth=2.2, label="Kalman Pitch")
    roll_line, = axes_plot.plot([], [], color="#2b59c3", linewidth=2.2, label="Kalman Roll")
    axes_plot.legend(loc="upper left")

    status_text = figure.text(
        0.05,
        0.94,
        "Waiting for telemetry...",
        ha="left",
        va="center",
        fontsize=11,
        family="monospace",
        color="#1f1f1f",
    )

    def drain_queue() -> None:
        while True:
            try:
                frame = frame_queue.get_nowait()
            except queue.Empty:
                break

            if isinstance(frame, SampleTelemetry):
                state.latest_sample = frame
            elif isinstance(frame, HealthTelemetry):
                state.latest_health = frame

    def update(_frame_index: int):
        drain_queue()

        if stream_thread.error is not None:
            raise RuntimeError(f"telemetry stream failed: {stream_thread.error}")

        sample = state.latest_sample
        if sample is None:
            return edge_lines + [
                accel_pitch_line,
                accel_roll_line,
                complementary_pitch_line,
                complementary_roll_line,
                pitch_line,
                roll_line,
            ]

        rotation = rotation_matrix_from_pitch_roll(sample.kalman_pitch_deg, sample.kalman_roll_deg)
        transformed_vertices = (rotation @ vertices.T).T

        for line, edge in zip(edge_lines, edges):
            start = transformed_vertices[edge[0]]
            end = transformed_vertices[edge[1]]
            line.set_data_3d(
                [start[0], end[0]],
                [start[1], end[1]],
                [start[2], end[2]],
            )

        if state.last_rendered_time_ms != sample.time_ms:
            state.last_rendered_time_ms = sample.time_ms
            seconds = sample.time_ms / 1000.0
            time_history.append(seconds)
            accel_pitch_history.append(sample.accel_pitch_deg)
            accel_roll_history.append(sample.accel_roll_deg)
            complementary_pitch_history.append(sample.complementary_pitch_deg)
            complementary_roll_history.append(sample.complementary_roll_deg)
            pitch_history.append(sample.kalman_pitch_deg)
            roll_history.append(sample.kalman_roll_deg)

        accel_pitch_line.set_data(time_history, accel_pitch_history)
        accel_roll_line.set_data(time_history, accel_roll_history)
        complementary_pitch_line.set_data(time_history, complementary_pitch_history)
        complementary_roll_line.set_data(time_history, complementary_roll_history)
        pitch_line.set_data(time_history, pitch_history)
        roll_line.set_data(time_history, roll_history)

        if time_history:
            start_time = max(0.0, time_history[-1] - 10.0)
            axes_plot.set_xlim(start_time, max(start_time + 10.0, time_history[-1] + 0.1))

        health = state.latest_health
        mode_text = "?"
        flag_text = "?"
        if health is not None:
            mode_text = str(health.mode)
            flag_text = f"0x{health.status_flags:02X}"

        status_text.set_text(
            "pitch={:7.2f} deg  roll={:7.2f} deg  temp={:6.2f} C  mode={}  flags={}".format(
                sample.kalman_pitch_deg,
                sample.kalman_roll_deg,
                sample.temp_cel,
                mode_text,
                flag_text,
            )
        )

        return edge_lines + [
            accel_pitch_line,
            accel_roll_line,
            complementary_pitch_line,
            complementary_roll_line,
            pitch_line,
            roll_line,
        ]

    animation = FuncAnimation(figure, update, interval=50, blit=False, cache_frame_data=False)
    figure._animation = animation  # keep a strong reference for matplotlib

    try:
        plt.tight_layout()
        plt.show()
    finally:
        stream_thread.stop()
        stream_thread.join(timeout=1.0)

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
