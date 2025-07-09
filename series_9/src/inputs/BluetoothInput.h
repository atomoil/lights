#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <queue>

#include "../init.h"
#include "LampMessage.h"



class BluetoothInput
{
private:
    class BTCallbacks : public BLECharacteristicCallbacks {
    public:
        BTCallbacks(BluetoothInput* parent) : parent(parent) {}
        void onWrite(BLECharacteristic *pCharacteristic) override {
            std::string value = pCharacteristic->getValue();
            // Add value to an array or buffer in BluetoothInput
            parent->onBLEWrite(value);
        }
    private:
        BluetoothInput* parent;
    };

    void onBLEWrite(const std::string& value); // Add this method
    LampMessage processMessage(String message);

    String ssData = "";
    boolean stringComplete = false;
    boolean showErrorOnce = true;
    BLEServer* pServer = NULL;

    BLECharacteristic* pCharacteristic = NULL;
    std::queue<std::string> messageQueue;

public:
    BluetoothInput();
    void setup();
    LampMessage loop();
    int getMessageCount();
    void sendMessage(char* message);
    //std::tuple<TOUCH_STATE,float> loop(); // should be Actions loop(); but one thing at a time!

    bool deviceConnected = false;
};