#include <Arduino.h>
#include "../../init.h"
#include "../../managers/LedManager.h"
#include "../../managers/PaletteManager.h"
#include "../../managers/AnimationManager.h"
#include "BaseAnimationMode.h"
#include "movingdots/MovingDot.h"

#define MAX_DOTS 2

class MovingDotsMode: public BaseAnimationMode
{
private:
    elapsedMillis timeSw;
    MovingDot* dots[MAX_DOTS];

public:
    void setup();
    void restart();
    void loop();
    MovingDotsMode(LEDManager *ledAttach, PaletteManager *paletteAttach, AnimationManager *animation);
};

struct DotColour {
    float hue;
    float saturation;
    float brightness;
};