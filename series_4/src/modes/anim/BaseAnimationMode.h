#ifndef BASE_ANIM_MODE_HPP_INCLUDED
#define BASE_ANIM_MODE_HPP_INCLUDED

#include <Arduino.h>
#include "../BaseMode.h"
#include "../../managers/PaletteManager.h"
#include "../../managers/AnimationManager.h"

class BaseAnimationMode : public BaseMode
{
protected:
    PaletteManager *palette;
    AnimationManager *animation;

public:
    BaseAnimationMode(LEDManager *ledAttach, PaletteManager *paletteAttach, AnimationManager *animAttach);
};

#endif