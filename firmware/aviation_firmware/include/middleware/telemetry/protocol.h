#ifndef MIDDLEWARE_TELEMETRY_PROTOCOL_H
#define MIDDLEWARE_TELEMETRY_PROTOCOL_H

namespace comms {
namespace protocol {

constexpr char kSchemaFrameType[] = "SCHEMA";
constexpr char kDataFrameType[] = "DATA";
constexpr char kHealthFrameType[] = "HEALTH";

constexpr char kDataSchema[] =
	"SCHEMA,DATA,time_ms,ax_g,ay_g,az_g,gx_dps,gy_dps,gz_dps,temp_cel,"
	"accel_pitch_deg,accel_roll_deg,complementary_pitch_deg,complementary_roll_deg,"
	"kalman_pitch_deg,kalman_roll_deg";

constexpr char kHealthSchema[] = "SCHEMA,HEALTH,uptime_ms,mode,status_flags";

}
}

#endif
