#include <Arduino.h>
#include "init.h"
#include "main.h"
#include "managers/LedManager.h"
#include "modes/ColourCyclingMode.h"
#include "modes/SingleColourMode.h"
#include "inputs/touch_input.h"

#define INITIAL_TOUCH_DOWN_TIME 1000

LEDManager leds;
TouchInput touch(TOUCH_ON, TOUCH_OFF);

// define all instance up front
int cols_1[] = {255, 0, 0, /* */ 0, 255, 0, /* */ 0, 0, 255};
ColourCyclingMode *rgbmode = new ColourCyclingMode(leds, 1500, float(1300), cols_1, 3);
//
int cols_2[] = {255, 255, 255, /* */ 5, 5, 5, /* */ 250, 250, 250, /* */ 0, 0, 0};
ColourCyclingMode *touchdownCyclingMode = new ColourCyclingMode(leds, 2000, float(2000), cols_2, 4);


SingleColourMode *touchdownInitialMode = new SingleColourMode(leds, INITIAL_TOUCH_DOWN_TIME, 0, 255, 255);

// set the first mode
BaseMode *mode = rgbmode;

// functions
void processTouchData(std::tuple<TOUCH_STATE, float> val);

void setup()
{
    // put your setup code here, to run once:
    leds.setup();
    touch.setup();
    mode->setup();
}

void loop()
{
    std::tuple<TOUCH_STATE, float> touchData = touch.loop();
    processTouchData(touchData);

    mode->loop(); // pass any non-mode changing input to mode

    leds.loop(); // update leds last
}

// based on inputs, possibly change mode
void processTouchData(std::tuple<TOUCH_STATE, float> val)
{
    TOUCH_STATE touchState;
    float touchValue;
    std::tie(touchState, touchValue) = val; // get the output of this
    switch (touchState)
    {
    case TOUCH_DOWN:
        //Serial.print("TOUCH DOWN for ");
        //Serial.println(touchValue);
        // only set this if it's not already set
        if (touchValue < INITIAL_TOUCH_DOWN_TIME && mode != touchdownInitialMode)
        {
            Serial.println("TOUCH DOWN initial");
            mode = touchdownInitialMode;
            mode->restart();
        }
        if (touchValue >= INITIAL_TOUCH_DOWN_TIME && mode != touchdownCyclingMode) {
            Serial.println("TOUCH DOWN cycling");
            mode = touchdownCyclingMode;
            mode->restart();
        }
        break;
    case TOUCH_UP:
        Serial.print("TOUCH ended and lasted ");
        Serial.println(touchValue);
        mode = rgbmode;
        mode->restart();
        break;
    case NONE:
        break;
    }
}