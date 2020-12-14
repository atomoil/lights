
//---------LEDS----------------//
#include "FastLED.h"
#define NUM_LEDS 16

#define DATA_PIN1 6
#define CLOCK_PIN1 7

#define DATA_PIN2 8
#define CLOCK_PIN2 9

#define DATA_PIN3 10
#define CLOCK_PIN3 11

#define DATA_PIN4 18
#define CLOCK_PIN4 19

#define DATA_PIN5 21
#define CLOCK_PIN5 20

#define DATA_PIN6 23
#define CLOCK_PIN6 22

CRGB leds[6][NUM_LEDS];

uint8_t hue = 0;
uint8_t bri = 255;

#include <IRremote.h>

//--------------IR------------------//
int RECV_PIN = 5;
IRrecv irrecv(RECV_PIN);
decode_results results;
bool leds_on = true;

/*
//for COM-11759 older remote
#define POWER 0x10EFD827 
#define A 0x10EFF807 
#define B 0x10EF7887
#define C 0x10EF58A7
#define UP 0x10EFA05F
#define DOWN 0x10EF00FF
#define LEFT 0x10EF10EF
#define RIGHT 0x10EF807F
#define SELECT 0x10EF20DF
*/

// for COM-14865, newer remote
#define POWER 0x00FF629D
#define A 0x00FF22DD
#define B 0x00FF02FD
#define C 0x00FFC23D
#define UP 0x00FF9867
#define DOWN 0x00FF38C7
#define LEFT 0x00FF30CF
#define RIGHT 0x00FF7A85
#define SELECT 0x10EF20DF
//#define SELECT 0x00FF18E7
