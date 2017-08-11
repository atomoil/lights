//-- fastLED
#include "FastLED.h"
const int NUM_LEDS = 50;
const int DATA_PIN = 21;
const int CLOCK_PIN = 20;
CRGB leds[NUM_LEDS];
int ledChange[NUM_LEDS]; int ledValues[NUM_LEDS]; int ledTimings[NUM_LEDS];
int rVal, gVal, bVal; int counter = 0;

//-- RTC
#include <Wire.h>
#include <TimeLib.h>
#include <DS1307RTC.h>

//-- onboard LED
const int ledPin = 13;

//-- Bluetooth Serial port
#define btSerial Serial1
char sData;

//-- touchRead
const int sensPin = A9; int sens = 0; int sBias = 0;
const int noisePin = A8;  int noise = 0; int nBias = 0;

// -- Filter
float filtAlpha = 0.1; 
int filt = 0;       //initialization of EMA S
 
//-- Switch
int holdCount;
boolean sw = true ; boolean oldTouch; boolean isTouch;

// --Timers
const int deltaSw = 10; //check touch every 10mS
elapsedMillis timeSw;
const int deltaHold = 1000; // how long touching lamp (secs)
elapsedMillis timeHold;
const int deltaUpdate = 60;// update led array values
elapsedMillis timeUpdate;

// -- State
#define STATE_INACTIVE 0
#define STATE_TOUCH_ON 1
#define STATE_ON_BRIGHT 2
#define STATE_ON_ANIMATED 3
#define STATE_TOUCH_OFF 4
int currentState = STATE_INACTIVE;

struct LightDot {
  int led;
  float increment;
  float currentValue;
  float triggerFreq;
  float colourId;
};

LightDot lights[] = {
  {0, 0.1, 0, 3, 0},
  {1, 0.1, 0, 3, 0}
};




