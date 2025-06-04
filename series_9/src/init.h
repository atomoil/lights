#ifndef INIT_HPP_INCLUDED
#define INIT_HPP_INCLUDED



//-------LEDS-------

#define NUM_COLUMNS 6
#define COLUMN_MAPPING {1,3,5,4,2,0}

#define DATA_PIN1 6
#define CLOCK_PIN1 7

#define DATA_PIN2 8
#define CLOCK_PIN2 9

#define DATA_PIN3 10
#define CLOCK_PIN3 11

#define DATA_PIN4 23 //these two swapped with 6 on new board
#define CLOCK_PIN4 22

#define DATA_PIN5 21
#define CLOCK_PIN5 20

#define DATA_PIN6 18 //these two swapped with 4 on new board
#define CLOCK_PIN6 19

// end:-------LEDS-------

// TOUCH

#define TOUCH_PIN 15

// REMOTE
#define IR_RECV_PIN 5

#include "lamps.h"

#endif