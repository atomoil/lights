#include "BaseFFTMode.h"

BaseFFTMode::BaseFFTMode(
    LEDManager *ledAttach,
    PaletteManager *paletteAttach,
    AudioManager *audioAttach) : BaseMode(ledAttach),
                                     palette(paletteAttach),
                                     audio(audioAttach)
{

}
