/*
    Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleWrite.cpp
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
#include <M5Atom.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <elapsedMillis.h>

// timer to blink LED if needed
elapsedMillis timeElapsed;
unsigned int interval = 40;
char ch;

// --- BLE setup ----//
BLECharacteristic* pCharacteristic = NULL;

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/
//
#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

class BTCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) 
    {std::string value = pCharacteristic->getValue();
      M5.dis.drawpix(0, 0x0000ff); //LED on
      if (value.length() > 0) {
        for (int i = 0; i < value.length(); i++)
          Serial2.print(value[i]);
      }
      //Serial2.println(); // if you want return new line
    }
};
