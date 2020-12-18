#include "SetColourOnceMode.h"

SetColourOnceMode::SetColourOnceMode(
    LEDManager *ledAttach,
    float transitionTimeMsAttach,
    int rAttach,
    int gAttach,
    int bAttach) : BaseMode(ledAttach),
                   transitionTimeMs(transitionTimeMsAttach),
                   r(rAttach),
                   g(gAttach),
                   b(bAttach)
{
}


void SetColourOnceMode::setup() {
    restart();
}

void SetColourOnceMode::restart() {
    hasTriggered = false;
}

void SetColourOnceMode::loop() {
    if (hasTriggered == false) {
        hasTriggered = true;
        setAllLEDsToRGB(r, g, b, transitionTimeMs);
    }
}