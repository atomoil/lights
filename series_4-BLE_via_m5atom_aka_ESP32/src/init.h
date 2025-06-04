/*
    Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLETests/SampleWrite.cpp
    Ported to Arduino ESP32 by Evandro Copercini
    
    When flashing the M5Atom:
    Select M5stick-c from board menu.
    Upload speed 921600
    Partitioning scheme: no OTA
    Port /dev/cu.usbserial-xxxxxxxx
    
    I used BLE terminal app on ipad to test.
    SERIAL MONITOR  - SET TO 57600

    Data from wireless (BLE receive) Handled by callback
    which then sends it to teensy via Serial2.
*/

/*
base on BLE_Server_multiconnect
*/
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <M5Atom.h>
#include <BLE2902.h>
#include <elapsedMillis.h>

// timer to blink LED if needed
elapsedMillis timeElapsed;
unsigned int interval = 40;
char ch;

String buf;
bool fullPkt;

BLEServer* pServer = NULL;
BLECharacteristic* pCharacteristic = NULL;
bool deviceConnected = false;
bool oldDeviceConnected = false;

//serial ports
#define serialTeensy Serial2 // hardware serial port to/from BLE chip
#define serialUSB Serial // USB serial  to/from mac


// See the following for generating UUIDs:
// https://www.uuidgenerator.net/
#define SERVICE_UUID        "F1A0EB1D-9F0D-4EE5-8271-08AB90716F53"
#define CHARACTERISTIC_UUID "2FB035BE-2E57-4616-A4ED-55EB6F036E30"
// #define SERVICE_UUID           "6E400001-B5A3-F393-E0A9-E50E24DCCA9E" // UART service UUID
// #define CHARACTERISTIC_UUID_RX "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_TX "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"

// Callbacks for BLE 
class serverCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer *pServer) {
      deviceConnected = true;
      BLEDevice::startAdvertising();
    };

    void onDisconnect(BLEServer *pServer) {
      deviceConnected = false;
    }
};

// data from BLE ESP serial -> Teensy
class BTCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
      std::string value = pCharacteristic->getValue();
      M5.dis.drawpix(0, 0x0000ff); //LED on
      if (value.length() > 0) {
        for (int i = 0; i < value.length(); i++)
        {
          Serial2.print(value[i]);
          Serial.print(value[i]);
        }
      }
      Serial.println(); // if you want return new line
    }
};
