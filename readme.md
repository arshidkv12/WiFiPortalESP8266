# WiFiPortalESP8266

A lightweight and easy-to-use WiFi configuration portal for ESP8266-based boards. Quickly set up SSID, password, and other device parameters through a simple web interface.


## 🚀 Features

- Captive portal for first-time WiFi setup
- Stores credentials in EEPROM
- Auto-connect to saved WiFi on boot
- Simple web interface to configure SSID and password
- Optional device name configuration
- Easily embeddable in other projects

## 📺 Demo Video

[![Watch the video](https://img.youtube.com/vi/f-tT3yUv6D4/hqdefault.jpg)](https://youtu.be/f-tT3yUv6D4)

👉 Click the image above to watch a quick demo on YouTube.

## 📦 Installation

1. Open Arduino IDE
2. Go to **Sketch > Include Library > Add .ZIP Library**
3. Choose the downloaded `WiFiPortalESP8266` ZIP file
4. Done! The library will now appear under **Examples**

## 🧪 Example

```cpp
#define DEBUG  
#include <WiFiPortalESP8266.h>

WiFiPortalESP8266 portal;


void setup() {
  Serial.begin(9600); 
  portal.setButtonPin(D7);
  portal.setDeviceName("ESP-Setup");
  portal.connectToWiFi();
  
}

void loop() {
  portal.handleWifiButtonPress();
}
```

## 🌐 Web Portal

To configure WiFi credentials using the portal:

1. **Press and hold the WiFi setup button (D7 to GND) for 3 seconds.**
2. On your **mobile phone or laptop**, enable WiFi and connect to the ESP8266 hotspot (e.g., `ESP-Setup`).
3. Open a browser and go to [http://192.168.1.1](http://192.168.1.1).
4. Enter your WiFi **SSID** and **Password**, then click the **Connect** button.
5. The ESP8266 will save your WiFi credentials to EEPROM and automatically attempt to connect to your network.

Next time the device restarts, it will auto-connect using the saved credentials.

## 📸 Screenshots
<div align="center">
  <img src="https://github.com/arshidkv12/WiFiPortalESP8266/blob/main/scr.jpg?raw=true" alt="ESP8266 WIFI" width="500"/>
</div>

## 🙌 Contributions Welcome
Feel free to fork, improve, and submit pull requests.


