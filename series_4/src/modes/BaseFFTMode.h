#ifndef BASE_FFT_MODE_HPP_INCLUDED
#define BASE_FFT_MODE_HPP_INCLUDED

#include <Arduino.h>
#include "BaseMode.h"
#include "../managers/PaletteManager.h"
#include "../managers/AudioManager.h"

class BaseFFTMode : public BaseMode
{
protected:
    PaletteManager *palette;
    AudioManager *audio;

public:
    void setup();
    void restart();
    BaseFFTMode(LEDManager *ledAttach, PaletteManager *paletteAttach, AudioManager *audioAttach);
};

#endif