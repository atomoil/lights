#ifndef ANIM_MANAGER_HPP_INCLUDED
#define ANIM_MANAGER_HPP_INCLUDED

#include <Arduino.h>

#define ANIMATION_FRAME_RATE 60.0

class AnimationManager {

private:
    float speed = 100.0;

public:
    void setSpeed(float speed);
    float getSpeed();
};

#endif