
#define DEBUG  

#include <WiFiPortalESP8266.h>

WiFiPortalESP8266 portal;


void setup() {
  Serial.begin(9600); 
  portal.setButtonPin(D7);
  portal.setDeviceName("ABC ESP8266");
  portal.connectToWiFi();
  
}

void loop() {
  portal.handleWifiButtonPress();
}
