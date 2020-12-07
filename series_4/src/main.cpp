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
