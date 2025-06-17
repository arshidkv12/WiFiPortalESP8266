#include "WiFiPortalESP8266.h"

WiFiPortalESP8266::WiFiPortalESP8266()
  : webServer(80), apIP(192, 168, 1, 1), serverOn(false) {
    EEPROM.begin(80);
  }

void WiFiPortalESP8266::setupServer() {
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP( deviceName );

  if (!SPIFFS.begin()) return;

  dnsServer.setTTL(300);
  dnsServer.setErrorReplyCode(DNSReplyCode::ServerFailure);
  dnsServer.start(53, "*", apIP);

  webServer.on("/", std::bind(&WiFiPortalESP8266::handleRoot, this));
  webServer.onNotFound(std::bind(&WiFiPortalESP8266::handleRoot, this));
  webServer.begin();

  serverOn = true;
}

void WiFiPortalESP8266::run() {
  if (serverOn) {
    dnsServer.processNextRequest();
    webServer.handleClient();
  }
}

void WiFiPortalESP8266::stopServer() {
  if (serverOn) {
    dnsServer.stop();
    webServer.stop();
    WiFi.mode(WIFI_OFF);
    serverOn = false;
  }
}

void WiFiPortalESP8266::connectToWiFi() {

  String ssid = readStringFromEEPROM(0, 32);
  String pass = readStringFromEEPROM(32, 64);

  WiFi.begin(ssid.c_str(), pass.c_str());

  log("Connecting to WiFi");
  unsigned long start = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - start < 15000) {
    delay(500);
    log(".");
    handleWifiButtonPress();
  }

  if (WiFi.status() == WL_CONNECTED) {
    WifiConnected = true;
    logln("\nConnected! IP: " + WiFi.localIP().toString());
  } else {
    WifiConnected = false;
    logln("\nFailed to connect.");
  }
}

void WiFiPortalESP8266::handleRoot() {
  String message = R"rawliteral(
    <!DOCTYPE html>
      <html>
      <head>
        <meta name="viewport" content="width=device-width, initial-scale=1">
        <title>WiFi Setup</title>
        <style>
          body {
            font-family: Arial, sans-serif;
            background: #f4f4f4;
            margin: 0;
            padding: 0;
            display: flex;
            justify-content: center;
            align-items: center;
            height: 100vh;
          }
          .container {
            background: #fff;
            margin: 7px;
            padding:15px;
            border-radius: 10px;
            box-shadow: 0 4px 8px rgba(0,0,0,0.2);
            width: 100%;
            max-width: 400px;
          }
          h2 {
            text-align: center;
            color: #333;
          }
          input[type="text"]{
            width: 100%;
            padding: 12px;
            margin: 8px 0;
            border: 1px solid #ccc;
            border-radius: 5px;
            box-sizing: border-box; 
          }
          input[type="submit"] {
            width: 100%;
            background-color: #4CAF50;
            color: white;
            padding: 14px;
            margin-top: 10px;
            border: none;
            border-radius: 5px;
            cursor: pointer;
            font-size: 16px;
          }
          input[type="submit"]:hover {
            background-color: #45a049;
          }
        </style>
      </head>
      <body>
        <div class="container">
          <h2>WiFi Configuration</h2>
          <form action="/" method="POST">
            <label for="ssid">SSID</label>
            <input type="text" id="ssid" name="ssid" value="{ssid}" placeholder="Enter WiFi SSID" required>

            <label for="pass">Password</label>
            <input type="text" id="pass" value="{pass}" name="pass" placeholder="Enter WiFi Password">

            <input type="submit" value="Connect">
            <p style="color:green;">{msg}</p>
          </form>
        </div>
      </body>
      </html>
  )rawliteral";

  String ssid = "";
  String pass = "";

  if (webServer.method() == HTTP_POST) {
    ssid = webServer.arg("ssid");
    pass = webServer.arg("pass");
    message.replace("{ssid}", ssid);
    message.replace("{pass}", pass);
    message.replace("{msg}", "Successfully Saved!");
  }

  if (webServer.method() != HTTP_POST) {
    message.replace("{ssid}", "");
    message.replace("{pass}", "");
    message.replace("{msg}", "");
  }



  webServer.send(200, "text/html", message);
  
  if (webServer.method() == HTTP_POST) {

    saveStringToEEPROM(0, ssid);
    saveStringToEEPROM(32, pass);
    EEPROM.commit();

    stopServer();
    connectToWiFi();
  }

}


void WiFiPortalESP8266::setButtonPin(uint8_t pin, unsigned long duration) {
  buttonPin = pin;
  longPressDuration = duration;
  pinMode(buttonPin, INPUT_PULLUP);
}

void WiFiPortalESP8266::setDeviceName(String name){
  deviceName = name;
}



bool WiFiPortalESP8266::handleWifiButtonPress() {
  if (buttonPin == 255) return false; 

  bool state = digitalRead(buttonPin) == LOW;  

  if (state && !buttonPressed) {
    pressStartTime = millis();
    buttonPressed = true;
  }

  if (!state && buttonPressed) {
    buttonPressed = false;
  }

  if (state && buttonPressed && (millis() - pressStartTime >= longPressDuration)) {
    
    logln("[WiFiPortalESP8266] Long press detected!");
    
    buttonPressed = false;
    setupServer();
    while(1){
      run();
      delay(100);
      if(!serverOn){
        break;
      }
    }
    
    return true;
  }
  return false;
}


void WiFiPortalESP8266::saveStringToEEPROM(int address, const String& data) {
  for (unsigned int i = 0; i < data.length(); i++) {
    EEPROM.write(address + i, data[i]);
  }
  EEPROM.write(address + data.length(), '\0');
}

String WiFiPortalESP8266::readStringFromEEPROM(int address, int length) {
  String data = "";
  for (int i = 0; i < length; i++) {
    char c = EEPROM.read(address + i);
    if (c == '\0' || c == 0xFF) break;
    data += c;
  }
  return data;
}
