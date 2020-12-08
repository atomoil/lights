#include <Arduino.h>
#include "managers/LedManager.h"
#include "BaseMode.h"




class SingleColourMode: public BaseMode
{
private:
    float transitionTimeMs;
    int r;
    int g;
    int b;
    boolean hasTriggered = false;
public:
    void setup();
    void restart();
    void loop();
    SingleColourMode(LEDManager &ledAttach, float transitionTimeMs, int r, int g, int b);
};