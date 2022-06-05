#include "init.h"

void setup() {
   M5.begin(true, false, true);
  Serial.begin(57600); //usb not used for this
  Serial2.begin(57600, SERIAL_8N1, 22, 19); //serial to teensy
  // Create the BLE Device
  BLEDevice::init("LAMP-08-03");
 

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

  bool writtenSerial = false;
  // data from esp serial back to BT
  while (Serial2.available() != 0 ) {
    writtenSerial = true;
    ch = Serial2.read();
    M5.dis.drawpix(0, 0x0000ff); //LED on
    pCharacteristic->setValue((uint8_t*)&ch,1);
    // pCharacteristic->notify();
  }
  if (writtenSerial == true) {
    pCharacteristic->notify();
  }
    
  if (timeElapsed > interval) //LED off
  {
   M5.dis.drawpix(0, 0x000000); 
   timeElapsed = 0; // reset counter to 0
  }
  M5.update();
}
