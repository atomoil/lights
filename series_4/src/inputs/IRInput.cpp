#include "IRInput.h"

IRInput::IRInput() {
    irrecv = new IRrecv(IR_RECV_PIN);
}

void IRInput::setup()
{
    //irrecv->enableIRIn(); // Start the IR receiver
}

LampMessage IRInput::loop()
{
    char* empty; // for returns with no value;
    if (irrecv->decode(&results))
    {
        if (results.value == POWER)
        {
            return {LAMP_TOGGLE_ON, 0, empty};
            /*
            Serial.println("POWER");

            if (app_mode != MODE_REACTIVE)
            {
                // we're in FFT mode
                allLightsOff();
            }
            else
            {
                // we're in REACTIVE mode
                if (currentState == STATE_ON_BRIGHT)
                {
                    allLightsOff();
                }
                else if (currentState == STATE_ON_ANIMATED)
                {
                    allLightsFadeDown();
                }
                else
                {
                    allLightsOn();
                }
            }
            */
        }
#ifdef SUPPORTS_FFT
        if (results.value == A)
        {
            Serial.println("A");
            return {FFT_MODE, 0, empty};
        }
        if (results.value == B)
        {
            Serial.println("B");
            return {FFT_MODE, 1, empty};
        }
        if (results.value == C)
        {
            Serial.println("C");
            return {FFT_MODE, 2, empty};
        }
#else
        if (results.value == A)
        {
            Serial.println("A");
            return {SET_ANIM_SPEED, 200, empty};
        }
        if (results.value == B)
        {
            Serial.println("B");
            return {SET_ANIM_SPEED, 5000, empty};
        }
        if (results.value == C)
        {
            Serial.println("C");
            return {SET_ANIM_SPEED, 20000, empty};
        }
#endif

        if (results.value == UP)
        {
            Serial.println("UP");
            return {MULT_ANIM_SPEED, 1.25, empty};
        }
        if (results.value == DOWN)
        {
            Serial.println("DOWN");
            return {MULT_ANIM_SPEED, 0.75, empty};
        }
        if (results.value == LEFT)
        {
            Serial.println("LEFT");
            return {INC_BRIGHTNESS, -0.1, empty};
        }
        if (results.value == RIGHT)
        {
            Serial.println("RIGHT");
            return {INC_BRIGHTNESS, 0.1, empty};
        }
        if (results.value == SELECT)
        {
            Serial.println("SELECT");
        }
        irrecv->resume();
    }
    return {LAMP_NONE, 0, empty};
}