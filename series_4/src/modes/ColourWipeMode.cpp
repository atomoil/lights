
#include "ColourWipeMode.h"

ColourWipeMode::ColourWipeMode(
    LEDManager *ledAttach,
    PaletteManager *paletteAttach,
    unsigned int howOftenToChangeAttach,
    float transitionTimeMsAttach) : BaseMode(ledAttach),
                                    palette(paletteAttach),
                                    howOftenToChange(howOftenToChangeAttach),
                                    transitionTimeMs(transitionTimeMsAttach)
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
    if (timeSw > howOftenToChange) {
        timeSw = 0;

        int hue = palette->hueForSwatch(currentSwatch);
        int sat = palette->satForSwatch(currentSwatch);
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