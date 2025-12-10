#include "fusion/altitude_math.h"
#include <cmath>

namespace fusion {

bool get_angles_from_accel(types::ScaledAccelSample& accel, types::AccelTiltAngles angles){
	accel.ax = x;
	accel.ay = y;
	accel.az = z;

	float denom = sqrt(y * y + z * z);
	if (denom == 0.0f) {
        	return false;
    	}

	angles.roll = atan2(y , z) * 57.3;
	angles.pitch = atan2((- x) , denom) * 57.3;

	return true;
	}

}
