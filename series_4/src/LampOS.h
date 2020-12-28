#ifndef LAMP_OS_HPP_INCLUDED
#define LAMP_OS_HPP_INCLUDED

#define LAMP_OS_DEBUG

#include <Arduino.h>
#include "init.h"
#include "lamps.h"
#include "main.h"

#include "managers/LedManager.h"
#include "managers/AudioManager.h"
#include "managers/AnimationManager.h"

#include "inputs/TouchInput.h"
#include "inputs/BluetoothInput.h"
#include "inputs/IRInput.h"

#include "modes/OriginalAnimationMode.h"
#include "modes/SetColourOnceMode.h"
#include "modes/ColourCyclingRGBMode.h"
#include "modes/SingleColourAnimatingMode.h"
#include "modes/ColourWipeMode.h"
#include "modes/RandomPixelMode.h"

#ifdef SUPPORTS_FFT

#include "modes/FFTBarsMode.h"
#include "modes/FFTPulseMode.h"

#endif

#define INITIAL_TOUCH_DOWN_TIME 1500

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
    AnimationManager *animation;
    TouchInput *touch;
    BluetoothInput *bluetooth;
    IRInput *remoteControl;
    ColourCyclingRGBMode *rgbMode;
    ColourCyclingRGBMode *touchdownCyclingMode;
    OriginalAnimationMode *animationMode;
    SetColourOnceMode *brightFadeInMode;
    SetColourOnceMode *brightMode;
    SetColourOnceMode *switchOffMode;
    SetColourOnceMode *offMode;
    SingleColourAnimatingMode *singleColourAnimatingMode;
    ColourWipeMode *colourWipeMode;
    RandomPixelMode *randomPixelMode;

#ifdef SUPPORTS_FFT
    AudioManager *audio;
    FFTBarsMode *fftBarsMode;
    FFTPulseMode *fftPulseMode;
#endif

    BaseMode *mode;
    LampState lampState;
    elapsedMillis frameMs;
    float frameSize;
    boolean debugTouchAmount = false;
#ifdef LAMP_OS_DEBUG
    int debugTick = 5000;
#endif
    
    void processTouchData(std::tuple<TOUCH_STATE, float> val);
    void processLampMessage(LampMessage);

public:
    void setup();
    void loop();
    LampOS();
};

#endif