#ifndef WIFI_PORTAL_ESP8266_H
#define WIFI_PORTAL_ESP8266_H

#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>
#include <FS.h>

#ifdef DEBUG
#define log(x) Serial.print(x);
#define logln(x) Serial.println(x);
#else
#define log(x) 
#define logln(x) 
#endif

class WiFiPortalESP8266 {
public:
  WiFiPortalESP8266();
  void setupServer();
  void run();
  void stopServer();
  void connectToWiFi();
  static String readStringFromEEPROM(int address, int length);
  static void saveStringToEEPROM(int address, const String& data);
  void setButtonPin(uint8_t pin, unsigned long duration = 3000);
  void setDeviceName(String name);
  bool handleWifiButtonPress(); 
  bool WifiConnected = false;

private:
  void handleRoot();
  DNSServer dnsServer;
  ESP8266WebServer webServer;
  IPAddress apIP;
  bool serverOn;
  uint8_t buttonPin = 255;
  String deviceName = "ESP8266";
  unsigned long pressStartTime = 0;
  unsigned long longPressDuration = 3000;  
  bool buttonPressed = false;
};

#endif
