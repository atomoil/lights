#include "FastLED.h"
// How many leds in your strip?
#define NUM_LEDS 8

//const int DATA_PIN = 21; // v1 boards
//const int CLOCK_PIN = 20;
const int DATA_PIN = 6; //v1.1 6 strip boards
const int CLOCK_PIN = 7;

// Define the array of leds
CRGB leds[NUM_LEDS];

void setup() { 
       FastLED.addLeds<APA102, DATA_PIN, CLOCK_PIN, BGR>(leds, NUM_LEDS);
       LEDS.setBrightness(75);
for(int i = 0; i < NUM_LEDS; i++) {
         leds[0] = CRGB::Black;
}
  FastLED.show();
}

void loop() { 
   static uint8_t hue = 0;
   
for(int i = 0; i < NUM_LEDS; i++) {
    // Set the i'th led to red 
    leds[i] = CHSV(hue++, 255, 255);
    // Show the leds
    FastLED.show(); 

    delay(10);
  }
}

/////

