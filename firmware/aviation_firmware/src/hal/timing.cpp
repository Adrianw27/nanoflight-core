#include "hal/timing.h"
#include <Arduino.h>
#include <cstdint>

using std::uint32_t;

namespace hal {

uint32_t get_current_micros(){
	return micros();
}

float compute_delta_seconds(uint32_t& last_micros){
	uint32_t current_micros = micros();
	uint32_t delta = current_micros - last_micros;  
	last_micros = current_micros;
	
	return static_cast<float>(delta) / 1'000'000.0f;
}

}
	
