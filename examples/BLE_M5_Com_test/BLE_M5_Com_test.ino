#include "init.h"

void setup() {
  buf.reserve(128);
  M5.begin(true, false, true);
  serialUSB.begin(57600);
  serialUART.begin(57600, SERIAL_8N1, 22, 19); //serial to teensy

  // Create the BLE Device
  BLEDevice::init("LAMP-07-99");

  // Create the BLE Server
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());


  // Create the BLE Service
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // Create a BLE Characteristic
  pTxCharacteristic = pService->createCharacteristic(
                        CHARACTERISTIC_UUID_TX,
                        BLECharacteristic::PROPERTY_NOTIFY
                      );

  pTxCharacteristic->addDescriptor(new BLE2902());

  BLECharacteristic * pRxCharacteristic = pService->createCharacteristic(
      CHARACTERISTIC_UUID_RX,
      BLECharacteristic::PROPERTY_WRITE
                                          );

  pRxCharacteristic->setCallbacks(new MyCallbacks());

  // Start the service
  pService->start();

  // Start advertising
  pServer->getAdvertising()->start();
  serialUSB.println("Waiting a client connection to notify...");
}

void loop() {

  while (serialUART.available() != 0 )
  {
    ch = serialUART.read();
    if (ch != '\n')
    {
      buf += ch;
    } else {
       buf += '\n';
      fullPkt = true;
    }
  }

  if (deviceConnected && fullPkt) {
    M5.dis.drawpix(0, 0x0000ff); //LED on
    char charBuf[128];
    buf.toCharArray(charBuf, 128);
    pTxCharacteristic->setValue(charBuf);
    pTxCharacteristic->notify();
    fullPkt = false;
    buf = "";
  }


// disconnecting
if (!deviceConnected && oldDeviceConnected) {
  delay(500); // give the bluetooth stack the chance to get things ready
  pServer->startAdvertising(); // restart advertising
  serialUSB.println("start advertising");
  oldDeviceConnected = deviceConnected;
}
// connecting
if (deviceConnected && !oldDeviceConnected) {
  // do stuff here on connecting
  oldDeviceConnected = deviceConnected;
}

if (timeElapsed > interval) //LED off
{
  M5.dis.drawpix(0, 0x000000);
  timeElapsed = 0; // reset counter to 0
}
M5.update();
}
