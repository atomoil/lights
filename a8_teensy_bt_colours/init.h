//-- fastLED
#include "FastLED.h"

#define LAMP_TALL_RAKU 0
#define LAMP_SHORT_RAKU 1
#define LAMP_TALL_TURQ 2

#define LAMP_CURRENT LAMP_SHORT_RAKU

// -----------------------------------

#define BOARD_v1 0
#define BOARD_v2_6_STRIP 1

#if LAMP_CURRENT == LAMP_TALL_TURQ
  #define BOARD_CURRENT BOARD_v1
  
#elif LAMP_CURRENT == LAMP_SHORT_RAKU
  #define BOARD_CURRENT BOARD_v1

#elif LAMP_CURRENT == LAMP_TALL_RAKU
  #define BOARD_CURRENT BOARD_v1
#endif




#if BOARD_CURRENT == BOARD_v1
const int DATA_PIN = 21; // v1 boards
const int CLOCK_PIN = 20;
const int sensPin = A9; //v1 board 23,22
const int noisePin = A8;
#endif

#if BOARD_CURRENT == BOARD_v2_6_STRIP
const int DATA_PIN = 6; //v1.1 6 strip boards
const int CLOCK_PIN = 7;
const int sensPin = 3;
const int noisePin = 2;
#endif


int sens = 0; 
int sBias = 0;
int noise = 0; 
int nBias = 0;

//-- RTC
#include <Wire.h>
#include <TimeLib.h>
#include <DS1307RTC.h>

const int NUM_LEDS = 55; // 97  tall grey/brown 59 new smallest grey crank 55 otehr small grey, turqoise 55

//--Specific Lights
#if LAMP_CURRENT == LAMP_TALL_RAKU
// Crank light
const int touchTriggerOn = 1800;
const int touchTriggerOff = 1400;
#endif

#if LAMP_CURRENT == LAMP_SHORT_RAKU
// Small Black & Yellow & Small Crank
const int touchTriggerOn = 1200;
const int touchTriggerOff = 800;
#endif
//
#if LAMP_CURRENT == LAMP_TURQUOISE
// Tall Turquoise and Yellow
//const int touchTriggerOn = 1400;
//const int touchTriggerOff = 1000;
//
// Ian's Tall Turquoise
const int touchTriggerOn = 800;
const int touchTriggerOff = 600;
#endif

CRGB leds[NUM_LEDS];
int ledChange[NUM_LEDS]; int ledValues[NUM_LEDS]; int ledTimings[NUM_LEDS];
int rVal, gVal, bVal; int counter = 0;

//-- onboard LED
const int ledPin = 13;

//-- Bluetooth Serial port
#define btSerial Serial1
char sData;


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

elapsedMillis timeElapsedInState;


// -- Colour Palettes

#define PALETTE_HUE 0
#define PALETTE_SATURATION 1

// default palette value
int palette[5][2] = { {40,255}, {80,255}, {120,255}, {140,255}, {180,255} };
int totalPalettes = sizeof(palette) / sizeof(palette[0]);



// -- App State
#define STATE_INACTIVE 0
#define STATE_TOUCH_INITIAL 1
#define STATE_TOUCH_ON 2
#define STATE_ON_BRIGHT 3
#define STATE_ON_ANIMATED 4
#define STATE_TOUCH_OFF 5
int currentState = STATE_INACTIVE;


// -- Dot State
struct LightDot {
  int led;
  float currentValue;
  float increment;
  float minimumValue;
  float maximumValue;
  int colourId;
  int hue;
  int sat;
};

LightDot lights[NUM_LEDS];

void initData(){

  int i;
  for (i = 0; i < NUM_LEDS; i = i + 1){
    // Serial.print ("LED: ");
    // Serial.println(i); // for example
    float minValue = 0.0;
    float maxValue = 255.0;
    int hue = 80;
    int sat = 255;
    LightDot light = {i, 0.0, 0.0, minValue, maxValue, 0, hue, sat };
    lights[i] = light;
  }

}

