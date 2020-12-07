#include <Arduino.h>
#include "init.h"
#include "managers/LedManager.h"

class BaseMode
{
protected:
    LEDManager &leds;

public:
    BaseMode(LEDManager &ledAttach);
    virtual void setup() = 0;
    virtual void restart() = 0;
    virtual void loop() = 0;
};