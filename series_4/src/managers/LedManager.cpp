#include "LedManager.h"

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
            leds[x][y] = CRGB(0, 15, 0);
        }
    }

    FastLED.setMaxPowerInVoltsAndMilliamps(5, 3000);

    frameSize = 1000.0/FRAME_RATE;
}

void LEDManager::loop()
{
    // @TODO transition colours towards desired
    if (frameMs >= frameSize) { // 60 fps
        frameMs = 0;
        updateLEDs();
    }
    FastLED.show();
}

void LEDManager::updateLEDs() {
    for(int x=0;x<NUM_COLUMNS;x++) {
        for(int y=0;y<NUM_LEDS;y++) {
            LedData data = matrix[x][y];
            data.r = updateColour(data.r);
            data.g = updateColour(data.g);
            data.b = updateColour(data.b);

            leds[x][y] = CRGB(int(data.r.current), int(data.g.current), int(data.b.current));
            matrix[x][y] = data;
        }
    }
}


LedColour LEDManager::updateColour(LedColour colour){
    if (colour.changeValue > 0) {
        colour.current = colour.current+colour.changeValue;
        if (colour.current >= colour.desired) {
            colour.current = colour.desired;
            colour.changeValue = 0;
        }
    } else if (colour.changeValue < 0) {
        colour.current = colour.current+colour.changeValue;
        if (colour.current <= colour.desired) {
            colour.current = colour.desired;
            colour.changeValue = 0;
        }
    }
    return colour;
}

// 1 second = 60 frames

void LEDManager::setLED(int x, int y, int r, int g, int b, float timeInMilliseconds)
{
    if (x < NUM_COLUMNS && y < NUM_LEDS)
    {
        //leds[x][y] = CRGB(r, g, b);
        LedData data = matrix[x][y];
        if (timeInMilliseconds <= 0) { // 0 value means set immediately
            data.r.changeValue = 0;
            data.r.current = r;
            data.r.desired = r;
            //
            data.g.changeValue = 0;
            data.g.current = g;
            data.g.desired = g;
            //
            data.b.changeValue = 0;
            data.b.current = b;
            data.b.desired = b;
        } else {
            // map seconds to 'frames'
            float frames = timeInMilliseconds / FRAME_RATE;
            //
            data.r.desired = r;
            data.r.changeValue = (data.r.desired-data.r.current)/frames;
            //
            data.g.desired = g;
            data.g.changeValue = (data.g.desired-data.g.current)/frames;
            //
            data.b.desired = b;
            data.b.changeValue = (data.b.desired-data.b.current)/frames;
        }
        matrix[x][y] = data;
    }
}

int LEDManager::totalLEDs() {
    return NUM_COLUMNS * NUM_LEDS;
}