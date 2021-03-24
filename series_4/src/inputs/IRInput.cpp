#include "IRInput.h"

IRInput::IRInput() {
    irrecv = new IRrecv(IR_RECV_PIN);
}

void IRInput::setup()
{
    irrecv->enableIRIn(); // Start the IR receiver
}

LampMessage IRInput::loop()
{
    String empty = ""; // for returns with no value;
    if (irrecv->decode(&results))
    {
        irrecv->resume();
        if (results.value == POWER)
        {
            Serial.println("POWER");
            return {LAMP_TOGGLE_ON, 0, 0, empty};

        }
#ifdef SUPPORTS_FFT
        if (results.value == A)
        {
            Serial.println("A");
            //return {FFT_MODE, 0, 0, empty};
            return {CYCLE_ANIM_MODE, 0, 0, empty};
        }
        if (results.value == B)
        {
            Serial.println("B");
            //return {FFT_MODE, 1, 0, empty};
            return {CYCLE_FFT_MODE, 0, 0, empty};
        }
        if (results.value == C)
        {
            Serial.println("C");
            // return {FFT_MODE, 2, 0, empty};
        }
#else
        if (results.value == A)
        {
            Serial.println("A");
            return {SET_ANIM_SPEED, 200, 0, empty};
        }
        if (results.value == B)
        {
            Serial.println("B");
            return {SET_ANIM_SPEED, 5000, 0, empty};
        }
        if (results.value == C)
        {
            Serial.println("C");
            return {SET_ANIM_SPEED, 20000, 0, empty};
        }
#endif

        if (results.value == UP)
        {
            Serial.println("UP");
            return {MULT_ANIM_SPEED, 1.25, 0, empty};
        }
        if (results.value == DOWN)
        {
            Serial.println("DOWN");
            return {MULT_ANIM_SPEED, 0.75, 0, empty};
        }
        if (results.value == LEFT)
        {
            Serial.println("LEFT");
            return {INC_BRIGHTNESS, -0.1, 0, empty};
        }
        if (results.value == RIGHT)
        {
            Serial.println("RIGHT");
            return {INC_BRIGHTNESS, 0.1, 0, empty};
        }
        if (results.value == SELECT)
        {
            Serial.println("SELECT");
        }

    }
    return {LAMP_NONE, 0, 0, empty};
}