#include <Arduino.h>
#include "init.h"
#include "led_manager.h"

class CyclingTestMode
{
private:
    LEDManager &leds;
    const unsigned int deltaSw = 500;
    uint8_t  count = 1;
    elapsedMillis timeSw;

    // void loopLEDs();
    void ledRed();
    void ledGreen();
    void ledBlue();
    void ledWhite();
    void setAllLEDsTo(int r, int g, int b);

public:
    void setup();
    void loop();
    CyclingTestMode(LEDManager &ledAttach);
};