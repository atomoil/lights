#include <Arduino.h>
#include "init.h"
#include "main.h"
#include "led_manager.h"
#include "modes/cycling_test.h"
#include "inputs/touch_input.h"

LEDManager leds;
CyclingTestMode mode(leds);
TouchInput touch(TOUCH_ON, TOUCH_OFF);

void setup()
{
    // put your setup code here, to run once:
    leds.setup();
    touch.setup();
    mode.setup();
}

void loop()
{
    // put your main code here, to run repeatedly:
    // TOUCH INPUT
    std::tuple<STATE, float> touchData = touch.loop();
    processTouchData(touchData);

    mode.loop(); // pass any non-mode changing input to mode

    leds.loop(); // update leds last
}

// based on inputs, possibly change mode
void processTouchData(std::tuple<STATE,float> val) {
    STATE touchState;
    float touchValue;
    std::tie (touchState, touchValue) = touch.loop(); // get the output of this
    if (touchState == TOUCH_DOWN) {
        Serial.print("TOUCH DOWN for ");
        Serial.println(touchValue);
    } else if (touchState == TOUCH_UP) {
        Serial.print("TOUCH ended and lasted ");
        Serial.println(touchValue);
    }
}