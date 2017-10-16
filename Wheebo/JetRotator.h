#ifndef JET_ROTATOR_h
#define JET_ROTATOR_h

class JetRotator {
	public:
		void begin();
		void set(uint8_t deg);
	private:
		const uint8_t ROTATION_CIRCUIT_ADDRESS = 8;
};

#endif
