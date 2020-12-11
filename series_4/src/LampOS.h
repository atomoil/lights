#ifndef LAMP_OS_HPP_INCLUDED
#define LAMP_OS_HPP_INCLUDED

#include <Arduino.h>
#include "init.h"
#include "lamps.h"
#include "main.h"

#include "managers/LedManager.h"
#include "managers/AudioManager.h"

#include "inputs/TouchInput.h"
#include "inputs/BluetoothInput.h"
#include "inputs/IRInput.h"

#include "modes/ColourCyclingMode.h"
#include "modes/SingleColourMode.h"
#include "modes/AnimationMode.h"

#ifdef SUPPORTS_FFT

#include "modes/FFTBarsMode.h"
#include "modes/FFTPulseMode.h"

#endif

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

#ifdef SUPPORTS_FFT
    AudioManager *audio;
    FFTBarsMode *fftBarsMode;
    FFTPulseMode *fftPulseMode;
#endif

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