#include <Arduino.h>
#include "../../init.h"
#include "../../managers/LedManager.h"
#include "../../managers/PaletteManager.h"
#include "../../managers/AnimationManager.h"
#include "BaseAnimationMode.h"


class ColourWipeMode: public BaseAnimationMode
{
private:
    int currentRow;
    int currentSwatch;
    elapsedMillis timeSw;

public:
    void setup();
    void restart();
    void loop();
    ColourWipeMode(LEDManager *ledAttach, PaletteManager *paletteAttach, AnimationManager *animation);
};