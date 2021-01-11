#include "AnimationManager.h"
    
    
void AnimationManager::setSpeed(float s){
    if (s < 10.0) {
        s = 10.0;
    }
    Serial.print("Animation::setSpeed > ");
    Serial.println(s);
    speed = s;
}

float AnimationManager::getSpeed(){
    return speed;
}