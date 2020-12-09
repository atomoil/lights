#include "BaseMode.h"

BaseMode::BaseMode(
    LEDManager *ledAttach): 
        leds(ledAttach)
{};


void BaseMode::setAllLEDsTo(int r, int g, int b, float transitionTime)
{
    for (int x = 0; x < NUM_LEDS; x++)
    {
        for (int y = 0; y < NUM_LEDS; y++)
        {
            leds.setLED(x, y, r, g, b, transitionTime);
        }
    }
}