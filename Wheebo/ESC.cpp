#include "Arduino.h"
#include "ESC.h"

void ESC::begin() {
/* Pin setup */
	pinMode(ESC_PIN, OUTPUT);

/* Timer setup */
	ledcSetup(LEDC_CHANNEL_0, LEDC_SERVO_FREQ, LEDC_TIMER_16_BIT);
	ledcAttachPin(ESC_PIN, LEDC_CHANNEL_0);
	ledcWrite(LEDC_CHANNEL_0, servo_pwm_count(0));
	delay(3000);  // Wait for initialize
}



void ESC::set(uint8_t speed) {
	Serial.println("Set motor speed = " + String(speed));
	ledcWrite(LEDC_CHANNEL_0, servo_pwm_count(speed));
}

void ESC::stop() {
	set(0);
}

int ESC::servo_pwm_count(int v) {
  float vv = (v + 90) / 180.0 ;
  return (int)(65536 * (SERVO_MIN_WIDTH_MS + vv * (SERVO_MAX_WIDTH_MS - SERVO_MIN_WIDTH_MS)) / 20.0) ;
}
