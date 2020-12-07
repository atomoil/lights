#include "colour_cycling.h"


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

        /*

        if (count == 1)
            ledRed();
        if (count == 2)
            ledGreen();
        if (count == 3)
            ledBlue();
        if (count == 4)
            ledWhite();
        */
        count++;
    }
}

void ColourCyclingMode::ledRed()
{
    setAllLEDsTo(255, 0, 0);
}

void ColourCyclingMode::ledGreen()
{
    setAllLEDsTo(0, 255, 0);
}

void ColourCyclingMode::ledBlue()
{
    setAllLEDsTo(0, 0, 255);
}

void ColourCyclingMode::ledWhite()
{
    setAllLEDsTo(255, 125, 125);
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