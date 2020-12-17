#include "BluetoothInput.h"

BluetoothInput::BluetoothInput() {}

void BluetoothInput::setup()
{
    //Serial.begin(57600);
    BLE_Serial.begin(57600);

    ssData.reserve(200);
}

LampMessage BluetoothInput::loop()
{
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
        /*
        Serial.print("BT: ");
        Serial.print(inChar);
        Serial.print(" > '");
        Serial.print(ssData);
        Serial.println("'");
        */
    }

    if (stringComplete)
    {
        Serial.print("BLE recieved: '");
        Serial.print(ssData);
        Serial.println("'");
        // clear the string:
        LampMessage msg = processMessage(ssData);
        ssData = "";
        stringComplete = false;
        return msg;
    }
    //char* empty = "";
    return {LAMP_NONE, 0, ""};
}

LampMessage BluetoothInput::processMessage(String msg)
{
    String empty = ""; // for returns with no value;
    //on
    if (msg == "st:on")
    {
        Serial.println("LAMP_ON!");
        return {LAMP_ON, 0, empty};
        //allLightsOn();
    }

    if (msg == "st:off")
    {
        return {LAMP_OFF, 0, empty};
        // allLightsOff();
    }

    if (msg == "an:slow")
    { // an is for animation
        return {SET_ANIM_SPEED, 10000, empty};
        //setAnimatingSpeed(10000);
    }
    else if (msg == "an:fast")
    {
        return {SET_ANIM_SPEED, 100, empty};
        //setAnimatingSpeed(100);
    }
    else if (msg.startsWith("an"))
    {
        char input[100];
        ssData.toCharArray(input, 99);
        char *text = strtok(input, ":");
        text = strtok(0, ":");
        float duration = float(atoi(text));
        Serial.println(duration);
        //setAnimatingSpeed(duration);
        return {SET_ANIM_SPEED, duration, empty};
    }

    if (msg.startsWith("am"))
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

    if (msg.startsWith("br"))
    { // br is for multiply brightness
        char input[100];
        ssData.toCharArray(input, 99);
        char *text = strtok(input, ":");
        text = strtok(0, ":");
        float mult = atof(text);
        return {INC_BRIGHTNESS, mult, empty};
        
    }

    if (msg.startsWith("bs"))
    {
        // bs is for set brightness
        char input[100];
        ssData.toCharArray(input, 99);
        char *text = strtok(input, ":");
        text = strtok(0, ":");
        float value = atof(text);
        Serial.println(value);
        return {SET_BRIGHTNESS, value, empty};
    }

    // pl:10:11:20:21:30:31:40:41:50:51:60:61
    // pl:10:100:20:100:30:100:40:100:50:100
    //
    if (msg.startsWith("pl"))
    { // pl is for palette
        char input[100];
        ssData.toCharArray(input, 99);
        Serial.print("PALETTE: ");
        Serial.println(input);
        return {SET_PALETTE, 0, ssData};
    }

    if (msg.startsWith("pg"))
    {
        return {GET_PALETTE, 0, empty};
    }

    if (msg.startsWith("cs"))
    {
        char input[100];
        ssData.toCharArray(input, 99);
        char *text = strtok(input, ":");
        text = strtok(0, ":");
        return {SET_COLOUR, 0, text};
    }

    if (msg.startsWith("v:get"))
    {
        Serial.println("v:get");
        return {GET_VERSION, 0, empty};
        // sendVersionOverBluetooth();
    }

    if (msg.startsWith("v:lvl"))
    {
        Serial.println("v:lvl");
        return {GET_LEVELS, 0, empty};
        // sendLevelsOverBluetooth();
    }

    if (msg.startsWith("d:sendsens:1"))
    {
        return {DEBUG_SENSITIVITY, 1, empty};
        // sendTouchValue = true;
    }
    else if (msg.startsWith("d:sendsens:0"))
    {
        return {DEBUG_SENSITIVITY, 0, empty};
        // sendTouchValue = false;
    }

#ifdef SUPPORTS_FFT
    if (msg.startsWith("md:0"))
    {
        return {FFT_MODE, 0, empty}; //
    }
    else if (msg.startsWith("md:1"))
    {
        return {FFT_MODE, 1, empty}; //
    }
    else if (msg.startsWith("md:2"))
    {
        return {FFT_MODE, 2, empty}; //
    }
#endif

    Serial.print("Message unknown '");
    Serial.print(msg);
    Serial.println("'");
    // return a default
    
    return {LAMP_NONE, 0, empty};
}

void BluetoothInput::sendMessage(char* message) {
    Serial.print("BluetoothInput::sendMessage >> ");
    Serial.println(message);
    //Serial.println(message);
    BLE_Serial.write(message);
}