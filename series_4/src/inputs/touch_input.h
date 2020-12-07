#include <Arduino.h>
#include "init.h"
#include "JonHub-Filters-master/Filters.h"
#include <tuple>

enum STATE { NONE, TOUCH_DOWN, TOUCH_UP };

class TouchInput
{
private:
    float sBias;
    boolean isTouching;
    int triggerOn;
    int triggerOff;
    elapsedMillis timeElapsed;
    elapsedMillis checkMs;
    FilterOnePole filterLP;
    FilterOnePole filterLP2;
    //FilterOnePole filterLP(LOWPASS, 1.0); //create a one pole (RC) lowpass filter
    //FilterOnePole filterLP2(LOWPASS, 1.0);


public:
    TouchInput(int on, int off);
    void setup();
    std::tuple<STATE,float> loop(); // should be Actions loop(); but one thing at a time!
};

