/* Test to turn on M% stick-c LED on ESP32 from Alexa
 
Open Monitor terminal in Arduino (115200) to see messages
Tell Alexa to "dscover devices" or use app to add'other device'.
when ESP32 is running "Alexa turn on(off) lamp one" is command
Turns on/off LED on ESP32
*/
//#include <Arduino.h>
#include <M5Atom.h>  
#include <WiFi.h>
#include "fauxmoESP.h"

#define SERIAL_BAUDRATE 115200
#define WIFI_SSID "YOUR WIFI NAME"
#define WIFI_PASS "YOUR WIFI PASSWORD"
#define LAMP_1 "lamp one"

fauxmoESP fauxmo;

// ------------------------Wi-Fi Connection-------------------------------------
void wifiSetup() {
  // Set WIFI module to STA mode
  WiFi.mode(WIFI_STA);

  // Connect
  Serial.printf("[WIFI] Connecting to %s ", WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  // Wait
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(100);
  }
  Serial.println();

  // Connected!
  Serial.printf("[WIFI] STATION Mode, SSID: %s, IP address: %s\n", WiFi.SSID().c_str(), WiFi.localIP().toString().c_str());
}
//-----------------------------------------------------------------------------------
