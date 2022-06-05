#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <elapsedMillis.h>
#include <M5Atom.h>

// timer to blink LED if needed
elapsedMillis timeElapsed;
unsigned int interval = 40;

//serial ports
#define serialUART Serial2 // hardware serial port to/from BLE chip
#define serialUSB Serial // USB serial  to/from mac

BLEServer *pServer = NULL;
BLECharacteristic * pTxCharacteristic;
bool deviceConnected = false;
bool oldDeviceConnected = false;
char ch;

String buf;
bool fullPkt;

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

#define SERVICE_UUID           "6E400001-B5A3-F393-E0A9-E50E24DCCA9E" // UART service UUID
#define CHARACTERISTIC_UUID_RX "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_TX "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"


class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
    };

    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
    }
};

// Incoming BLE data
class MyCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
      std::string rxValue = pCharacteristic->getValue();
      M5.dis.drawpix(0, 0x0000ff); //LED on
      if (rxValue.length() > 0) {
        for (int i = 0; i < rxValue.length(); i++)
        {
          serialUSB.print(rxValue[i]); // BLE --> USB serial
          serialUART.print(rxValue[i]); // BLE --> uart(teensy)
        }
      }
    }
};
