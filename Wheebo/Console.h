#ifndef CONSOLE_h
#define CONSOLE_h
#include <WiFi.h>
#include "ESC.h"
#include "JetRotator.h"


class Console {
	public:
		void begin();
    void setCallback(void (*onConsoleReceive)(String));
    void check();
		void print(String str);

	private:
    static const int MAX_SRV_CLIENTS = 1;
    WiFiServer server;
    WiFiClient serverClients[MAX_SRV_CLIENTS];
    String str;
    void (*_onConsoleReceive)(String) = NULL;
};

#endif
