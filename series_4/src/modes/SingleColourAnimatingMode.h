#include <Arduino.h>
#include "../managers/LedManager.h"
#include "BaseMode.h"


class SingleColourAnimatingMode: public BaseMode
{
private:
    float transitionTimeMs;
    int hue;
    int sat;
    boolean animateUp;
    elapsedMillis triggerMs;
public:
    void setup();
    void restart();
    void loop();
    void updateColour(int hue, int sat);
    SingleColourAnimatingMode(LEDManager *ledAttach, float transitionTimeMs, int hue, int sat);
};