#include <Arduino.h>
#include "init.h"
#include "managers/LedManager.h"
#include "BaseMode.h"


class ColourCyclingMode: public BaseMode
{
private:
    unsigned int howOftenToChange;
    int* colours;
    int countColours;
    elapsedMillis timeSw;
    uint8_t count = 0;

    void setAllLEDsTo(int r, int g, int b);

public:
    void setup();
    void restart();
    void loop();
    ColourCyclingMode(LEDManager &ledAttach, unsigned int howOftenToChangeAttach, int coloursAttach[6], int countColours);
};