#include <Arduino.h>
#include "../init.h"
#include "../managers/LedManager.h"
#include "../managers/PaletteManager.h"
#include "BaseMode.h"


class ColourWipeMode: public BaseMode
{
private:
    PaletteManager *palette;
    unsigned int howOftenToChange;
    float transitionTimeMs;
    int currentRow;
    int currentSwatch;
    elapsedMillis timeSw;

public:
    void setup();
    void restart();
    void loop();
    ColourWipeMode(LEDManager *ledAttach, PaletteManager *paletteAttach, unsigned int howOftenToChangeAttach, float transitionTimeMs);
};