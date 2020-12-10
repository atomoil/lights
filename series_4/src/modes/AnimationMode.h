#ifndef ANIM_MODE_HPP_INCLUDED
#define ANIM_MODE_HPP_INCLUDED

#include <Arduino.h>
#include "../init.h"
#include "../managers/LedManager.h"
#include "../managers/PaletteManager.h"
#include "BaseMode.h"

#define ANIMATION_FRAME_RATE 30

// -- Dot State
struct LightDot {
  int column;
  int led;
  float currentValue;
  float increment;
  float minimumValue;
  float maximumValue;
  int colourId;
  int hue;
  int sat;
};


class AnimationMode: public BaseMode
{
private:
    PaletteManager *palette;
    LightDot lights[NUM_COLUMNS][NUM_LEDS];
    elapsedMillis frameMs;
    float frameSize;
    float currentAnimatingSpeed = 5000;

public:
    void setup();
    void restart();
    void loop();
    void setAnimationSpeed(float newSpeed);
    float getAnimationSpeed();
    AnimationMode(LEDManager *ledAttach, PaletteManager *palette);
};




#endif