#include "LampOS.h"

LampOS::LampOS()
{
    leds = new LEDManager();
    palette = new PaletteManager();
    animation = new AnimationManager();

    touch = new TouchInput(TOUCH_ON, TOUCH_OFF);
    bluetooth = new BluetoothInput();
    remoteControl = new IRInput();

    int cols_1[30] = {255, 0, 0, /* */ 0, 255, 0, /* */ 0, 0, 255};
    rgbMode = new ColourCyclingRGBMode(leds, 1500, float(1300), cols_1, 3);

    int cols_2[30] = {40, 55, 45, /* */ 240, 250, 245, /* */ 10, 10, 10, /* */ 240, 245, 250};
    touchdownCyclingMode = new ColourCyclingRGBMode(leds, 1000, float(2000), cols_2, 4);

    animationMode = new OriginalAnimationMode(leds, palette, animation);

    brightFadeInMode = new SetColourOnceMode(leds, INITIAL_TOUCH_DOWN_TIME, 200, 200, 200);

    brightMode = new SetColourOnceMode(leds, 0, 255, 255, 255);

    switchOffMode = new SetColourOnceMode(leds, 2000, 0, 0, 0);
    offMode = new SetColourOnceMode(leds, 0, 0, 0, 0);

    singleColourAnimatingMode = new SingleColourAnimatingMode(leds, 3000, 255, 255);

    colourWipeMode = new ColourWipeMode(leds, palette, animation);
    randomPixelMode = new RandomPixelMode(leds, palette, animation);

#ifdef SUPPORTS_FFT

    audio = new AudioManager();
    fftBarsMode = new FFTBarsMode(leds, palette, audio);
    fftPulseMode = new FFTPulseMode(leds, palette, audio);

#endif

    // set the first mode
    //mode = rgbMode;
    //mode = colourWipeMode;
    //mode = randomPixelMode;
    mode = offMode;
};

void LampOS::setup()
{
//#ifdef LAMP_OS_DEBUG
    delay(2000); // wait for Serial to be ready so we can debug stuff
//#endif
    Serial.println("LampOS::setup start");
    frameSize = 1000.0 / 60;
    lampState = OFF;

    leds->setup();
    palette->setup();

    touch->setup();
    bluetooth->setup();
    remoteControl->setup();

    // @TODO - copy the setupAll from Arduino the object orientated way
    rgbMode->setup();
    touchdownCyclingMode->setup();
    animationMode->setup();
    brightFadeInMode->setup();
    brightMode->setup();
    switchOffMode->setup();
    offMode->setup();
    Serial.println("offMode setup");
    singleColourAnimatingMode->setup();
    Serial.println("singleColourAnimatingMode setup");
    colourWipeMode->setup();
    Serial.println("colourWipeMode setup");
    randomPixelMode->setup();
    Serial.println("randomPixelMode setup");

#ifdef SUPPORTS_FFT

    Serial.println("FFT is supported!!");
    audio->setup();

    fftBarsMode->setup();
    fftPulseMode->setup();

#else
    Serial.println("FFT not supported");
#endif

    //debugTouchAmount = true;

    Serial.println("LampOS::setup complete");
}

void LampOS::loop()
{

    if (frameMs > frameSize)
    {
        frameMs = 0;
        // get inputs first
        std::tuple<TOUCH_STATE, float> touchData = touch->loop();
        processTouchData(touchData);
        //
        LampMessage bleData = bluetooth->loop();
        processLampMessage(bleData);
        //
        LampMessage irData = remoteControl->loop();
        processLampMessage(irData);

        if (debugTouchAmount == true)
        {
            float touchAmount = touch->getCurrentTouchAmount();
            char touch_message[80];
            sprintf(touch_message, "<t=%0.0f/>", touchAmount);
            bluetooth->sendMessage(touch_message);
        }
#ifdef LAMP_OS_DEBUG
        debugTick++;
        if (debugTick > 60)
        {
            Serial.println("LampOS::loop");
            debugTick = 0;
        }
#endif
    }

    mode->loop(); // update the mode after input but before leds
    leds->loop(); // update leds last
}

// based on inputs, possibly change mode
void LampOS::processTouchData(std::tuple<TOUCH_STATE, float> val)
{
    TOUCH_STATE touchState;
    float touchValue;
    std::tie(touchState, touchValue) = val; // get the output of this
    switch (touchState)
    {
    case TOUCH_DOWN:
    {
        if (lampState == ON)
        {
            lampState = TURNING_OFF;
            mode = switchOffMode;
            mode->restart();
        }
        else
        {
            lampState = TURNING_ON;
            mode = brightFadeInMode;
            mode->restart();
        }
    }
    break;
    case TOUCH_ACTIVE:
    {
        if (lampState == TURNING_ON)
        {

            //Serial.print("TOUCH DOWN for ");
            //Serial.println(touchValue);
            // only set this if it's not already set
            if (touchValue < INITIAL_TOUCH_DOWN_TIME && mode != brightFadeInMode)
            {
                Serial.println("TOUCH DOWN initial");
                mode = brightFadeInMode;
                mode->restart();
            }
            if (touchValue >= INITIAL_TOUCH_DOWN_TIME && mode != touchdownCyclingMode)
            {
                Serial.println("TOUCH DOWN cycling");
                mode = touchdownCyclingMode;
                mode->restart();
            }
        }
    }
    break;
    case TOUCH_UP:
    {
        Serial.print("TOUCH ended and lasted ");
        Serial.println(touchValue);
        if (lampState == TURNING_OFF)
        {
            lampState = OFF;
            mode = offMode;
            mode->restart();
        }
        else if (lampState == TURNING_ON)
        {
            lampState = ON;
            if (touchValue < INITIAL_TOUCH_DOWN_TIME)
            {
                mode = brightMode;
                mode->restart();
            }
            else
            {
                animation->setSpeed(touchValue);
                mode = animationMode;
                mode->restart();
            }
        }
    }
    break;
    case NONE:
        break;
    }
}

