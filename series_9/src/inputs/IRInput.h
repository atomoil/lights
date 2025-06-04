
#ifndef IRINPUT_HPP_INCLUDED
#define IRINPUT_HPP_INCLUDED

#include <Arduino.h>
#include "../init.h"
#include "LampMessage.h"

#include <IRremote.h>


// for COM-14865, newer remote
#define POWER 0x00FF629D
#define A 0x00FF22DD
#define B 0x00FF02FD
#define C 0x00FFC23D
#define UP 0x00FF9867
#define DOWN 0x00FF38C7
#define LEFT 0x00FF30CF
#define RIGHT 0x00FF7A85
#define SELECT 0x00FF18E7


class IRInput
{
private:
    decode_results results;
    IRrecv *irrecv;

public:
    IRInput();
    void setup();
    LampMessage loop();
    //std::tuple<TOUCH_STATE,float> loop(); // should be Actions loop(); but one thing at a time!
};

#endif