#ifndef BASE_MODE_HPP_INCLUDED
#define BASE_MODE_HPP_INCLUDED

#include <Arduino.h>
#include "../init.h"
#include "../managers/LedManager.h"

class BaseMode
{
protected:
    LEDManager *leds;
    void setAllLEDsTo(int r, int g, int b, float transitionTime); // this would be useful to be shared...
public:
    BaseMode(LEDManager *ledAttach);
    virtual void setup() = 0;
    virtual void restart() = 0;
    virtual void loop() = 0;
};


#endif