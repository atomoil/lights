#include <Arduino.h>
#include "../init.h"
#include "LampMessage.h"

#define BLE_Serial Serial1

class BluetoothInput
{
private:
    String ssData = "";
    boolean stringComplete = false;
    boolean showErrorOnce = true;
    LampMessage processMessage(String message);

public:
    BluetoothInput();
    void setup();
    LampMessage loop();
    void sendMessage(char* message);
    //std::tuple<TOUCH_STATE,float> loop(); // should be Actions loop(); but one thing at a time!
};