void LampOS::processLampMessage(LampMessage lampMsg)
{
    LampMessageType type = lampMsg.type;
    switch (type)
    {
    case LAMP_NONE:
        break;
    case LAMP_ON:
    {
        mode = brightFadeInMode;
        mode->restart();
        break;
    }
    case LAMP_TOGGLE_ON:
    {
        if (lampState == ON || lampState == TURNING_ON)
        {
            // switch off, but nice fade
            lampState = OFF;
            mode = switchOffMode;
            mode->restart();
        }
        else
        {
            lampState = ON;
            mode = brightFadeInMode;
            mode->restart();
        }
    }
    break;
    case LAMP_OFF:
    {
        lampState = OFF;
        mode = switchOffMode; // nicer than just going off immediately
        mode->restart();
    }
    break;
    case SET_ANIM_SPEED:
    {
        // @TODO make a shared maanager for animation speed!
        //animationMode->setAnimationSpeed(lampMsg.number);
        animation->setSpeed(lampMsg.number);
        if (mode != animationMode && mode != colourWipeMode && mode != randomPixelMode) // @TODO this is ugly, can we improve it?
        {
            lampState = ON;
            mode = animationMode;
            mode->restart();
        }
    }
    break;
    case MULT_ANIM_SPEED:
    {
        float speed = animation->getSpeed();
        speed *= lampMsg.number;
        animation->setSpeed(speed);
        if (mode != animationMode && mode != colourWipeMode && mode != randomPixelMode)
        {
            lampState = ON;
            mode = animationMode; // animationMode is the default animation - @TODO configure default animation!
            mode->restart();
        }
    }
    break;
    case INC_BRIGHTNESS:
    {
        Serial.println("INC_BRIGHTNESS");
        float dotBrightness = leds->getBrightness() + lampMsg.number;
        if (dotBrightness < 0.1)
        {
            dotBrightness = 0.1;
        }
        else if (dotBrightness > 1.0)
        {
            dotBrightness = 1.0;
        }
        leds->setBrightness(dotBrightness);
    }
    break;
    case SET_BRIGHTNESS:
    {
        leds->setBrightness(lampMsg.number);
    }
    break;
    case SET_PALETTE:
    {
        Serial.print("LampOS:processLampMessage: '");
        Serial.print(lampMsg.string);
        Serial.println("'");
        palette->setPaletteFromPlCode(lampMsg.string);
        // debugging:
        Serial.print("getPalette: ");
        char palette_char[100];
        palette->getPaletteAsPlCode(palette_char);
        Serial.println(palette_char);
    }
    break;
    case GET_PALETTE:
    {
        char palette_char[100];
        palette->getPaletteAsPlCode(palette_char);
        bluetooth->sendMessage(palette_char);
    }
    break;
    case SET_COLOUR:
    {
        singleColourAnimatingMode->updateColour(lampMsg.number, lampMsg.number2);
        mode = singleColourAnimatingMode;
        mode->restart();
    }
    break;
    case GET_VERSION:
    {
        char version_message[80];
        sprintf(version_message, "<v=%s/>%s", LAMP_HARDWARE_VERSION, LAMP_SUPPORTS);
        //Serial.print("GET_VERSION - sending '");
        //Serial.print(version_message);
        //Serial.println("'");
        bluetooth->sendMessage(version_message);
    }
    break;
    case GET_LEVELS:
    {
        char levels_message[80];
        sprintf(levels_message, "<s=%.2f/><b=%.3f>", animation->getSpeed(), leds->getBrightness());
        bluetooth->sendMessage(levels_message);
    }
    break;
    case DEBUG_SENSITIVITY:
        //
        if (lampMsg.number >= 1)
        {
            debugTouchAmount = true;
        }
        else
        {
            debugTouchAmount = false;
        }

        break;
    case FFT_MODE:
#ifdef SUPPORTS_FFT
    {
        int fft_mode = int(lampMsg.number);
        Serial.print("FFT Mode: ");
        Serial.println(fft_mode);
        switch (fft_mode)
        {
        case 0:
        {
            lampState = ON;
            mode = animationMode;
            mode->restart();
        }
        break;
        case 1:
        {
            lampState = ON;
            mode = fftBarsMode;
            mode->restart();
        }
        break;
        case 2:
        {
            lampState = ON;
            mode = fftPulseMode;
            mode->restart();
        }
        break;
        }
        Serial.println("FFT Mode set");
    }
#endif
    break;
    case CYCLE_FFT_MODE:
    {
        if (mode == fftBarsMode)
        {
            lampState = ON;
            mode = fftPulseMode;
            mode->restart();
        }
        else
        {
            lampState = ON;
            mode = fftBarsMode;
            mode->restart();
        }
    }
    break;
    case ANIM_MODE:
    {
        int anim_mode = int(lampMsg.number);
        if (anim_mode == 0)
        {
            lampState = ON;
            mode = animationMode;
            mode->restart();
        }
        else if (anim_mode == 1)
        {
            lampState = ON;
            mode = colourWipeMode;
            mode->restart();
        }
        else if (anim_mode == 2)
        {
            lampState = ON;
            mode = randomPixelMode;
            mode->restart();
        }
    }
    break;
    case CYCLE_ANIM_MODE:
    {
        if (mode == animationMode)
        {
            mode = colourWipeMode;
        }
        else if (mode == colourWipeMode)
        {
            mode = randomPixelMode;
        }
        else
        {
            mode = animationMode;
        }
        lampState = ON;
        mode->restart();
    }
    break;
    }
}