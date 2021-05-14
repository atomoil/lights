#ifndef LAMP_MESSAGE_HPP_INCLUDED
#define LAMP_MESSAGE_HPP_INCLUDED


enum LampMessageType {
    LAMP_NONE,
    LAMP_ON, LAMP_OFF, LAMP_TOGGLE_ON, // nothing
    SET_ANIM_SPEED, MULT_ANIM_SPEED, // number
    INC_BRIGHTNESS, SET_BRIGHTNESS,// number
    SET_PALETTE, // string / char
    GET_PALETTE, // nothing
    SET_COLOUR, // string (hue:sat) hmm
    GET_VERSION, GET_LEVELS, // nothing
    DEBUG_SENSITIVITY,  // boolean
    SET_MODE, // number
    CYCLE_ANIM_MODE, // nothing
    CYCLE_FFT_MODE // nothing
};

struct LampMessage {
    LampMessageType type;
    float number;
    float number2;
    String string;
};

#endif