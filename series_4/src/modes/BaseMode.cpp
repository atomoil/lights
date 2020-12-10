#include "BaseMode.h"

BaseMode::BaseMode(
    LEDManager *ledAttach): 
        leds(ledAttach)
{};


void BaseMode::setAllLEDsTo(int r, int g, int b, float transitionTime)
{
    char message[30];
    sprintf(message, "setAllLEDs to %i, %i, %i",r,g,b);
    Serial.println(message);
    /*
    if (r > 255) r = 255;
    if (g > 255) g = 255;
    if (b > 255) b = 255;
    */
    for (int x = 0; x < NUM_LEDS; x++)
    {
        for (int y = 0; y < NUM_LEDS; y++)
        {
            leds->setRGB(x, y, r, g, b, transitionTime);
        }
    }
}