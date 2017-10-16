import controlP5.*;
import processing.net.*;

ControlP5 p5;
Client client;
String wheeboAddress = "192.168.100.115";

void setup() {
  size(640, 480);
  frameRate(60);
  client = new Client(this, wheeboAddress, 80);
  
 p5 = new ControlP5(this);
 controlP5.Group console_group = p5.addGroup("console_group")
              .disableCollapse()
              .setPosition(0, height-80)
              .setLabel("Console")
              .setBackgroundHeight(200);
        
  p5.addTextfield("input")
     .setGroup(console_group)
     .setPosition(0,0)
     .setSize(width-110, 20)
     .setFocus(true)
     .setColor(color(255));
     
  p5.addButton("connectButton")
    .setGroup(console_group)
    .setLabel("Send")
    .setPosition(width-100, 0)
    .setSize(100, 20);
    
  p5.addTextarea("console")
    .setGroup(console_group)
    .setPosition(0, 20)
    .setSize(width,100)
    .setFont(createFont("arial",12))
    .setLineHeight(14)
    .setColor(color(255))
    .setColorBackground(color(0))
    .setColorForeground(color(0))
    .setText("");

//Meters
  controlP5.Group motor_group = p5.addGroup("motor_group")
              .disableCollapse()
              .setPosition(0, 10)
              .setLabel("Motor Control")
              .setBackgroundHeight(200);
  
  p5.addKnob("power")
    .setPosition(10, 20)
    .setRange(0, 180)
    .setValue(0)
    .setRadius(50)
    .setLabel("Power")
    .setDragDirection(Knob.VERTICAL)
    .addCallback(new CallbackListener() {
      public void controlEvent(CallbackEvent event) {
        if(event.getAction() == ControlP5.ACTION_RELEASE) {
          float val = event.getController().getValue();
          int i = int(val);
          JSONObject json = new JSONObject();
          json.setInt("esc", i);
          client.write(json.toString() + "\n");
        }
      }
    });
    
  p5.addToggle("Override")
    .setLabel("Override")
    .setGroup(motor_group)
    .setPosition(50, 130)
    .setValue(false)
    .setMode(ControlP5.SWITCH)
    .setSize(20, 20);
  
  p5.addKnob("battery")
    .setPosition(140, 20)
    .setRange(0, 20)
    .setValue(20)
    .setRadius(50)
    .setLabel("Battery");    

  p5.addKnob("jet")
    .setPosition(250, 20)
    .setRange(0, 255)
    .setValue(0)
    .setRadius(50)
    .setLabel("Jet")
    .setDragDirection(Knob.VERTICAL)
    .addCallback(new CallbackListener() {
      public void controlEvent(CallbackEvent event) {
         if(event.getAction() == ControlP5.ACTION_RELEASE) {
          float val = event.getController().getValue();
          int i = int(val);
          JSONObject json = new JSONObject();
          json.setInt("jet", i);
          client.write(json.toString() + "\n");
         }
      }
    });
    
    
//Status Group
 controlP5.Group status_group = p5.addGroup("status")
              .disableCollapse()
              .setPosition(0, 220)
              .setLabel("Status")
              .setBackgroundHeight(200);

//Status
  p5.addTextlabel("label_status")
    .setGroup(status_group)
    .setPosition(0, 0)
    .setText("Standby")
    .setFont(createFont("Helvetica",12)); 

  p5.addTextlabel("label_pump_status")
    .setGroup(status_group)
    .setPosition(0, 20)
    .setText("Pump: Running")
    .setFont(createFont("Helvetica",12)); 
 
  p5.addTextlabel("label_kill_switch_status")
    .setGroup(status_group)
    .setPosition(0, 40)
    .setText("Kill switch: Activated")
    .setFont(createFont("Helvetica",12)); 
    
//Sensor Group
  controlP5.Group sensor_group = p5.addGroup("sensor_group")
                .disableCollapse()
                .setPosition(400, 10)
                .setLabel("Sensor")
                .setBackgroundHeight(200);

//Accelerometer
  p5.addTextlabel("label_accel")
    .setGroup(sensor_group)
    .setPosition(0, 0)
    .setText("Accelerometer") 
    .setFont(createFont("Helvetica",12)); 

   p5.addSlider("slider_accel_x")
     .setGroup(sensor_group)
     .setLabel("X")
     .setPosition(0,20)
     .setSize(100,10)
     .setRange(0,100)
     .setValue(40);
   p5.addSlider("slider_accel_y")
     .setGroup(sensor_group)
     .setLabel("Y")
     .setPosition(0,40)
     .setSize(100,10)
     .setRange(0,100)
     .setValue(40);    
   p5.addSlider("slider_accel_z")
     .setGroup(sensor_group)
     .setLabel("Z")
     .setPosition(0,60)
     .setSize(100,10)
     .setRange(0,100)
     .setValue(40);

 //Geomagnetic 
    p5.addTextlabel("label_geomag")
    .setGroup(sensor_group)
    .setPosition(0, 80)
    .setText("Geomagnetic") 
    .setFont(createFont("Helvetica",12)); 

   p5.addSlider("slider_geomag_x")
     .setGroup(sensor_group)
     .setLabel("X")
     .setPosition(0, 100)
     .setSize(100,10)
     .setRange(0,100)
     .setValue(40);
   p5.addSlider("slider_geomag_y")
     .setGroup(sensor_group)
     .setLabel("Y")
     .setPosition(0,120)
     .setSize(100,10)
     .setRange(0,100)
     .setValue(40);    
   p5.addSlider("slider_geomag_z")
     .setGroup(sensor_group)
     .setLabel("Z")
     .setPosition(0,140)
     .setSize(100,10)
     .setRange(0,100)
     .setValue(40);

  //Gyro 
  p5.addTextlabel("label_gyro")
    .setGroup(sensor_group)
    .setPosition(0, 160)
    .setText("Gyro") 
    .setFont(createFont("Helvetica",12)); 

   p5.addSlider("slider_gyro_x")
     .setGroup(sensor_group)
     .setLabel("X")
     .setPosition(0,180)
     .setSize(100,10)
     .setRange(0,100)
     .setValue(40);
   p5.addSlider("slider_gyro_y")
     .setGroup(sensor_group)
     .setLabel("Y")
     .setPosition(0,200)
     .setSize(100,10)
     .setRange(0,100)
     .setValue(40);    
   p5.addSlider("slider_gyro_z")
     .setGroup(sensor_group)
     .setLabel("Z")
     .setPosition(0,220)
     .setSize(100,10)
     .setRange(0,100)
     .setValue(40);
}

void draw() {
  background(50);
  readConsole();
}

void keyReleased() {
  if (key == 'a') {
    client.write("60\n");
    println("a");
  } else if (key == 'b') {
    JSONObject json = new JSONObject();
    json.setInt("pump", 1);
    client.write(json.toString() + "\n");
  } 
}

String str = "";
void readConsole() {
  while (client.available() > 0) {
    char c = (char)client.read();
    println(c);
  }
}