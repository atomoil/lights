#include "BluetoothInput.h"

BluetoothInput::BluetoothInput() {}

void BluetoothInput::setup()
{
    //Serial.begin(57600);
    Serial1.begin(57600);

    ssData.reserve(200);
    Serial.println("BluetoothInput::setup");
}

LampMessage BluetoothInput::loop()
{
    if (!Serial1.available()) {
        //Serial1.begin(57600);
        if (showErrorOnce == true) {
            Serial.println("BLE not available!!! :-/");
            showErrorOnce = false;
        }
        
    }
    // process one message at a time, hope there isn't a massive backlog :-/
    while (BLE_Serial.available() && stringComplete == false)
    {
        // get the new byte:
        char inChar = (char)BLE_Serial.read();
        // add it to the String:
        if (inChar == '\n')
        {
            stringComplete = true;
        }
        else
        {
            ssData += inChar;
        }
        Serial.print("BT: ");
        Serial.print(inChar);
        Serial.print(" > ");
        Serial.println(ssData);
        // if the incoming character is a newline, set a flag so the main loop can
        // do something about it:
    
    }

    if (stringComplete)
    {
        Serial.print("got Message (BT):");
        Serial.println(ssData);
        return processMessage(ssData);
        // clear the string:
        ssData = "";
        stringComplete = false;
    }
    char* empty = "";
    return {LAMP_NONE, 0, empty};
}

LampMessage BluetoothInput::processMessage(String ssData)
{
    char* empty; // for returns with no value;
    //on
    if (ssData == "st:on")
    {
        return {LAMP_ON, 0, empty};
        //allLightsOn();
    }

    if (ssData == "st:off")
    {
        return {LAMP_OFF, 0, empty};
        // allLightsOff();
    }

    if (ssData == "an:slow")
    { // an is for animation
        return {SET_ANIM_SPEED, 10000, empty};
        //setAnimatingSpeed(10000);
    }
    else if (ssData == "an:fast")
    {
        return {SET_ANIM_SPEED, 100, empty};
        //setAnimatingSpeed(100);
    }
    else if (ssData.startsWith("an"))
    {
        char input[100];
        ssData.toCharArray(input, 99);
        char *text = strtok(input, ":");
        text = strtok(0, ":");
        float duration = atof(text);
        //setAnimatingSpeed(duration);
        return {MULT_ANIM_SPEED, duration, empty};
    }

    if (ssData.startsWith("am"))
    { // am is for animation multiplier
        char input[100];
        ssData.toCharArray(input, 99);
        char *text = strtok(input, ":");
        text = strtok(0, ":");
        float mult = atof(text);
        // multAnimatingSpeed(mult);

        return {MULT_ANIM_SPEED, mult, empty};
        //Serial.print("multAnimatingSpeed ");
        //Serial.println(mult);
    }

    if (ssData.startsWith("br"))
    { // am is for brightness
        char input[100];
        ssData.toCharArray(input, 99);
        char *text = strtok(input, ":");
        text = strtok(0, ":");
        float mult = atof(text);
        // incDotBrightness(mult);
        Serial.print("incDotBrightness ");
        Serial.println(mult);
        return {INC_BRIGHTNESS, mult, empty};
        
    }

    // pl:10:11:20:21:30:31:40:41:50:51:60:61
    // pl:10:100:20:100:30:100:40:100:50:100
    //
    if (ssData.startsWith("pl"))
    { // pl is for palette
        char input[100];
        ssData.toCharArray(input, 99);
        return {SET_PALETTE, 0, input};
        /*
        char *text = strtok(input, ":");
        int col = 0;
        int i = 0;
        int row = 0;
        int elem = 0;
        while (text != 0 && i < totalPalettes * 2)
        {
            row = i / 2;
            elem = i % 2;
            text = strtok(0, ":");
            col = atoi(text);
            if (elem == 0)
            {
                palette[row][0] = convertHue(col);
            }
            else
            {
                palette[row][1] = convertSat(col);
            }
            Serial.print(row);
            Serial.print("/");
            Serial.print(elem);
            Serial.print(": ");
            Serial.println(col);
            i++;
        }
        resetPaletteOnAllDots();
        // save the palette to permanent storage
        EEPROM.put(0, palette);
        */
    }

    if (ssData.startsWith("v:get"))
    {
        Serial.println("v:get");
        return {GET_VERSION, 0, empty};
        // sendVersionOverBluetooth();
    }

    if (ssData.startsWith("v:lvl"))
    {
        Serial.println("v:lvl");
        return {GET_LEVELS, 0, empty};
        // sendLevelsOverBluetooth();
    }

    if (ssData.startsWith("d:sendsens:1"))
    {
        return {DEBUG_SENSITIVITY, 1, empty};
        // sendTouchValue = true;
    }
    else if (ssData.startsWith("d:sendsens:0"))
    {
        return {DEBUG_SENSITIVITY, 0, empty};
        // sendTouchValue = false;
    }

#ifdef SUPPORTS_FFT
    if (ssData.startsWith("md:0"))
    {
        return {FFT_MODE, 0, empty}; //
    }
    else if (ssData.startsWith("md:1"))
    {
        return {FFT_MODE, 1, empty}; //
    }
    else if (ssData.startsWith("md:2"))
    {
        return {FFT_MODE, 2, empty}; //
    }
#endif

    // return a default
    
    return {LAMP_NONE, 0, empty};
}

void BluetoothInput::sendMessage(char* message) {
    Serial.print("BluetoothInput::sendMessage >> ");
    Serial.println(message);
    //Serial.println(message);
    BLE_Serial.write(message);
}