
// serial
#define serialUART Serial1 // hardware serial port to/from BLE chip
#define serialUSB Serial // USB serial  to/from mac

//-- onboard LED
const int ledPin = 13;

bool LED_ENABLE = false;

//touch filter
// -- Filter
#include <Filters.h>
FilterOnePole filterLP( LOWPASS, 1.0 ); //create a one pole (RC) lowpass filter
FilterOnePole filterLP2( LOWPASS, 1.0 );
float filt = 0;

//touch read
const int sensPin = 15; 
int sens = 0; 
int sBias = 0;
int touchTriggerOn = 1200;
int touchTriggerOff = 900;
bool oldTouch;
bool isTouch;

// Audio
#include <Audio.h>
AudioInputAnalog         adc1(A0);
AudioAnalyzePeak         peak1;
AudioConnection          patchCord1(adc1, peak1);
elapsedMillis fps;

//---------LEDS----------------//
#include "FastLED.h"
#define NUM_LEDS 15

#define DATA_PIN1 6
#define CLOCK_PIN1 7

#define DATA_PIN2 8
#define CLOCK_PIN2 9

#define DATA_PIN3 10
#define CLOCK_PIN3 11

#define DATA_PIN4 23//these two swapped with 6 on new board
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

// IR remote
//--------------IR------------------//
#include <IRremote.h>
int RECV_PIN = 5;
IRrecv irrecv(RECV_PIN);
decode_results results;

//for COM-11759 older remote
//#define POWER 0x10EFD827 
//#define A 0x10EFF807 
//#define B 0x10EF7887
//#define C 0x10EF58A7
//#define UP 0x10EFA05F
//#define DOWN 0x10EF00FF
//#define LEFT 0x10EF10EF
//#define RIGHT 0x10EF807F
//#define SELECT 0x10EF20DF

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
