#include "SingleColourAnimatingMode.h"

SingleColourAnimatingMode::SingleColourAnimatingMode(
    LEDManager *ledAttach,
    float transitionTimeMsAttach,
    int hueAttach,
    int satAttach) : BaseMode(ledAttach),
                     transitionTimeMs(transitionTimeMsAttach),
                     hue(hueAttach),
                     sat(satAttach)
{
}

void SingleColourAnimatingMode::setup() {
    restart();
}

void SingleColourAnimatingMode::restart() {
    triggerMs = transitionTimeMs + 10; // force it to trigger
    animateUp = true;
}

void SingleColourAnimatingMode::loop() {
    if (triggerMs >= transitionTimeMs) {
        triggerMs = 0;
        int brightness = 1; // almost off but not quite
        if (animateUp == true) {
            brightness = 255;
        }
        setAllLEDsToHSV(hue, sat, brightness, transitionTimeMs);
        animateUp = !animateUp;
    }
}


void SingleColourAnimatingMode::updateColour(int h, int s) {
    hue = hue;
    sat = sat;
}