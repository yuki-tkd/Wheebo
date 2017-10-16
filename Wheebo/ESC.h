#ifndef ESC_h
#define ESC_h

class ESC {
	public:
		void begin();
		void set(uint8_t speed);
		void stop();
		uint8_t speed();
	private:
		uint8_t LEDC_CHANNEL_0 = 0;	//Use Timer0
		uint8_t LEDC_SERVO_FREQ = 50;	//50Hz = 20ms
		uint8_t LEDC_TIMER_16_BIT = 16;	//Resolution: 16bit
		uint8_t ESC_PIN = 25;

		float SERVO_MIN_WIDTH_MS = 0.6;
		float SERVO_MAX_WIDTH_MS = 1.6;

		int servo_pwm_count(int v);
};

#endif
