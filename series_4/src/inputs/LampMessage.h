#ifndef LAMP_MESSAGE_HPP_INCLUDED
#define LAMP_MESSAGE_HPP_INCLUDED


enum LampMessageType {
    LAMP_NONE,
    LAMP_ON, LAMP_OFF, LAMP_TOGGLE_ON, // nothing
    SET_ANIM_SPEED, MULT_ANIM_SPEED, // number
    INC_BRIGHTNESS, // number
    SET_PALETTE, // string / char
    GET_VERSION, GET_LEVELS, // nothing
    DEBUG_SENSITIVITY,  // boolean
    FFT_MODE, // number
};

struct LampMessage {
    LampMessageType type;
    float number;
    char* string;
};

#endif