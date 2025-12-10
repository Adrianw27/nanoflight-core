#include "fusion/altitude_math.h"
#include <cmath>

namespace fusion {

bool get_angles_from_accel(types::ScaledAccelSample& accel, types::AccelTiltAngles angles){
	float x = accel.ax_s;
	float y = accel.ay_s;
	float z = accel.az;

	float denom = sqrt(y * y + z * z);
	if (denom == 0.0f) {
        	return false;
    	}

	angles.roll = atan2(y , z) * 57.3;
	angles.pitch = atan2((- x) , denom) * 57.3;

	return true;
	}

}
