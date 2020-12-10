#include "LampOS.h"

LampOS::LampOS()
{
    leds = new LEDManager();
    palette = new PaletteManager();

    touch = new TouchInput(TOUCH_ON, TOUCH_OFF);
    bluetooth = new BluetoothInput();
    remoteControl = new IRInput();

    int cols_1[30] = {255, 0, 0, /* */ 0, 255, 0, /* */ 0, 0, 255};
    rgbMode = new ColourCyclingMode(leds, 1500, float(1300), cols_1, 3);
    
    int cols_2[30] = {5, 5, 5, /* */ 240, 250, 240, /* */ 10, 10, 10, /* */ 240, 240, 250};
    touchdownCyclingMode = new ColourCyclingMode(leds, 2000, float(2000), cols_2, 4);

    animationMode = new AnimationMode(leds, palette);

    brightFadeInMode = new SingleColourMode(leds, INITIAL_TOUCH_DOWN_TIME, 255, 255, 255);

    brightMode = new SingleColourMode(leds, 0, 255, 255, 255);

    switchOffMode = new SingleColourMode(leds, 5000, 0, 0, 0);
    offMode = new SingleColourMode(leds, 0, 0, 0, 0);

    // set the first mode
    //mode = rgbMode;
    //mode = animationMode;
    mode = offMode;
};

void LampOS::setup()
{
    //Serial.print("LampOS::setup");
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
}

void LampOS::loop()
{

    // get inputs first
    std::tuple<TOUCH_STATE, float> touchData = touch->loop();
    processTouchData(touchData);
    //
    LampMessage bleData = bluetooth->loop();
    processLampMessage(bleData);
    //
    LampMessage irData = remoteControl->loop();
    processLampMessage(irData);

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

void LampOS::processLampMessage(LampMessage message)
{
    LampMessageType type = message.type;
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
        animationMode->setAnimationSpeed(message.number);
        if (mode != animationMode)
        {
            lampState = ON;
            mode = animationMode;
            mode->restart();
        }
    }
    break;
    case MULT_ANIM_SPEED:
    {
        float speed = animationMode->getAnimationSpeed();
        speed *= message.number;
        animationMode->setAnimationSpeed(speed);
        if (mode != animationMode)
        {
            lampState = ON;
            mode = animationMode;
            mode->restart();
        }
    }
    break;
    case INC_BRIGHTNESS:
    {
        Serial.println("INC_BRIGHTNESS");
    }
    break;
    case SET_PALETTE:
    {
        palette->setPaletteFromPlCode(message.string);
    }
    break;
    case GET_VERSION:
    {
        char version_message[80];
        sprintf(version_message, "<v=%s/>%s", LAMP_HARDWARE_VERSION, LAMP_SUPPORTS);
        bluetooth->sendMessage(version_message);
    }
    break;
    case GET_LEVELS:
    {
        char levels_message[80];
        sprintf(levels_message, "<s=%.2f/><b=%.3f>", animationMode->getAnimationSpeed(), leds->getBrightness());

        bluetooth->sendMessage(levels_message);
    }
    break;
    case DEBUG_SENSITIVITY:
        break;
    case FFT_MODE:
        break;
    }
}