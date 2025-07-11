#include "LampOS.h"

LampOS::LampOS()
{
    leds = new LEDManager();
    palette = new PaletteManager();
    animation = new AnimationManager();

    bluetooth = new BluetoothInput();

    int cols_1[9] = {255, 0, 0, /* */ 0, 255, 0, /* */ 0, 0, 255};
    rgbMode = new ColourCyclingRGBMode(leds, 1500, float(1300), cols_1, 3);

    int cols_2[18] = {10, 20, 12,
                    180, 220, 180, 
                    20, 10, 12, 
                    220, 180, 180,
                    10, 10, 20, 
                    180, 180, 220 };
    touchdownCyclingMode = new ColourCyclingRGBMode(leds, 1000, float(1000), cols_2, 6);

    animationMode = new OriginalAnimationMode(leds, palette, animation);

    brightFadeInMode = new SetColourOnceMode(leds, INITIAL_TOUCH_DOWN_TIME, 200, 200, 200);
    brightFadeInMode->modeId = 1;
    brightFadeInMode->modeName = "bright";

    brightMode = new SetColourOnceMode(leds, 0, 255, 255, 255);
    brightMode->modeId = 1;
    brightMode->modeName = "bright";

    switchOffMode = new SetColourOnceMode(leds, 2000, 0, 0, 0);
    switchOffMode->modeId = 0;
    switchOffMode->modeName = "off";

    offMode = new SetColourOnceMode(leds, 0, 0, 0, 0);
    offMode->modeId = 0;
    offMode->modeName = "off";

    singleColourAnimatingMode = new SingleColourAnimatingMode(leds, 3000, 255, 255);

    colourWipeMode = new ColourWipeMode(leds, palette, animation);
    randomPixelMode = new RandomPixelMode(leds, palette, animation);
    movingDotsMode = new MovingDotsMode(leds, palette, animation);

    animationModes[0] = animationMode;
    animationModes[1] = colourWipeMode;
    animationModes[2] = randomPixelMode;
    animationModes[3] = movingDotsMode;

    lastActiveAnimationMode = animationMode;

#ifdef SUPPORTS_FFT

    audio = new AudioManager();
    fftBarsMode = new FFTBarsMode(leds, palette, audio);
    fftPulseMode = new FFTPulseMode(leds, palette, audio);

    fftModes[0] = fftBarsMode;
    fftModes[1] = fftPulseMode;

#endif

    // set the first mode
    //mode = rgbMode;
    //mode = colourWipeMode;
    //mode = randomPixelMode;
    //mode = movingDotsMode;
    mode = offMode;
};

