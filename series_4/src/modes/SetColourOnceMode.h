#include <Arduino.h>
#include "../managers/LedManager.h"
#include "BaseMode.h"




class SetColourOnceMode: public BaseMode
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
    SetColourOnceMode(LEDManager *ledAttach, float transitionTimeMs, int r, int g, int b);
};