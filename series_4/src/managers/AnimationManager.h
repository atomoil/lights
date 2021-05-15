#ifndef ANIM_MANAGER_HPP_INCLUDED
#define ANIM_MANAGER_HPP_INCLUDED

#include <Arduino.h>

class AnimationManager {

private:
    float speed = 100.0;

public:
    void setSpeed(float speed);
    float getSpeed();
};

#endif