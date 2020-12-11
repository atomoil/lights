#ifndef LAMP_OS_HPP_INCLUDED
#define LAMP_OS_HPP_INCLUDED

#include <Arduino.h>
#include "init.h"
#include "lamps.h"
#include "main.h"
#include "managers/LedManager.h"
#include "modes/ColourCyclingMode.h"
#include "modes/SingleColourMode.h"
#include "modes/AnimationMode.h"
#include "inputs/TouchInput.h"
#include "inputs/BluetoothInput.h"
#include "inputs/IRInput.h"

#define INITIAL_TOUCH_DOWN_TIME 1200

enum LampState
{
    OFF,
    TURNING_ON,
    ON,
    TURNING_OFF
};

class LampOS
{
private:
    LEDManager *leds;
    PaletteManager *palette;
    TouchInput *touch;
    BluetoothInput *bluetooth;
    IRInput *remoteControl;
    ColourCyclingMode *rgbMode;
    ColourCyclingMode *touchdownCyclingMode;
    AnimationMode *animationMode;
    SingleColourMode *brightFadeInMode;
    SingleColourMode *brightMode;
    SingleColourMode *switchOffMode;
    SingleColourMode *offMode;
    BaseMode *mode;
    LampState lampState;
    elapsedMillis frameMs;
    void processTouchData(std::tuple<TOUCH_STATE, float> val);
    void processLampMessage(LampMessage);
public:
    void setup();
    void loop();
    LampOS();
};

#endif