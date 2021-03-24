#include "BaseMode.h"

BaseMode::BaseMode(
    LEDManager *ledAttach): 
        leds(ledAttach)
{
    modeId = -1;
    modeName = "?";
};


void BaseMode::setAllLEDsToRGB(int r, int g, int b, float transitionTime)
{
    for (int x = 0; x < NUM_COLUMNS; x++)
    {
        for (int y = 0; y < NUM_LEDS; y++)
        {
            leds->setRGB(x, y, r, g, b, transitionTime);
        }
    }
}

void BaseMode::setAllLEDsToHSV(int h, int s, int v, float transitionTime)
{
    for (int x = 0; x < NUM_COLUMNS; x++)
    {
        for (int y = 0; y < NUM_LEDS; y++)
        {
            leds->setHSV(x, y, h, s, v, transitionTime);
        }
    }
}