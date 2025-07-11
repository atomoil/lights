#ifndef LAMP_OS_HPP_INCLUDED
#define LAMP_OS_HPP_INCLUDED

// #define LAMP_OS_DEBUG

#include <Arduino.h>
#include "init.h"
#include "lamps.h"
#include "main.h"

#include "managers/LedManager.h"
#include "managers/AnimationManager.h"

#include "inputs/BluetoothInput.h"

#include "modes/SetColourOnceMode.h"
#include "modes/ColourCyclingRGBMode.h"
#include "modes/SingleColourAnimatingMode.h"
#include "modes/anim/OriginalAnimationMode.h"
#include "modes/anim/ColourWipeMode.h"
#include "modes/anim/RandomPixelMode.h"
#include "modes/anim/MovingDotsMode.h"

#ifdef SUPPORTS_FFT
// as at 2025-06 the series 9 hardware does not support FFT, but it's planned to be added in the future

#include "managers/AudioManager.h"
#include "modes/FFTBarsMode.h"
#include "modes/FFTPulseMode.h"

#endif


#define INITIAL_TOUCH_DOWN_TIME 1500
#define COUNT_ANIMATION_MODES 4
#define COUNT_FFT_MODES 2

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
    BluetoothInput *bluetooth;
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
    MovingDotsMode *movingDotsMode;

    BaseAnimationMode *animationModes[COUNT_ANIMATION_MODES];
    BaseAnimationMode *lastActiveAnimationMode;

#ifdef SUPPORTS_FFT
    AudioManager *audio;
    FFTBarsMode *fftBarsMode;
    FFTPulseMode *fftPulseMode;
    BaseFFTMode *fftModes[COUNT_FFT_MODES];
#endif

    BaseMode *mode;
    LampState lampState;
    elapsedMillis frameMs;
    float frameSize;
    boolean debugTouchAmount = false;
#ifdef LAMP_OS_DEBUG
    int debugTick = 5000;
#endif
    
    void processLampMessage(LampMessage);
    void sendStateOverBluetooth();

public:
    void setup();
    void loop();
    LampOS();
};

#endif