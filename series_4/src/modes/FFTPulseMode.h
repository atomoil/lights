#include <Arduino.h>
#include "../init.h"
#include "../managers/LedManager.h"
#include "BaseMode.h"



class FFTPulseMode: public BaseMode
{
private:
    

public:
    void setup();
    void restart();
    void loop();
    FFTPulseMode(LEDManager *ledAttach);
};