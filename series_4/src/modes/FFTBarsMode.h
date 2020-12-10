#include <Arduino.h>
#include "../init.h"
#include "../managers/LedManager.h"
#include "BaseMode.h"



class FFTBarsMode: public BaseMode
{
private:
    

public:
    void setup();
    void restart();
    void loop();
    FFTBarsMode(LEDManager *ledAttach);
};