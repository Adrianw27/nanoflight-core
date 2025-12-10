#include "hal/timing.h"
#include <Arduino.h>
#include <cstdint>

using std::uint32_t;

namespace hal {

uint32_t get_current_micros(){
	return micros();
}

double compute_delta_seconds(uint32_t& last_micros){
	uint32_t current_micros = micros();
	uint32_t delta = current_micros - last_micros;  
	last_micros = current_micros;
	
	return static_cast<double>(delta) / 1000000.0;
}

}
	
