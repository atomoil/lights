#include <Arduino.h>
#include "init.h"
#include "main.h"
#include "led_manager.h"
#include "modes/cycling_test.h"

LEDManager leds;
CyclingTestMode mode(leds);

void setup()
{
    // put your setup code here, to run once:
    leds.setup();
    mode.setup();
}

void loop()
{
    // put your main code here, to run repeatedly:
    //loopLEDs();
    mode.loop();
    leds.loop();
}

/*
void loopLEDs()
{
    // put your main code here, to run repeatedly:
    // LEDs
    if (timeSw >= deltaSw)
    {
        //SerialUSB.println(count);
        timeSw = 0;
        if (count == 5)
            (count = 1);

        if (count == 1)
            (ledRed());
        if (count == 2)
            (ledGreen());
        if (count == 3)
            (ledBlue());
        if (count == 4)
            (ledWhite());
        count++;
    }
}

void ledRed()
{
    setAllLEDsTo(255, 0, 0);
}

void ledGreen()
{
    setAllLEDsTo(0, 255, 0);
}

void ledBlue()
{
    setAllLEDsTo(0, 0, 255);
}

void ledWhite()
{
    setAllLEDsTo(15, 15, 15);
}

void setAllLEDsTo(int r, int g, int b)
{
    for (int x = 0; x < NUM_LEDS; x++)
    {
        for (int y = 0; y < NUM_LEDS; y++)
        {
            leds.setLED(x, y, r, g, b, true);
        }
    }
}
*/