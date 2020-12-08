#include "init.h"

void setup() {
  //void begin(bool LCDEnable=true, bool SDEnable=true, bool SerialEnable=true,bool I2CEnable=false);
   M5.begin(true, false, true);
  //Serial.begin(57600); //usb not used for this
  Serial2.begin(57600, SERIAL_8N1, 22, 19); //serial to teensy

 //---- LAMP NAME ----//
  BLEDevice::init("LAMP-06-01");

  // Create Bluetooth server and service
  BLEServer *pServer = BLEDevice::createServer();

  BLEService *pService = pServer->createService(SERVICE_UUID);
  pCharacteristic = pService->createCharacteristic(CHARACTERISTIC_UUID, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE);

  pCharacteristic->setCallbacks(new BTCallbacks());
  // pCharacteristic->setValue("Hello from LAMP-99");
  
  pService->start();
  BLEAdvertising *pAdvertising = pServer->getAdvertising();
  pAdvertising->start();
}

void loop() {
  while (Serial2.available() != 0 ) {
   ch = Serial2.read();
   M5.dis.drawpix(0, 0x0000ff); //LED on
   pCharacteristic->setValue((uint8_t*)&ch,1);
   pCharacteristic->notify();
 }
    
  if (timeElapsed > interval) //LED off
  {
   M5.dis.drawpix(0, 0x000000); 
   timeElapsed = 0; // reset counter to 0
  }
   M5.update();
}
