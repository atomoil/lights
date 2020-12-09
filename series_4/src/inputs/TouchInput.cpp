#include "TouchInput.h"

TouchInput::TouchInput(int on, int off) : triggerOn(on),
                                          triggerOff(off)
{
}

void TouchInput::setup()
{
    filterLP = FilterOnePole(LOWPASS, 1.0);
    filterLP2 = FilterOnePole(LOWPASS, 1.0);
    isTouching = false;
    sBias = touchRead(TOUCH_PIN);
    delay(500);                   // let power supply settle
    sBias = touchRead(TOUCH_PIN); // DC offset, noise cal.
}

// tuple use from https://www.cplusplus.com/reference/tuple/tuple/
std::tuple<TOUCH_STATE, float> TouchInput::loop()
{
    boolean wasTouching = isTouching;

    //if (nextFrameMs >= 10) {
    //    nextFrameMs = 0;
    int sens = touchRead(TOUCH_PIN) - sBias;
    filterLP.input(sens);
    float filt = filterLP.output();
    filterLP2.input(filt);
    filt = filterLP2.output();
    if (filt < 10)
        filt = 0;

    if (filt > triggerOn)
    {
        isTouching = true;
    }
    if (filt < triggerOff)
    {
        isTouching = false;
    }
    //}

    TOUCH_STATE returnState = NONE;
    float returnValue = 0;
    // if state has changed
    if (isTouching != wasTouching)
    {
        Serial.print("isTouching has changed to ");
        Serial.println(isTouching);
        if (isTouching == true)
        { // TOUCH_DOWN
            // it's been timeElapsed since the last touch down
            returnState = TOUCH_DOWN;
        }
        else
        { // TOUCH_UP
            returnState = TOUCH_UP;
            returnValue = timeElapsed;
        }
        // reset the timer as state has changed
        timeElapsed = 0;
    }
    else
    {
        // report timeElapsed if we're touching, no need if we're not.
        if (isTouching == true)
        {
            returnState = TOUCH_ACTIVE;
            returnValue = timeElapsed;
        }
    }

    // @TODO (??) handle broadcasting touch level (for v0.9 iOS app not v1.0 app)
    return std::tuple<TOUCH_STATE, float>(returnState, returnValue);
}