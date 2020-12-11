#include <Arduino.h>
#include "../init.h"
#include "BaseFFTMode.h"


class FFTBarsMode: public BaseFFTMode
{
private:
    

public:
    // void setup();
    // void restart();
    void loop();
    FFTBarsMode(LEDManager *ledAttach, PaletteManager *paletteAttach);
};