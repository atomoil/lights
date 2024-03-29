#include "init.h"

void setup() {
   M5.begin(true, false, true);
  Serial.begin(57600); //usb not used for this
  Serial2.begin(57600, SERIAL_8N1, 22, 19); //serial to teensy
  // Create the BLE Device
  BLEDevice::init("LAMP-08-02");

  // Create the BLE Server
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new serverCallbacks());

  // Create the BLE Service
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // Create a BLE Characteristic
  pCharacteristic = pService->createCharacteristic(
                      CHARACTERISTIC_UUID_TX,
                      BLECharacteristic::PROPERTY_NOTIFY
                    );
  pCharacteristic->addDescriptor(new BLE2902());

  /*
  pCharacteristic = pService->createCharacteristic(
                                        CHARACTERISTIC_UUID_RX,
                                        BLECharacteristic::PROPERTY_WRITE
                                      );
  */
  pCharacteristic = pService->createCharacteristic(CHARACTERISTIC_UUID, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE_NR );

  pCharacteristic->setCallbacks(new BTCallbacks());
  // pCharacteristic->setValue("Hello from LAMP-99");


  // Start the service
  pService->start();

  // Start advertising
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(false);
  pAdvertising->setMinPreferred(0x0);  // set value to 0x00 to not advertise this parameter
  BLEDevice::startAdvertising();
  //Serial.println("Waiting a client connection to notify...");
}

void loop() {

  // disconnecting
  if (!deviceConnected && oldDeviceConnected) {
      delay(500); // give the bluetooth stack the chance to get things ready
      pServer->startAdvertising(); // restart advertising
      // Serial.println("start advertising");
      oldDeviceConnected = deviceConnected;
  }
  // connecting
  if (deviceConnected && !oldDeviceConnected) {
      // do stuff here on connecting
      // Serial.println("connecting");
      oldDeviceConnected = deviceConnected;
  }

  while (serialTeensy.available() != 0 )
  {
    ch = serialTeensy.read();
    if (ch != '\n')
    {
      buf += ch;
    } else {
      // buf += '\n';
      fullPkt = true;
    }
  }

  if (deviceConnected && fullPkt) {
    M5.dis.drawpix(0, 0x0000ff); //LED on
    char charBuf[128];
    buf.toCharArray(charBuf, 128);
    pCharacteristic->setValue(charBuf);
    pCharacteristic->notify();
    serialUSB.println(buf);
    fullPkt = false;
    buf = "";
  }

  if (timeElapsed > interval) //LED off
  {
   M5.dis.drawpix(0, 0x000000); 
   timeElapsed = 0; // reset counter to 0
  }

  M5.update();
}
