#include "cycling_test.h"


CyclingTestMode::CyclingTestMode(LEDManager &ledAttach): leds(ledAttach) {}

void CyclingTestMode::setup() {
    setAllLEDsTo(255,255,255);
}

void CyclingTestMode::loop()
{
    // put your main code here, to run repeatedly:
    // LEDs
    if (timeSw >= deltaSw)
    {
        timeSw = 0;
        if (count == 5)
            count = 1;
        
        if (count == 1)
            ledRed();
        if (count == 2)
            ledGreen();
        if (count == 3)
            ledBlue();
        if (count == 4)
            ledWhite();
        count++;
    }
}

void CyclingTestMode::ledRed()
{
    setAllLEDsTo(255, 0, 0);
}

void CyclingTestMode::ledGreen()
{
    setAllLEDsTo(0, 255, 0);
}

void CyclingTestMode::ledBlue()
{
    setAllLEDsTo(0, 0, 255);
}

void CyclingTestMode::ledWhite()
{
    setAllLEDsTo(255, 125, 125);
}

void CyclingTestMode::setAllLEDsTo(int r, int g, int b)
{
    for (int x = 0; x < NUM_LEDS; x++)
    {
        for (int y = 0; y < NUM_LEDS; y++)
        {
            leds.setLED(x, y, r, g, b, true);
        }
    }
}