#ifdef SUPPORTS_FFT

#include <Arduino.h>
#include "../init.h"
#include "BaseFFTMode.h"


class FFTPulseMode: public BaseFFTMode
{
public:
    void setup(); 
    void restart();
    void loop();
    FFTPulseMode(LEDManager *ledAttach, PaletteManager *paletteAttach, AudioManager *audioAttach);
};

#endif // SUPPORTS_FFT