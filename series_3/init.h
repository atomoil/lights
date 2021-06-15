//-- fastLED
#include "FastLED.h"
#include <stdio.h>
#include <string.h>

#define LAMP_S1_06_TURQUOISE 1
#define LAMP_S3_01_SAGGAR_DARK 2
#define LAMP_S3_02_SAGGAR_LIGHT 3
#define LAMP_S3_03_OBVARA 4
#define LAMP_S3_04_TALL_DARK 5
#define LAMP_S4_01_SANDY 6
#define LAMP_S4_02_RAKU_DARK 7
#define LAMP_S4_03_RAKU_TURQ 8
#define LAMP_S5_01_BLACK_YELLOW 9
#define LAMP_S5_02_BUTTERFLY  10
#define LAMP_S5_03_TURQ_WHITE 11
#define LAMP_S5_04_CAROL_BUTTERFLY 12
#define LAMP_TEST 13


#define LAMP_CURRENT LAMP_S5_03_TURQ_WHITE

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




// NOTE: Series 1 & 2 Lamps don't work with this codebase - please use previous codebase.
#if LAMP_CURRENT == LAMP_S1_06_TURQUOISE
const int touchTriggerOn = 700; // 700 // 1800
const int touchTriggerOff = 500; // 500
#define NUM_COLUMNS 1
const int NUM_LEDS = 55;
char hardware_version[] = "1.0";
#endif

//--Specific Lights
#if LAMP_CURRENT == LAMP_S3_03_OBVARA
const int touchTriggerOn = 700; // 700 // 1800
const int touchTriggerOff = 500; // 500
#define NUM_COLUMNS 6
const int NUM_LEDS = 13;
char hardware_version[] = "3.1";
#endif


#if LAMP_CURRENT == LAMP_S3_02_SAGGAR_LIGHT
const int touchTriggerOn = 640;
const int touchTriggerOff = 400;
#define NUM_COLUMNS 6
const int NUM_LEDS = 13;
char hardware_version[] = "3.1";
#endif


#if LAMP_CURRENT == LAMP_S3_04_TALL_DARK
const int touchTriggerOn = 700; // 700 // 1800
const int touchTriggerOff = 500; // 500
#define NUM_COLUMNS 6
const int NUM_LEDS = 16;
char hardware_version[] = "3.1";
#endif

#if LAMP_CURRENT == LAMP_S3_01_SAGGAR_DARK
const int touchTriggerOn = 700; // 700 // 1800
const int touchTriggerOff = 500; // 500
#define NUM_COLUMNS 6
const int NUM_LEDS = 13;
char hardware_version[] = "3.1";
#endif


#if LAMP_CURRENT == LAMP_S4_01_SANDY || LAMP_CURRENT == LAMP_S4_02_RAKU_DARK
const int touchTriggerOn = 700; // 700 // 1800
const int touchTriggerOff = 500; // 500
const int NUM_LEDS = 11;
#define NUM_COLUMNS 6
#define SUPPORTS_FFT 1
char hardware_version[] = "4.1";
#endif

#if LAMP_CURRENT == LAMP_S4_03_RAKU_TURQ
const int touchTriggerOn = 700; // 700 // 1800
const int touchTriggerOff = 500; // 500
const int NUM_LEDS = 12;
#define NUM_COLUMNS 6
#define SUPPORTS_FFT 1
char hardware_version[] = "4.1";
#endif



#if LAMP_CURRENT == LAMP_S5_01_BLACK_YELLOW
const int touchTriggerOn = 600;
const int touchTriggerOff = 400;
const int NUM_LEDS = 12;
#define NUM_COLUMNS 6
#define SUPPORTS_FFT 1
char hardware_version[] = "4.1";
#endif

#if LAMP_CURRENT == LAMP_S5_02_BUTTERFLY
const int touchTriggerOn = 500; // 700 // 1800
const int touchTriggerOff = 300; // 500
const int NUM_LEDS = 12;
#define NUM_COLUMNS 6
#define SUPPORTS_FFT 1
char hardware_version[] = "4.1";
#endif

#if LAMP_CURRENT == LAMP_S5_03_TURQ_WHITE
const int touchTriggerOn = 525; // 700 // 1800
const int touchTriggerOff = 400; // 500
const int NUM_LEDS = 12;
#define NUM_COLUMNS 6
#define SUPPORTS_FFT 1
char hardware_version[] = "4.1";
#endif

#if LAMP_CURRENT == LAMP_S5_04_CAROL_BUTTERFLY
const int touchTriggerOn = 300;
const int touchTriggerOff = 200;
const int NUM_LEDS = 11;
#define NUM_COLUMNS 6
#define SUPPORTS_FFT 1
char hardware_version[] = "4.1";
#endif

#if LAMP_CURRENT == LAMP_TEST
const int touchTriggerOn = 300;
const int touchTriggerOff = 200;
const int NUM_LEDS = 11;
#define NUM_COLUMNS 6
#define SUPPORTS_FFT 1
char hardware_version[] = "4.1";
#endif

#ifdef SUPPORTS_FFT
char supports[] = "<fft=1,2/>";
#else
char supports[] = "";
#endif

CRGB leds[NUM_COLUMNS][NUM_LEDS];


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

#define MODE_REACTIVE 0

#ifdef SUPPORTS_FFT

#include <Audio.h>

#include <math.h>
#define MODE_FFT_BARS 1
#define MODE_FFT_PULSE 2

float fft_max_band = 0.0;
float fft_mult = 1.0;
float fftVals[6];


#endif

int app_mode = MODE_REACTIVE;

//-- touchRead
const int sensPin = 15;
int sens = 0;
int sBias = 0;

//- Bluetooth serial data
String ssData = "";         // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete


//-- onboard LED
const int ledPin = 13;

//-- Bluetooth Serial port
#define btSerial Serial1
char sData;


// -- Filter
#include <Filters.h>
FilterOnePole filterLP( LOWPASS, 1.0 ); //create a one pole (RC) lowpass filter
FilterOnePole filterLP2( LOWPASS, 1.0 );
float filt = 0;

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

//
bool sendTouchValue = false;

float dotBrightness = 1.0;

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
float currentAnimatingSpeed;


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
