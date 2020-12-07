// serial
#define serialUSB Serial

//---------LEDS----------------//
#include "FastLED.h"
#define NUM_LEDS 15

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

CRGB leds[6][NUM_LEDS];


// --Timers
const int deltaSw = 500;
uint8_t  count = 1;
elapsedMillis timeSw;