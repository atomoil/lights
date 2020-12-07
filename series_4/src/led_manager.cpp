#include "led_manager.h"

void LEDManager::setup()
{
    /*
    int pins[] = { DATA_PIN1, DATA_PIN2, DATA_PIN3, DATA_PIN4, DATA_PIN5, DATA_PIN6 };
    for (int i = 0; i < NUM_COLUMNS; i++)
    {
        int pin_id = pins[i];
        FastLED.addLeds<APA102, pin_id, CLOCK_PIN1, BGR, DATA_RATE_MHZ(5)>(leds[i], NUM_LEDS);
    }
    */

    FastLED.addLeds<APA102, DATA_PIN1, CLOCK_PIN1, BGR, DATA_RATE_MHZ(5)>(leds[0], NUM_LEDS);
    FastLED.addLeds<APA102, DATA_PIN2, CLOCK_PIN2, BGR, DATA_RATE_MHZ(5)>(leds[1], NUM_LEDS);
    FastLED.addLeds<APA102, DATA_PIN3, CLOCK_PIN3, BGR, DATA_RATE_MHZ(5)>(leds[2], NUM_LEDS);
    FastLED.addLeds<APA102, DATA_PIN4, CLOCK_PIN4, BGR, DATA_RATE_MHZ(5)>(leds[3], NUM_LEDS);
    FastLED.addLeds<APA102, DATA_PIN5, CLOCK_PIN5, BGR, DATA_RATE_MHZ(5)>(leds[4], NUM_LEDS);
    FastLED.addLeds<APA102, DATA_PIN6, CLOCK_PIN6, BGR, DATA_RATE_MHZ(5)>(leds[5], NUM_LEDS);

    // set initial LED values
    for (int x = 0; x < NUM_COLUMNS; x++)
    {
        for (int y = 0; y < NUM_LEDS; y++)
        {
            leds[x][y] = CRGB(125, 125, 125);
        }
    }

    FastLED.setMaxPowerInVoltsAndMilliamps(5, 3000);
}

void LEDManager::loop()
{
    // @TODO transition colours towards desired
    FastLED.show();
}

void LEDManager::setLED(int x, int y, int r, int g, int b, boolean fixed)
{
    if (x < NUM_COLUMNS && y < NUM_LEDS)
    {
        leds[x][y] = CRGB(r, g, b);
    }
}