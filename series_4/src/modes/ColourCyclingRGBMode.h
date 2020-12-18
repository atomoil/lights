#include <Arduino.h>
#include "../init.h"
#include "../managers/LedManager.h"
#include "BaseMode.h"

#ifndef COLOURCYCLING_MAX_PALETTE
#define COLOURCYCLING_MAX_PALETTE 30
#endif

class ColourCyclingRGBMode: public BaseMode
{
private:
    unsigned int howOftenToChange;
    float transitionTimeMs;
    int colours[COLOURCYCLING_MAX_PALETTE];
    int countColours;
    elapsedMillis timeSw;
    uint8_t count = 0;

    

public:
    void setup();
    void restart();
    void loop();
    ColourCyclingRGBMode(LEDManager *ledAttach, unsigned int howOftenToChangeAttach, float transitionTimeMs, int coloursAttach[], int countColours);
};