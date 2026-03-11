# Telemetry Data Format

Telemetry is emitted over serial as newline-delimited CSV frames.

## Frame Types

Sample frames:

```text
DATA,time_ms,ax_g,ay_g,az_g,gx_dps,gy_dps,gz_dps,temp_cel,accel_pitch_deg,accel_roll_deg,complementary_pitch_deg,complementary_roll_deg,kalman_pitch_deg,kalman_roll_deg
```

Health frames:

```text
HEALTH,uptime_ms,mode,status_flags
```

At telemetry startup, the firmware emits two schema rows so the ground station can confirm field order:

```text
SCHEMA,DATA,time_ms,ax_g,ay_g,az_g,gx_dps,gy_dps,gz_dps,temp_cel,accel_pitch_deg,accel_roll_deg,complementary_pitch_deg,complementary_roll_deg,kalman_pitch_deg,kalman_roll_deg
SCHEMA,HEALTH,uptime_ms,mode,status_flags
```

## Parsing Notes

- The first CSV token is always the frame type.
- `DATA` frames contain one timestamped sensor-fusion sample.
- `HEALTH` frames contain lower-rate status information.
- All numeric fields are decimal ASCII values.
- `mode` is the numeric firmware mode enum.
- `status_flags` is the firmware status bitfield.