void LampOS::setup()
{
#ifdef LAMP_OS_DEBUG
    delay(1000); // wait for Serial to be ready so we can debug stuff
#endif
    Serial.println("LampOS::setup start");
    frameSize = 1000.0 / 60;
    lampState = OFF;

    leds->setup();
    palette->setup();

    bluetooth->setup();

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
    movingDotsMode->setup();

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
        //
        LampMessage bleData = bluetooth->loop();
        processLampMessage(bleData);
        //

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
        sendStateOverBluetooth();
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
        sendStateOverBluetooth();
    }
    break;
    case LAMP_OFF:
    {
        lampState = OFF;
        mode = switchOffMode; // nicer than just going off immediately
        mode->restart();
        sendStateOverBluetooth();
    }
    break;
    case SET_ANIM_SPEED:
    {
        animation->setSpeed(lampMsg.number);
        bool modeIsAnimation = false;
        int i;
        for(i=0;i<COUNT_ANIMATION_MODES;i++){
            if (mode == animationModes[i]) {
                modeIsAnimation = true;
            }
        }
        if (!modeIsAnimation){
            lampState = ON;
            mode = lastActiveAnimationMode;
            mode->restart();
        }
        sendStateOverBluetooth();
    }
    break;
    case MULT_ANIM_SPEED:
    {
        float speed = animation->getSpeed();
        speed *= lampMsg.number;
        animation->setSpeed(speed);

        bool modeIsAnimation = false;
        int i;
        for(i=0;i<COUNT_ANIMATION_MODES;i++){
            if (mode == animationModes[i]) {
                modeIsAnimation = true;
            }
        }
        if (!modeIsAnimation){
            lampState = ON;
            mode = lastActiveAnimationMode;
            mode->restart();
        }
        sendStateOverBluetooth();
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
        sendStateOverBluetooth();
    }
    break;
    case SET_BRIGHTNESS:
    {
        leds->setBrightness(lampMsg.number);
        sendStateOverBluetooth();
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
        char message[110];
        palette->getPaletteAsPlCode(palette_char);
        sprintf(message, "<p=%s/>", palette_char);
        bluetooth->sendMessage(message);
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
        BaseMode *checkMode;
        char version_message[200];
        sprintf(version_message, "<v=%s/>", LAMP_HARDWARE_VERSION); //, anim_message, fft_message);
        
        char anim_message[100];
        for(int i=0;i<COUNT_ANIMATION_MODES;i++){
            checkMode = animationModes[i];
            if (i==0){
                sprintf(anim_message, "%i:%s", checkMode->modeId,checkMode->modeName.c_str());
            } else {
                sprintf(anim_message, "%s,%i:%s", anim_message,checkMode->modeId,checkMode->modeName.c_str());
            }
        }
        sprintf(version_message, "%s<anim=%s/>", version_message, anim_message);
        
#ifdef SUPPORTS_FFT
        char fft_message[100];
        for(int i=0;i<COUNT_FFT_MODES;i++){
            checkMode = fftModes[i];
            if (i==0){
                sprintf(fft_message, "%i:%s", checkMode->modeId,checkMode->modeName.c_str());
            } else {
                sprintf(fft_message, "%s,%i:%s", fft_message,checkMode->modeId,checkMode->modeName.c_str());
            }
        }
        sprintf(version_message, "%s<fft=%s/>", version_message, fft_message);
#endif

        //char version_message[80];
        //sprintf(version_message, "<v=%s/><anim=%s>%s", LAMP_HARDWARE_VERSION, anim_message, fft_message);
        Serial.print("GET_VERSION - sending '");
        Serial.print(version_message);
        Serial.println("'");
        bluetooth->sendMessage(version_message);
    }
    break;
    case GET_LEVELS:
    {
        sendStateOverBluetooth();
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
    case SET_MODE:
    {
        BaseMode *checkMode;
        BaseMode *foundMode;
        bool found = false;
        bool isAnimation = false;
        int i;
        int requestedId = int(lampMsg.number);
        for(i=0;i<COUNT_ANIMATION_MODES;i++) {
            checkMode = animationModes[i];
            if (checkMode->modeId == requestedId) {
                found = true;
                isAnimation = true;
                foundMode = checkMode;
            }
        }
#ifdef SUPPORTS_FFT
        for(i=0;i<COUNT_FFT_MODES;i++) {
            checkMode = fftModes[i];
            if (checkMode->modeId == requestedId) {
                found = true;
                foundMode = checkMode;
            }
        }
#endif
        if (found == true) {
            Serial.print("found mode:");
            Serial.println(foundMode->modeName);
            lampState = ON;
            mode = foundMode;
            if (isAnimation) lastActiveAnimationMode = (BaseAnimationMode*) foundMode;
            mode->restart();
            sendStateOverBluetooth();
        }
    }
    break;
#ifdef SUPPORTS_FFT
    case CYCLE_FFT_MODE:
    {
        BaseMode *checkMode;
        int currentModeId = -1;
        int i;
        for(i=0;i<COUNT_FFT_MODES;i++) {
            checkMode = fftModes[i];
            if (checkMode == mode) {
                currentModeId = i;
            }
        }
        currentModeId++;
        if (currentModeId >= COUNT_FFT_MODES) currentModeId = 0;
        mode = fftModes[currentModeId];
        lampState = ON;
        mode->restart();
        sendStateOverBluetooth();
    }
    break;
#endif // SUPPORTS_FFT
    case CYCLE_ANIM_MODE:
    {
        BaseMode *checkMode;
        int currentModeId = -1;
        int i;
        for(i=0;i<COUNT_ANIMATION_MODES;i++) {
            checkMode = animationModes[i];
            if (checkMode == mode) {
                currentModeId = i;
            }
        }
        if (currentModeId == -1) {
            mode = lastActiveAnimationMode;
        } else {
            currentModeId++;
            if (currentModeId >= COUNT_ANIMATION_MODES) currentModeId = 0;
            mode = animationModes[currentModeId];
            lastActiveAnimationMode = (BaseAnimationMode*) mode;
        }
        lampState = ON;
        mode->restart();
        sendStateOverBluetooth();
    }
    break;
    }
}

void LampOS::sendStateOverBluetooth() {
    char levels_message[180];
    sprintf(levels_message, "<s=%.2f/><b=%.3f/><md=%d:%s/>", 
                            animation->getSpeed(), 
                            leds->getBrightness(), 
                            mode->modeId, 
                            mode->modeName.c_str());
    bluetooth->sendMessage(levels_message);
}