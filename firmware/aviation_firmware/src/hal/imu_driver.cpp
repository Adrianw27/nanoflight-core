#include <Wire.h>
#include <cstdint>
#include "config/project_config.h"
#include "hal/imu_driver.h"

using std::uint8_t;
using std::uint16_t;
using std::int16_t;
using types::RawAccelSample;
using types::RawGyroSample;
using types::ScaledAccelSample;
using types::ScaledGyroSample;

namespace hal {

inline int16_t to_int16(uint8_t hi, uint8_t lo) {
    return static_cast<int16_t>(
        (static_cast<uint16_t>(hi) << 8) | static_cast<uint16_t>(lo)
    );
}

bool imu_init(){
	Wire.begin();

	Wire.beginTransmission(config::imu_i2c_address);
	Wire.write(config::pwr_mgmt_address);
	Wire.write(0x00);

	uint8_t status = Wire.endTransmission();

	delay(100);

	return status == 0;
}

bool imu_read_raw(types::RawAccelSample& accel, types::RawGyroSample& gyro, int16_t& temp){	
	 Wire.beginTransmission(config::imu_i2c_address);
	 Wire.write(config::accel_output_address);  
	 if (Wire.endTransmission(false) != 0) {     
		 return false;                          
	 }

    	// 14 bytes: accel(6) + temp(2) + gyro(6)
	constexpr uint8_t num_bytes = 14;
	uint8_t bytes_read = Wire.requestFrom(config::imu_i2c_address, num_bytes);
	if (bytes_read < num_bytes) {
		return false; 
	}

	// Read all 8 bit registers and store in buffer
	uint8_t buf[num_bytes];
	for (uint8_t i = 0; i < num_bytes; ++i) { 	
		buf[i] = Wire.read();
	}

	// Convert individual bytes to 16 bit int
	accel.ax = to_int16(buf[0],  buf[1]);
	accel.ay = to_int16(buf[2],  buf[3]);
	accel.az = to_int16(buf[4],  buf[5]);
	
	temp = to_int16(buf[6], buf[7]);

	gyro.gx = to_int16(buf[8],  buf[9]);
	gyro.gy = to_int16(buf[10], buf[11]);
	gyro.gz = to_int16(buf[12], buf[13]);
	return true;
}

bool imu_read_scaled(types::ScaledAccelSample& accel, types::ScaledGyroSample& gyro, float& temp_cel){
	types::RawAccelSample raw_accel{};
	types::RawGyroSample raw_gyro{};
	int16_t raw_temp;
	if (!imu_read_raw(raw_accel, raw_gyro, raw_temp)) {
		return false;
	}

	accel.ax_g = static_cast<float>(raw_accel.ax) / config::accel_lsb_per_g;
	accel.ay_g = static_cast<float>(raw_accel.ay) / config::accel_lsb_per_g;
	accel.az_g = static_cast<float>(raw_accel.az) / config::accel_lsb_per_g;

	temp_cel = static_cast<float>(raw_temp) / config::temp_lsb_per_deg + config::temp_offset;

	gyro.gx_dps = static_cast<float>(raw_gyro.gx) / config::gyro_lsb_per_dps;
	gyro.gy_dps = static_cast<float>(raw_gyro.gy) / config::gyro_lsb_per_dps;
	gyro.gz_dps = static_cast<float>(raw_gyro.gz) / config::gyro_lsb_per_dps;

	return true;
}

}
