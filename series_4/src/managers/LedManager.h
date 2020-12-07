#ifndef LED_MANAGER_HPP_INCLUDED
#define LED_MANAGER_HPP_INCLUDED

#include "FastLED.h"
#include "init.h"


class LEDManager
{
private:
    CRGB leds[NUM_COLUMNS][NUM_LEDS];

public:
    void setup();
    void loop();
    int totalLEDs();
    void setLED(int x, int y, int r, int g, int b, boolean fixed);
};

#endif