#include <Arduino.h>
#include "init.h"
#include "managers/LedManager.h"
#include "BaseMode.h"


class ColourCyclingMode: public BaseMode
{
private:
    unsigned int howOftenToChange;
    float transitionTimeMs;
    int* colours;
    int countColours;
    elapsedMillis timeSw;
    uint8_t count = 0;

    

public:
    void setup();
    void restart();
    void loop();
    ColourCyclingMode(LEDManager *ledAttach, unsigned int howOftenToChangeAttach, float transitionTimeMs, int coloursAttach[6], int countColours);
};