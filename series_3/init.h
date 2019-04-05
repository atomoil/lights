//-- fastLED
#include "FastLED.h"

#define LAMP_S3_OBVARA 1
#define LAMP_S3_TALL_DARK 2
#define LAMP_S3_ 3
#define LAMP_SERIES_3 7

#define LAMP_CURRENT LAMP_S3_OBVARA

// -----------------------------------
//---------LEDS----------------//
#include "FastLED.h"

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

#define NUM_COLUMNS 6

//--Specific Lights
#if LAMP_CURRENT == LAMP_S3_OBVARA
// Crank light
const int touchTriggerOn = 700; //1800
const int touchTriggerOff = 500; //1400
const int NUM_LEDS = 13;
#endif


CRGB leds[6][NUM_LEDS];


//--------------IR------------------//
#include <IRremote.h>

int RECV_PIN = 5;
IRrecv irrecv(RECV_PIN);
decode_results results;

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

//-- touchRead
const int sensPin = 15; 
int sens = 0; 
int sBias = 0;



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

LightDot lights[NUM_COLUMNS][NUM_LEDS];

void initData(){
  for (int c = 0; c < NUM_COLUMNS; c++){
    for (int i = 0; i < NUM_LEDS; i = i + 1){
      // Serial.print ("LED: ");
      // Serial.println(i); // for example
      float minValue = 0.0;
      float maxValue = 255.0;
      int hue = 80;
      int sat = 255;
      LightDot light = {i, 0.0, 0.0, minValue, maxValue, 0, hue, sat };
      lights[c][i] = light;
    }
  }
}

