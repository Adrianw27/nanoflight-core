#include <Wire.h>
#include <cstdint>
#include "config/project_config.h"
#include "hal/imu_driver.h"

namespace hal {

bool imu_init(){
	Wire.begin();

	Wire.beginTransmission(config::imu_i2c_address);
	Wire.write(config::pwr_mgmt_address);
	Wire.write(0x00);

	uint8_t status = Wire.endTransmission();

	delay(100);

	return status == 0;
}

bool imu_read_raw(types::ImuRawSample& out){	
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
	out.ax = to_int16(buf[0],  buf[1]);
	out.ay = to_int16(buf[2],  buf[3]);
	out.az = to_int16(buf[4],  buf[5]);
	out.t = to_int16(buf[6], buf[7]);	
	out.gx = to_int16(buf[8],  buf[9]);
	out.gy = to_int16(buf[10], buf[11]);
	out.gz = to_int16(buf[12], buf[13]);

	return true;
}

bool imu_read_scaled(types::ImuScaledSample& out){
	types::ImuRawSample raw{};
	if (!imuReadRaw(raw)) {
		return false;
	}

	out.ax = static_cast<float>(raw.ax) / config::accel_lsb_per_g;
	out.ay = static_cast<float>(raw.ay) / config::accel_lsb_per_g;
	out.az = static_cast<float>(raw.az) / config::accel_lsb_per_g;

	out.t = static_cast<float>(raw.t) / config::temp_lsb_per_deg + config::temp_offset;

	out.gx = static_cast<float>(raw.gx) / config::gyro_lsb_per_dps;
	out.gy = static_cast<float>(raw.gy) / config::gyro_lsb_per_dps;
	out.gz = static_cast<float>(raw.gz) / config::gyro_lsb_per_dps;

	return true;
}

}

namespace hal {

inline int16_t to_int16(uint8_t hi, uint8_t lo) {
    return static_cast<int16_t>(
        (static_cast<uint16_t>(hi) << 8) | static_cast<uint16_t>(lo)
    );
}

}
