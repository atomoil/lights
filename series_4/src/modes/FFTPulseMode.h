#include <Arduino.h>
#include "../init.h"
#include "BaseFFTMode.h"


class FFTPulseMode: public BaseFFTMode
{
private:
    
public:
    // void setup(); 
    // xsvoid restart();
    void loop();
    FFTPulseMode(LEDManager *ledAttach, PaletteManager *paletteAttach);
};