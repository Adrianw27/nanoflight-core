#include <Wire.h>
#include <cstdint>
#include "config/project_config.h"
#include "hal/imu_driver.h"

namespace hal {

bool imuInit(){
	Wire.begin();

	Wire.beginTransmission(config::kImuI2cAddr);
	Wire.write(config::kPwrMgmt1Addr);
	Wire.write(0x00);

	uint8_t status = Wire.endTransmission();

	delay(100);

	return status == 0;
}

bool imuReadRaw(types::ImuRawSample& out){	
	 Wire.beginTransmission(config::kImuI2cAddr);
	 Wire.write(config::kSensorOutputAddr);  
	 if (Wire.endTransmission(false) != 0) {     
		 return false;                          
	 }

    	// 14 bytes: accel(6) + temp(2) + gyro(6)
	constexpr uint8_t kBytesToRead = 14;
	uint8_t bytesRead = Wire.requestFrom(config::kImuI2cAddr, kBytesToRead);
	if (bytesRead < kBytesToRead) {
		return false; 
	}

	// Read all 8 bit registers and store in buffer
	uint8_t buf[kBytesToRead];
	for (uint8_t i = 0; i < kBytesToRead; ++i) { 	
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

bool imuReadScaled(types::ImuScaledSample& out){
	types::ImuRawSample raw{};
	if (!imuReadRaw(raw)) {
		return false;
	}

	out.ax = static_cast<float>(raw.ax) / config::kLsbPerG;
	out.ay = static_cast<float>(raw.ay) / config::kLsbPerG;
	out.az = static_cast<float>(raw.az) / config::kLsbPerG;

	out.t = static_cast<float>(raw.t) / config::kLsbPerC + config::kTempOffset;

	out.gx = static_cast<float>(raw.gx) / config::kLsbPerDps;
	out.gy = static_cast<float>(raw.gy) / config::kLsbPerDps;
	out.gz = static_cast<float>(raw.gz) / config::kLsbPerDps;

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
