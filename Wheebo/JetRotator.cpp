#include "Arduino.h"
#include "JetRotator.h"
#include "Wire.h"

void JetRotator::begin() {
	Wire.begin();
}


void JetRotator::set(uint8_t deg) {
	Serial.println("Set the direction to  = " + String(deg));
	Wire.beginTransmission(ROTATION_CIRCUIT_ADDRESS);
	Wire.write(deg);
	Wire.endTransmission();
}
