#include "Arduino.h"
#include "Console.h"
#include "ESC.h"
#include "JetRotator.h"

void Console::begin() {
  server = WiFiServer(80);
  server.begin();
}

void Console::setCallback(void (*onConsoleReceive)(String)) {
    _onConsoleReceive = onConsoleReceive;  
}

void Console::check() {
  uint8_t i;
  if (WiFi.status() == WL_CONNECTED) {
    //check if there are any new clients
    if (server.hasClient()){
      for(i = 0; i < MAX_SRV_CLIENTS; i++){
        //find free/disconnected spot
        if (!serverClients[i] || !serverClients[i].connected()){
          if(serverClients[i]) serverClients[i].stop();
          serverClients[i] = server.available();
          if (!serverClients[i]) {
            Serial.println("available broken");
          }
          Serial.print("New client: ");
          Serial.print(i); Serial.print(' ');
          Serial.println(serverClients[i].remoteIP());
          break;
        }
      }
      if (i >= MAX_SRV_CLIENTS) {
        //no free/disconnected spot so reject
        server.available().stop();
      }
    }

    //check clients for data
    for(i = 0; i < MAX_SRV_CLIENTS; i++){
      if (serverClients[i] && serverClients[i].connected()){
        if(serverClients[i].available()){
          while(serverClients[i].available()) {
            char c = serverClients[i].read();
            if(c == '\n') {
              if(_onConsoleReceive != NULL) {
                _onConsoleReceive(str);
              }
              str = "";
            } else {
              str += c;  
            }
          }
        }
      } else {
        serverClients[i].stop();
      }
    }
  }
}
void Console::print(String str) {
  for(int i = 0; i < MAX_SRV_CLIENTS; i++){
    if (serverClients[i] && serverClients[i].connected()){
      char buf[100]; 
      int len = str.length()+1; // 文字列長さ
      str.toCharArray(buf,len);
      serverClients[i].write(buf, len);
      //Serial.write(buf);
    }
  }
}
