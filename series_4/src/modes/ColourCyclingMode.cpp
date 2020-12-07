#include "ColourCyclingMode.h"


ColourCyclingMode::ColourCyclingMode(
    LEDManager &ledAttach, 
    unsigned int howOftenToChangeAttach, 
    int coloursAttach[], 
    int countColoursAttach): 
        BaseMode(ledAttach),
        howOftenToChange(howOftenToChangeAttach),
        colours(coloursAttach),
        countColours(countColoursAttach)
{}

void ColourCyclingMode::setup() {
    setAllLEDsTo(255,255,255);
    Serial.print("Total colours are:");
    Serial.println(countColours);
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
        setAllLEDsTo( r, g, b);

        count++;
    }
}

void ColourCyclingMode::setAllLEDsTo(int r, int g, int b)
{
    for (int x = 0; x < NUM_LEDS; x++)
    {
        for (int y = 0; y < NUM_LEDS; y++)
        {
            leds.setLED(x, y, r, g, b, true);
        }
    }
}