#include "BaseAnimationMode.h"

BaseAnimationMode::BaseAnimationMode (
    LEDManager *ledAttach,
    PaletteManager *paletteAttach,
    AnimationManager *animAttach) : BaseMode(ledAttach),
                                     palette(paletteAttach),
                                     animation(animAttach)
{

}
