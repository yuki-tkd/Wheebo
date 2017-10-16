#ifndef PREFERENCE_h
#define PREFERENCE_h

class Preference {
	public:
		void begin();
		void setRotationOffset(uint8_t offset);
		uint8_t readRotationOffset();
	private:
		uint16_t EEPROM_SIZE = 64;
		uint16_t addr = 0;
};

#endif
