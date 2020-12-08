#include "ColourCyclingMode.h"


ColourCyclingMode::ColourCyclingMode(
    LEDManager &ledAttach, 
    unsigned int howOftenToChangeAttach,
    float transitionTimeMsAttach,
    int coloursAttach[], 
    int countColoursAttach): 
        BaseMode(ledAttach),
        howOftenToChange(howOftenToChangeAttach),
        transitionTimeMs(transitionTimeMsAttach),
        colours(coloursAttach),
        countColours(countColoursAttach)
{}

void ColourCyclingMode::setup() {
    setAllLEDsTo(255,255,255,0); // this should never actually be seen!
    restart();
}

void ColourCyclingMode::restart()
{
    timeSw = howOftenToChange; // get triggered immediately
    count = 0;
}

void ColourCyclingMode::loop()
{
    // put your main code here, to run repeatedly:
    // LEDs
    if (timeSw >= howOftenToChange)
    {
        timeSw = 0;

        if (count >= countColours)
            count = 0;
        
        int col_index = count * 3;
        int r = colours[col_index];
        int g = colours[col_index+1];
        int b = colours[col_index+2];
        setAllLEDsTo( r, g, b, transitionTimeMs );

        count++;
    }
}

