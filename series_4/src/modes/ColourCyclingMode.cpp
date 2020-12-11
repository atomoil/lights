#include "ColourCyclingMode.h"


ColourCyclingMode::ColourCyclingMode(
    LEDManager *ledAttach, 
    unsigned int howOftenToChangeAttach,
    float transitionTimeMsAttach,
    int coloursAttach[], 
    int countColoursAttach): 
        BaseMode(ledAttach),
        howOftenToChange(howOftenToChangeAttach),
        transitionTimeMs(transitionTimeMsAttach),
        countColours(countColoursAttach)
{
    int max = countColours*3;
    if (max > COLOURCYCLING_MAX_PALETTE) {
        Serial.print("Warning too many colours passed (");
        Serial.print(countColours);
        Serial.print(") truncating to ");
        Serial.print((COLOURCYCLING_MAX_PALETTE/3));
        Serial.println(" if that's not enough, increase COLOURCYCLING_MAX_PALETTE");
        max = COLOURCYCLING_MAX_PALETTE;
    }
    for(int i=0;i<max;i++) {
        colours[i] = coloursAttach[i];
    }
}

void ColourCyclingMode::setup() {
    setAllLEDsTo(255,255,255,0); // this should never actually be seen!
    restart();
    /*
    // debug for passing in array
    for(int i=0;i<countColours;i++) {
        char message[40];
        int off = i*3;
        sprintf(message,"%i > %i,%i,%i",i,colours[off],colours[off+1],colours[off+2]);
        Serial.print("ColourCycling ");
        Serial.println(message);
    }*/
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

