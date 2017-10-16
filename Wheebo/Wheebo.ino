#include <ArduinoJson.h>
#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

#include "ESC.h"
#include "Preferences.h"
#include "JetRotator.h"
#include "Console.h"

#define SIZE_OF_ARRAY(ary)  (sizeof(ary)/sizeof((ary)[0]))
#define MATH_PI 3.14

#define LED_R 26
#define LED_G 27
#define LED_B 32
#define PUMP_PIN 14


#define BNO055_SAMPLERATE_DELAY_MS (100)
Adafruit_BNO055 bno = Adafruit_BNO055();

//const char* ssid = "Rockhopper";
//const char* password = "abcdefgf";

//const char* ssid =  "aterm-d97c15-g";
//const char* password = "36bd0469b2900";

const char* ssid = "W04_5001D9B04ECE";
const char* password = "0y4tmqt2qmqyq3d";

ESC esc;
JetRotator jet_rotator;
Console console;

hw_timer_t* timer = NULL;

void onConsoleCallback(String message);

void IRAM_ATTR onTimer(){
//  sensors_event_t event;
//  bno.getEvent(&event);

//  uint8_t sys, gyro, accel, mag = 0;
//  bno.getCalibration(&sys, &gyro, &accel, &mag);
//
//  StaticJsonBuffer<1024> jsonBuffer;
//  JsonObject& root = jsonBuffer.createObject();
//  JsonArray& orientation = root.createNestedArray("data");
//  orientation.add((float)event.orientation.x);
//  orientation.add((float)event.orientation.y);
//  orientation.add((float)event.orientation.z);
//  JsonArray& calibration = root.createNestedArray("calibration");
//  calibration.add(sys);
//  calibration.add(gyro);
//  calibration.add(accel);
//  calibration.add(mag);

//  float x = event.orientation.x;
//  float y = event.orientation.y;
//  float z = event.orientation.z;
//
//  float theta;
//  if(y < 0 && z> 0) { //第一象限 (-, +)
//    theta = MATH_PI / 2 * abs(z) / (abs(y) + abs(z));
//  } else if(y > 0 && z > 0) { //第二象限 (+, +)
//    theta = MATH_PI / 2 * (1 + abs(y) / (abs(y) + abs(z)));
//  } else if(y > 0 && z < 0) {
//    theta = MATH_PI / 2 * (2 + abs(z) / (abs(y) + abs(z)));
//  } else if(y < 0 && z < 0) {
//    theta = MATH_PI / 2 * (3 + abs(y) / (abs(y) + abs(z)));
//  }
//
//  Serial.println("x=" + String(x) + "y=" + String(y) + "z=" + String(z) + "theta=" + String(theta));
//
//  
//  root.set("Theta", theta);
//  root.set("x", x);
//
//  String str;
//  root.printTo(str);
//  str += "\n";
  //console.print(String(theta) + "\n");
}

void setup() {
  /* Setup IO Pins */
  pinMode(PUMP_PIN, OUTPUT);
  
  pinMode(LED_R, OUTPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(LED_B, OUTPUT);



  Serial.begin(115200);
  Serial.println("Booting");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }

  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  
/* Starting the console*/
  console.begin();
  console.setCallback(onConsoleCallback);
  
/* Setup ESC */
  esc.begin();

/* I2C setup */
  jet_rotator.begin();


/* Setup Timer for 9DoF sensor */
  timer = timerBegin(1, 80, true);
  timerAttachInterrupt(timer, &onTimer, true);
  timerAlarmWrite(timer, 1000 * BNO055_SAMPLERATE_DELAY_MS, true); //Run in 100msec
  timerAlarmEnable(timer);

  Serial.println("System started");


  digitalWrite(LED_B, HIGH);
}

void loop() {
  console.check();
}

void onConsoleCallback(String message) {
  char json[1024];
  int len = message.length() + 1;
  message.toCharArray(json, len);
  StaticJsonBuffer<1024> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(json);

  if(root.containsKey("esc")){
    int power = root["esc"].as<int>();
    esc.set(power);
    Serial.print("Set esc power = " + String(power));
  } else if(root.containsKey("jet")) {
    int deg = root["jet"].as<int>();  
    jet_rotator.set(deg);
    Serial.print("Set jet deg = " + String(deg));
  } else if(root.containsKey("pump")) {
    pump();  
  }
}

void pump() {
  digitalWrite(PUMP_PIN, !digitalRead(14));
}


