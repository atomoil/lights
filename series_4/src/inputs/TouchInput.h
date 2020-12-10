#include <Arduino.h>
#include "../init.h"
#include "../JonHub-Filters-master/Filters.h"
#include <tuple>

enum TOUCH_STATE { NONE, TOUCH_DOWN, TOUCH_ACTIVE, TOUCH_UP };

class TouchInput
{
private:
    float sBias;
    boolean isTouching;
    int triggerOn;
    int triggerOff;
    elapsedMillis timeElapsed;
    elapsedMillis nextFrameMs;
    FilterOnePole filterLP;
    FilterOnePole filterLP2;

public:
    TouchInput(int on, int off);
    void setup();
    std::tuple<TOUCH_STATE,float> loop(); // should be Actions loop(); but one thing at a time!
};

