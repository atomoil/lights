#include "TouchInput.h"

TouchInput::TouchInput(int on, int off) : triggerOn(on),
                                          triggerOff(off)
{
}

void TouchInput::setup()
{
    delay(500);                   // let power supply settle
    filterLP = FilterOnePole(LOWPASS, 1.0);
    filterLP2 = FilterOnePole(LOWPASS, 1.0);
    isTouching = false;
    sBias = 0.0;
    prevTouchAmount = 0;
    sBias = touchRead(TOUCH_PIN); // DC offset, noise cal.
    Serial.print("TouchInput::setup sBias=");
    Serial.println(sBias);
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
    touchAmount = filt;
    float margin = 100.0;
    
    if (touchAmount > prevTouchAmount+margin ||
        touchAmount < prevTouchAmount-margin) {
        prevTouchAmount = touchAmount;
#ifdef LAMP_OS_DEBUG
        Serial.print("TouchInput::loop ");
        Serial.print(sBias);
        Serial.print(" ");
        Serial.print(sens);
        Serial.print(" ");
        Serial.print(touchAmount);
        Serial.println(" ");
#endif
    
    }

    TOUCH_STATE returnState = NONE;
    float returnValue = 0;
    // if state has changed
    if (isTouching != wasTouching)
    {
        Serial.print("isTouching has changed to ");
        Serial.print(isTouching);
        Serial.print(" ");
        Serial.print(sens);
        Serial.print(" ");
        Serial.print(filt);
        Serial.println(" ");
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

float TouchInput::getCurrentTouchAmount() {
    return touchAmount;
}

float TouchInput::getCurrentBias() {
    return sBias;
}