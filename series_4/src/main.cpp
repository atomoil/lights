#include <Arduino.h>
#include "init.h"
#include "main.h"
#include "led_manager.h"
#include "modes/ColourCyclingMode.h"
#include "inputs/touch_input.h"

LEDManager leds;
TouchInput touch(TOUCH_ON, TOUCH_OFF);

// define all instance up front
int cols_1[] = {255,0,0,    0,255,0,  0,0,255 };
ColourCyclingMode rgbmode(leds, 1500, cols_1, 3);
//
int cols_2[] = { 10,10,10,   50,50,50,  150,150,150 };
ColourCyclingMode touchdownmode(leds, 500, cols_2, 3);
ColourCyclingMode* mode = &rgbmode;

// functions
void processTouchData(std::tuple<TOUCH_STATE,float> val);

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
void processTouchData(std::tuple<TOUCH_STATE,float> val) {
    TOUCH_STATE touchState;
    float touchValue;
    std::tie (touchState, touchValue) = touch.loop(); // get the output of this
    if (touchState == TOUCH_DOWN) {
        Serial.print("TOUCH DOWN for ");
        Serial.println(touchValue);
        mode = &touchdownmode;
        // mode = ColourCyclingMode(leds);
    } else if (touchState == TOUCH_UP) {
        Serial.print("TOUCH ended and lasted ");
        Serial.println(touchValue);
        mode = &rgbmode;
    }
}