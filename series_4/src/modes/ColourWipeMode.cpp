
#include "ColourWipeMode.h"

ColourWipeMode::ColourWipeMode(
    LEDManager *ledAttach,
    PaletteManager *paletteAttach,
    AnimationManager *animationAttach) : BaseAnimationMode(ledAttach, paletteAttach, animationAttach)
{
}

void ColourWipeMode::setup()
{
    restart();
}

void ColourWipeMode::restart()
{
    currentRow = 0;
    currentSwatch = 0;
}

void ColourWipeMode::loop()
{
    if (timeSw > animation->getSpeed()) {
        timeSw = 0;

        int hue = palette->hueForSwatch(currentSwatch);
        int sat = palette->satForSwatch(currentSwatch);
        float transitionTimeMs = animation->getSpeed() * 4;
        for(int i=0;i<NUM_COLUMNS;i++){
            leds->setHSV(i, currentRow, hue, sat, 255, transitionTimeMs);
        }

        currentRow += 1;
        if (currentRow >= NUM_LEDS) {
            currentRow = 0;

            currentSwatch += 1;
            if (currentSwatch >= palette->totalSwatches()){
                currentSwatch = 0;
            }
        } 
    }
}