#include "SingleColourMode.h"

SingleColourMode::SingleColourMode(
    LEDManager &ledAttach,
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


void SingleColourMode::setup() {
    restart();
}

void SingleColourMode::restart() {
    hasTriggered = false;
}

void SingleColourMode::loop() {
    if (hasTriggered == false) {
        hasTriggered = true;
        setAllLEDsTo(r, g, b, transitionTimeMs);
    }
}