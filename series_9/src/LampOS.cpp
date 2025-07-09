#include "LampOS.h"

LampOS::LampOS()
{
    // Initialize pointers to null first
    leds = nullptr;
    palette = nullptr;
    animation = nullptr;
    bluetooth = nullptr;
    rgbMode = nullptr;
    touchdownCyclingMode = nullptr;
    animationMode = nullptr;
    brightFadeInMode = nullptr;
    brightMode = nullptr;
    switchOffMode = nullptr;
    offMode = nullptr;
    singleColourAnimatingMode = nullptr;
    colourWipeMode = nullptr;
    randomPixelMode = nullptr;
    movingDotsMode = nullptr;
    
    // Create only essential objects to avoid memory issues
    leds = new LEDManager();
    palette = new PaletteManager();
    animation = new AnimationManager();
    
    // Create only essential mode for now
    offMode = new SetColourOnceMode(leds, 0, 0, 0, 0);
    if (offMode) {
        offMode->modeId = 0;
        offMode->modeName = "off";
    }
    
    mode = offMode;
}

void LampOS::setup()
{
    Serial.println("=== LampOS::setup START ===");
    Serial.println("Constructor completed, checking objects...");
    
    if (leds) Serial.println("leds: OK");
    else Serial.println("leds: NULL!");
    
    if (palette) Serial.println("palette: OK");
    else Serial.println("palette: NULL!");
    
    if (animation) Serial.println("animation: OK");
    else Serial.println("animation: NULL!");
    
    if (offMode) Serial.println("offMode: OK");
    else Serial.println("offMode: NULL!");
    
    if (mode) Serial.println("mode: OK");
    else Serial.println("mode: NULL!");
    
    Serial.print("Free heap: ");
    Serial.println(ESP.getFreeHeap());
    Serial.flush();
    
    frameSize = 1000.0 / 60;
    lampState = OFF;

    if (leds) {
        Serial.println("Calling leds->setup()...");
        delay(100); // Small delay to ensure serial output is flushed
        Serial.flush();
        leds->setup();
        Serial.println("leds->setup() completed");
    }
    
    if (palette) {
        Serial.println("Calling palette->setup()...");
        palette->setup();
        Serial.println("palette->setup() completed");
    }

    // Skip bluetooth for now since it might be causing issues
    // bluetooth->setup();

    if (offMode) {
        Serial.println("Calling offMode->setup()...");
        offMode->setup();
        Serial.println("offMode->setup() completed");
    }
    
    Serial.println("=== LampOS::setup COMPLETED ===");
    Serial.flush();
}

void LampOS::loop()
{
    static unsigned long lastHeartbeat = 0;
    static int heartbeatCount = 0;
    
    // Simple heartbeat every 5 seconds
    if (millis() - lastHeartbeat > 5000) {
        Serial.print("Heartbeat #");
        Serial.println(heartbeatCount++);
        Serial.flush();
        lastHeartbeat = millis();
    }

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