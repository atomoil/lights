#include "LedManager.h"
#include "../jesusgollonet-ofpennereasing-PennerEasing/Sine.h"

void LEDManager::setup()
{
    Serial.println("LEDManager::setup() called");
    /*
    int pins[] = { DATA_PIN1, DATA_PIN2, DATA_PIN3, DATA_PIN4, DATA_PIN5, DATA_PIN6 };
    for (int i = 0; i < NUM_COLUMNS; i++)
    {
        int pin_id = pins[i];
        FastLED.addLeds<APA102, pin_id, CLOCK_PIN1, BGR, DATA_RATE_MHZ(5)>(leds[i], NUM_LEDS);
    }
    */
    brightness = 1.0;

    Serial.println("LEDManager::setup() addLeds next");

    FastLED.addLeds<APA102, DATA_PIN_1, CLOCK_PIN_1, BGR, DATA_RATE_MHZ(5)>(leds[0], NUM_LEDS);
    FastLED.addLeds<APA102, DATA_PIN_2, CLOCK_PIN_2, BGR, DATA_RATE_MHZ(5)>(leds[1], NUM_LEDS);
    FastLED.addLeds<APA102, DATA_PIN_3, CLOCK_PIN_3, BGR, DATA_RATE_MHZ(5)>(leds[2], NUM_LEDS);
    FastLED.addLeds<APA102, DATA_PIN_4, CLOCK_PIN_4, BGR, DATA_RATE_MHZ(5)>(leds[3], NUM_LEDS);
    FastLED.addLeds<APA102, DATA_PIN_5, CLOCK_PIN_5, BGR, DATA_RATE_MHZ(5)>(leds[4], NUM_LEDS);
    FastLED.addLeds<APA102, DATA_PIN_6, CLOCK_PIN_6, BGR, DATA_RATE_MHZ(5)>(leds[5], NUM_LEDS);


    Serial.println("LEDManager::setup() addLeds complete");

    // set initial LED values
    for (int x = 0; x < NUM_COLUMNS; x++)
    {
        for (int y = 0; y < NUM_LEDS; y++)
        {
            matrix[x][y] = {x, y, { 0, NO_TWEEN, 0, 0, 0 }, { 0, NO_TWEEN, 0, 0, 0 }, { 0, NO_TWEEN, 0, 0, 0 }};
            leds[x][y] = CRGB(0, 0, 0);
        }
    }

    FastLED.setMaxPowerInVoltsAndMilliamps(5, 3000);

    frameSize = 1000.0 / LED_FRAME_RATE;
    Serial.println("LEDManager::setup() finished");
}


int LEDManager::xMax()
{
    return NUM_COLUMNS;
}

int LEDManager::yMax()
{
    return NUM_LEDS;
}

void LEDManager::loop()
{
    if (frameMs >= frameSize)
    { // 60 fps
        frameMs = 0;
        updateLEDs();
    }
    FastLED.show();
}

void LEDManager::updateLEDs()
{
    //float time = float(tweenMs);
    boolean debug = true;
    //Serial.println(time);
    for (int x = 0; x < NUM_COLUMNS; x++)
    {
        for (int y = 0; y < NUM_LEDS; y++)
        {
            float time = float(tweenMs[x][y]);
            LedData data = matrix[x][y];
            data.r = updateColour(time, data.r, debug);
            data.g = updateColour(time, data.g, false);
            data.b = updateColour(time, data.b, false);
            debug = false;

            leds[x][y] = CRGB(int(data.r.current * brightness), int(data.g.current * brightness), int(data.b.current * brightness));
            matrix[x][y] = data;
        }
    }
}

LedColourTween LEDManager::updateColour(float time, LedColourTween colour, boolean debug)
{

    if (colour.tweenType == SINE) {
        colour.current = Sine::easeInOut(time, colour.begin, colour.change, colour.duration);
    }
    if (time >= colour.duration){
        colour.tweenType = NO_TWEEN;
    }
    /*
    if (debug == true) {
        char msg[80];
        sprintf(msg, "Sine::easeInOut(%0.2f, %0.2f, %0.2f, %0.2f) = 0.2f", time, colour.begin, colour.change, colour.duration, colour.current);
        Serial.println(msg);
    }*/
    /*
    if (colour.changeValue > 0)
    {
        colour.current = colour.current + colour.changeValue;
        if (colour.current >= colour.desired)
        {
            colour.current = colour.desired;
            colour.changeValue = 0;
        }
    }
    else if (colour.changeValue < 0)
    {
        colour.current = colour.current + colour.changeValue;
        if (colour.current <= colour.desired)
        {
            colour.current = colour.desired;
            colour.changeValue = 0;
        }
    }
    */
    return colour;
}

// 1 second = 60 frames

void LEDManager::setRGB(int xr, int y, int r, int g, int b, float timeInMilliseconds)
{
    int xmap[NUM_COLUMNS] = COLUMN_MAPPING;
    int x = xmap[xr];
    //tweenMs = 0;
    if (x < NUM_COLUMNS && y < NUM_LEDS)
    {
        tweenMs[x][y] = 0;
        LedData data = matrix[x][y];
        if (timeInMilliseconds <= 0)
        { // 0 value means set immediately
            data.r.current = r;
            data.r.tweenType = NO_TWEEN;
            //
            data.g.current = g;
            data.g.tweenType = NO_TWEEN;
            //
            data.b.current = b;
            data.b.tweenType = NO_TWEEN;
        }
        else
        {
            //
            data.r.begin = data.r.current;
            data.r.change = r - data.r.current;
            data.r.duration = timeInMilliseconds;
            data.r.tweenType = SINE;
            //
            data.g.begin = data.g.current;
            data.g.change = g - data.g.current;
            data.g.duration = timeInMilliseconds;
            data.g.tweenType = SINE;
            //
            data.b.begin = data.b.current;
            data.b.change = b - data.b.current;
            data.b.duration = timeInMilliseconds;
            data.b.tweenType = SINE;
        }
        matrix[x][y] = data;
    }
}

void LEDManager::setHSV(int x, int y, int h, int s, int v, float timeInMilliseconds)
{
    CRGB colour = CRGB().setHSV(h, s, v);
    setRGB(x, y, colour.r, colour.g, colour.b, timeInMilliseconds);
}

void LEDManager::setBrightness(float level)
{
    // @TODO - clamp values to 0.1 - 1
    if (level > 1) {
        brightness = 1;
    } else if (level < 0.1) {
        brightness = 0.1;
    } else {
         brightness = level;
    }
}
float LEDManager::getBrightness()
{
    return brightness;
}