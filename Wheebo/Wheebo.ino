#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#define SIZE_OF_ARRAY(ary)  (sizeof(ary)/sizeof((ary)[0]))

#define BNO055_SAMPLERATE_DELAY_MS (100)
Adafruit_BNO055 bno = Adafruit_BNO055(55);


#define LEDC_CHANNEL_3 0
#define LEDC_TIMER_16_BIT 16
#define LEDC_SERVO_FREQ 50
#define SERVO_MIN_WIDTH_MS 0.6
#define SERVO_MAX_WIDTH_MS 1.6


const char* ssid = "Rockhopper";
const char* password = "abcdefgf";

//ESC
const uint8_t ESC_PIN = 25;
const uint8_t CHANNEL_0 = 0;
const uint8_t LEDC_TIMER_BIT = 8;   // Resolution: 8bit
const uint8_t LEDC_BASE_FREQ = 50;  // Hz
const uint8_t VALUE_MAX = 255;      // Max value of PWM
const uint8_t MOTOR_STOP = 189;      // TODO: Should be tested
const uint8_t MOTOR_MAX = 255;      // TODO: Should be tested
uint8_t esc_speed = MOTOR_STOP;

//Jet rotation circuit
const uint8_t ROTATION_CIRCUIT_ADDRESS = 8;

WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  Serial.println("Booting");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }
  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH)
      type = "sketch";
    else // U_SPIFFS
      type = "filesystem";

    // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
    Serial.println("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());


/* Starting the console*/
  server.begin();


/* Pin setup */
  pinMode(ESC_PIN, OUTPUT);

/* Timer setup */
  //ledcSetup(CHANNEL_0, 50, LEDC_TIMER_BIT);
  //ledcAttachPin(ESC_PIN, CHANNEL_0);
  //ledcWrite(CHANNEL_0, MOTOR_STOP);
  delay(3000);  // Wait for initialize

  ledcSetup(LEDC_CHANNEL_3, LEDC_SERVO_FREQ, LEDC_TIMER_16_BIT);
  ledcAttachPin(ESC_PIN, LEDC_CHANNEL_3);
  ledcWrite(LEDC_CHANNEL_3, servo_pwm_count(0));
  delay(3000);  // Wait for initialize

/* I2C setup */
  Wire.begin();

  Serial.println("System started");
}


WiFiClient client;


int servo_pwm_count(int v)
{
  float vv = (v + 90) / 180.0 ;
  return (int)(65536 * (SERVO_MIN_WIDTH_MS + vv * (SERVO_MAX_WIDTH_MS - SERVO_MIN_WIDTH_MS)) / 20.0) ;
}

void loop() {
  //ArduinoOTA.handle();
  client = server.available();
  serverRead(client);
  //delay(5000);
  //esc(50);
  //delay(5000);
  //esc(0);
}

void esc(uint8_t s) {
  //uint8_t next_speed = map(s, 0, 100, MOTOR_STOP, MOTOR_MAX);
  //if(next_speed != esc_speed) {
    Serial.println("Set motor speed = " + String(s));
    serverSend(client, "Set motor speed = " + String(s));
    //ledcWrite(CHANNEL_0, s);
    ledcWrite(LEDC_CHANNEL_3, servo_pwm_count(s));
    //esc_speed = next_speed;
  //}
}

void rotate(byte b) {
  Serial.println("Set the direction to  = " + String(b));
  serverSend(client, "Set the direction to  = " + String(b));
  Wire.beginTransmission(ROTATION_CIRCUIT_ADDRESS);
  Wire.write(b);
  Wire.endTransmission();
}

String str;
void serverRead(WiFiClient client) {
  if(client) {
    while(client.connected()) {
      if(client.available()) {
        char c = client.read();
        if(c == '\n') {
          Serial.println(str);
          serverSend(client, str);
          int power = str.toInt();
          esc(power);
          //byte b = (byte)power;
          //rotate(b);
          str = "";
          break;
        } else {
          str += c; 
        }
      }
    }
  }
}

void serverSend(WiFiClient client, String str) {  
  client.println(str);
}

