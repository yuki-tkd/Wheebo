#include "Arduino.h"
#include "Preference.h"
#include "EEPROM.h"

void Preference::begin() {
	if(!EEPROM.begin(EEPROM_SIZE)) {
		Serial.println("Failed to initialize EEPROM");
	}
}

void Preference::setRotationOffset(uint8_t offset) {
	EEPROM.write(0, offset);
	EEPROM.commit();
}

uint8_t Preference::readRotationOffset() {
	return EEPROM.read(0);
}
