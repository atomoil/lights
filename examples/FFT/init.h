#include <Audio.h>
#include <math.h>

//---------LEDS----------------//
#include "FastLED.h"
#define NUM_LEDS 12 // tester has 12

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
uint8_t sat = 200;
int fftVals[8];

const int amp = 255;
