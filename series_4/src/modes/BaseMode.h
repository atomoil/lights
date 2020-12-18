#ifndef BASE_MODE_HPP_INCLUDED
#define BASE_MODE_HPP_INCLUDED

#include <Arduino.h>
#include "../init.h"
#include "../managers/LedManager.h"

class BaseMode
{
protected:
    LEDManager *leds;
    void setAllLEDsToRGB(int r, int g, int b, float transitionTime);
    void setAllLEDsToHSV(int h, int s, int v, float transitionTime);
public:
    BaseMode(LEDManager *ledAttach);
    virtual void setup() = 0;
    virtual void restart() = 0;
    virtual void loop() = 0;
};


#endif