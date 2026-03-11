from __future__ import annotations

import argparse
import csv
import math
import queue
import threading
import time
from dataclasses import dataclass
from typing import Dict, Iterable, Iterator, Optional, Union

import serial
from serial import SerialException


@dataclass(frozen=True)
class SampleTelemetry:
    time_ms: int
    ax_g: float
    ay_g: float
    az_g: float
    gx_dps: float
    gy_dps: float
    gz_dps: float
    temp_cel: float
    accel_pitch_deg: float
    accel_roll_deg: float
    complementary_pitch_deg: float
    complementary_roll_deg: float
    kalman_pitch_deg: float
    kalman_roll_deg: float


@dataclass(frozen=True)
class HealthTelemetry:
    uptime_ms: int
    mode: int
    status_flags: int


TelemetryFrame = Union[SampleTelemetry, HealthTelemetry]


class TelemetryParser:
    def __init__(self) -> None:
        self._schemas: Dict[str, list[str]] = {}

    def parse_line(self, line: str) -> Optional[TelemetryFrame]:
        stripped = line.strip()
        if not stripped:
            return None

        row = next(csv.reader([stripped]))
        if not row:
            return None

        frame_type = row[0]
        if frame_type == "SCHEMA":
            self._parse_schema(row)
            return None

        if frame_type == "DATA":
            return self._parse_data(row)

        if frame_type == "HEALTH":
            return self._parse_health(row)

        raise ValueError(f"unsupported telemetry frame type: {frame_type}")

    @property
    def schemas(self) -> Dict[str, list[str]]:
        return dict(self._schemas)

    def _parse_schema(self, row: list[str]) -> None:
        if len(row) < 3:
            raise ValueError("schema frame missing columns")

        schema_type = row[1]
        self._schemas[schema_type] = row[2:]

    def _parse_data(self, row: list[str]) -> SampleTelemetry:
        expected_columns = 15
        if len(row) != expected_columns:
            raise ValueError(f"DATA frame expected {expected_columns} columns, got {len(row)}")

        return SampleTelemetry(
            time_ms=int(row[1]),
            ax_g=float(row[2]),
            ay_g=float(row[3]),
            az_g=float(row[4]),
            gx_dps=float(row[5]),
            gy_dps=float(row[6]),
            gz_dps=float(row[7]),
            temp_cel=float(row[8]),
            accel_pitch_deg=float(row[9]),
            accel_roll_deg=float(row[10]),
            complementary_pitch_deg=float(row[11]),
            complementary_roll_deg=float(row[12]),
            kalman_pitch_deg=float(row[13]),
            kalman_roll_deg=float(row[14]),
        )

    def _parse_health(self, row: list[str]) -> HealthTelemetry:
        expected_columns = 4
        if len(row) != expected_columns:
            raise ValueError(f"HEALTH frame expected {expected_columns} columns, got {len(row)}")

        return HealthTelemetry(
            uptime_ms=int(row[1]),
            mode=int(row[2]),
            status_flags=int(row[3]),
        )


class SerialTelemetryReader:
    def __init__(self, port: str, baud_rate: int = 115200, timeout: float = 0.25) -> None:
        self._port = port
        self._baud_rate = baud_rate
        self._timeout = timeout
        self._parser = TelemetryParser()

    @property
    def parser(self) -> TelemetryParser:
        return self._parser

    def frames(self) -> Iterator[TelemetryFrame]:
        with serial.Serial(self._port, self._baud_rate, timeout=self._timeout) as connection:
            while True:
                raw = connection.readline()
                if not raw:
                    continue

                line = raw.decode("utf-8", errors="ignore")
                frame = self._parser.parse_line(line)
                if frame is not None:
                    yield frame


class DemoTelemetryReader:
    def __init__(self, period_seconds: float = 0.05) -> None:
        self._period_seconds = period_seconds

    def frames(self) -> Iterator[TelemetryFrame]:
        start_time = time.monotonic()
        last_health_second = -1
        while True:
            elapsed = time.monotonic() - start_time
            pitch_deg = 20.0 * math.sin(elapsed * 0.9)
            roll_deg = 35.0 * math.sin(elapsed * 0.6)
            yield SampleTelemetry(
                time_ms=int(elapsed * 1000.0),
                ax_g=0.0,
                ay_g=0.0,
                az_g=1.0,
                gx_dps=roll_deg * 0.05,
                gy_dps=pitch_deg * 0.05,
                gz_dps=0.0,
                temp_cel=23.0,
                accel_pitch_deg=pitch_deg * 1.1,
                accel_roll_deg=roll_deg * 1.1,
                complementary_pitch_deg=pitch_deg * 0.98,
                complementary_roll_deg=roll_deg * 0.98,
                kalman_pitch_deg=pitch_deg,
                kalman_roll_deg=roll_deg,
            )
            elapsed_second = int(elapsed)
            if elapsed_second % 2 == 0 and elapsed_second != last_health_second:
                last_health_second = elapsed_second
                yield HealthTelemetry(
                    uptime_ms=int(elapsed * 1000.0),
                    mode=4,
                    status_flags=0x03,
                )
            time.sleep(self._period_seconds)


class TelemetryStreamThread(threading.Thread):
    def __init__(self, reader: Union[SerialTelemetryReader, DemoTelemetryReader], out_queue: "queue.Queue[TelemetryFrame]") -> None:
        super().__init__(daemon=True)
        self._reader = reader
        self._out_queue = out_queue
        self._stop_event = threading.Event()
        self._error: Optional[BaseException] = None

    @property
    def error(self) -> Optional[BaseException]:
        return self._error

    def stop(self) -> None:
        self._stop_event.set()

    def run(self) -> None:
        try:
            for frame in self._reader.frames():
                if self._stop_event.is_set():
                    return

                try:
                    self._out_queue.put(frame, timeout=0.1)
                except queue.Full:
                    try:
                        self._out_queue.get_nowait()
                    except queue.Empty:
                        pass
                    self._out_queue.put(frame, timeout=0.1)
        except BaseException as exc:  # pragma: no cover - surfaced through UI loop
            self._error = exc


def open_line_source(path: str) -> Iterable[str]:
    with open(path, "r", encoding="utf-8") as handle:
        for line in handle:
            yield line


def parse_file(path: str) -> Iterator[TelemetryFrame]:
    parser = TelemetryParser()
    for line in open_line_source(path):
        frame = parser.parse_line(line)
        if frame is not None:
            yield frame


def main() -> int:
    parser = argparse.ArgumentParser(description="Read NanoFlight telemetry frames from serial or a log file.")
    parser.add_argument("--port", help="Serial port path, for example /dev/ttyACM0")
    parser.add_argument("--baud", type=int, default=115200, help="Serial baud rate")
    parser.add_argument("--file", help="Read telemetry frames from a CSV log file instead of serial")
    parser.add_argument("--limit", type=int, default=0, help="Stop after N parsed frames. Zero means unlimited.")
    args = parser.parse_args()

    if bool(args.port) == bool(args.file):
        parser.error("exactly one of --port or --file is required")

    count = 0
    try:
        if args.file:
            source = parse_file(args.file)
        else:
            source = SerialTelemetryReader(args.port, args.baud).frames()

        for frame in source:
            print(frame)
            count += 1
            if args.limit > 0 and count >= args.limit:
                break
    except (OSError, SerialException, ValueError) as exc:
        print(f"telemetry read failed: {exc}")
        return 1

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
