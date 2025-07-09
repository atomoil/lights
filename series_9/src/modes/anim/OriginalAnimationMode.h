#ifndef ANIM_MODE_HPP_INCLUDED
#define ANIM_MODE_HPP_INCLUDED

#include <Arduino.h>
#include "../../init.h"
#include "../../managers/LedManager.h"
#include "../../managers/PaletteManager.h"
#include "../../managers/AnimationManager.h"
#include "BaseAnimationMode.h"

#define ANIMATION_FRAME_RATE 60.0

// -- Dot State
struct LightDot {
  int column;
  int led;
  double currentValue;
  double increment;
  double minimumValue;
  double maximumValue;
  int colourId;
};

class OriginalAnimationMode: public BaseAnimationMode
{
private:
    LightDot lights[NUM_COLUMNS][NUM_LEDS];
    elapsedMillis frameMs;
    float frameSize;
    float currentAnimatingSpeed = 5000;

public:
    void setup();
    void restart();
    void loop();
    // void setAnimationSpeed(float newSpeed);
    // float getAnimationSpeed();
    OriginalAnimationMode(LEDManager *ledAttach, PaletteManager *palette, AnimationManager *animation);
};




#endif