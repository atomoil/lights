#include "FastLED.h"
// How many leds in your strip?
#define NUM_LEDS 12

const int DATA_PIN1 = 6; //v1.1 6 strip boards.  on skt 2=clk, 3 = data
const int CLOCK_PIN1 = 7;

const int DATA_PIN2 = 8; 
const int CLOCK_PIN2 = 9;

const int DATA_PIN3 = 10; 
const int CLOCK_PIN3 = 11;

const int DATA_PIN4 = 18; 
const int CLOCK_PIN4 = 19;

const int DATA_PIN5 = 21; 
const int CLOCK_PIN5 = 20;

const int DATA_PIN6 = 23; 
const int CLOCK_PIN6 = 22;

// Define the array of leds
CRGB leds1[NUM_LEDS]; CRGB leds2[NUM_LEDS]; CRGB leds3[NUM_LEDS];
CRGB leds4[NUM_LEDS]; CRGB leds5[NUM_LEDS]; CRGB leds6[NUM_LEDS];

void setup() { 
       FastLED.addLeds<APA102, DATA_PIN1, CLOCK_PIN1, BGR>(leds1, NUM_LEDS);
       FastLED.addLeds<APA102, DATA_PIN2, CLOCK_PIN2, BGR>(leds2, NUM_LEDS);
       FastLED.addLeds<APA102, DATA_PIN3, CLOCK_PIN3, BGR>(leds3, NUM_LEDS);
       FastLED.addLeds<APA102, DATA_PIN4, CLOCK_PIN4, BGR>(leds4, NUM_LEDS);
       FastLED.addLeds<APA102, DATA_PIN5, CLOCK_PIN5, BGR>(leds5, NUM_LEDS);
       FastLED.addLeds<APA102, DATA_PIN6, CLOCK_PIN6, BGR>(leds6, NUM_LEDS);
       LEDS.setBrightness(75);
for(int i = 0; i < NUM_LEDS; i++) {
         leds1[0] = CRGB::Black;
         leds2[0] = CRGB::Black;
         leds2[0] = CRGB::Black;
         leds4[0] = CRGB::Black;
         leds5[0] = CRGB::Black;
         leds6[0] = CRGB::Black;
}
  FastLED.show();
}

void loop() { 
   static uint8_t hue = 0;
   
for(int i = 0; i < NUM_LEDS; i++) {
    hue++;
    leds1[i] = CHSV(hue, 255, 255);
    leds2[i] = CHSV(hue, 255, 255);
    leds3[i] = CHSV(hue, 255, 255);
    leds4[i] = CHSV(hue, 255, 255);
    leds5[i] = CHSV(hue, 255, 255);
    leds6[i] = CHSV(hue, 255, 255);
    FastLED.show(); 
    delay(10);
  }
}

/////

