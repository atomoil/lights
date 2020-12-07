#include "init.h"

void setup() {    
  serialUSB.begin(57600); //USB to arduino monitor if needed
  
  FastLED.addLeds<APA102, DATA_PIN1, CLOCK_PIN1, BGR, DATA_RATE_MHZ(5)>(leds[0], NUM_LEDS);
  FastLED.addLeds<APA102, DATA_PIN2, CLOCK_PIN2, BGR, DATA_RATE_MHZ(5)>(leds[1], NUM_LEDS);
  FastLED.addLeds<APA102, DATA_PIN3, CLOCK_PIN3, BGR, DATA_RATE_MHZ(5)>(leds[2], NUM_LEDS);
  FastLED.addLeds<APA102, DATA_PIN4, CLOCK_PIN4, BGR, DATA_RATE_MHZ(5)>(leds[3], NUM_LEDS);
  FastLED.addLeds<APA102, DATA_PIN5, CLOCK_PIN5, BGR, DATA_RATE_MHZ(5)>(leds[4], NUM_LEDS);
  FastLED.addLeds<APA102, DATA_PIN6, CLOCK_PIN6, BGR, DATA_RATE_MHZ(5)>(leds[5], NUM_LEDS);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 3000); 
}

void loop() {
// LEDs
  if (timeSw >= deltaSw) {
    //SerialUSB.println(count);
    timeSw = 0;
    if (count == 5)(count = 1);
    
    if (count == 1)(ledRed());
    if (count == 2)(ledGreen());
    if (count == 3)(ledBlue());
    if (count == 4)(ledWhite());
        count++;
        
    FastLED.show();
    }
}

//-------------Functions -----------------//

void ledRed() {
  for (int i = 0; i < NUM_LEDS; i++)
  {
    leds[0][i] = CRGB(255, 0, 0);
    leds[1][i] = CRGB(255, 0, 0);
    leds[2][i] = CRGB(255, 0, 0);
    leds[3][i] = CRGB(255, 0, 0);
    leds[4][i] = CRGB(255, 0, 0);
    leds[5][i] = CRGB(255, 0, 0);
  }
}

void ledGreen() {
  for (int i = 0; i < NUM_LEDS; i++)
  {
    leds[0][i] = CRGB(0, 255, 0);
    leds[1][i] = CRGB(0, 255, 0);
    leds[2][i] = CRGB(0, 255, 0);
    leds[3][i] = CRGB(0, 255, 0);
    leds[4][i] = CRGB(0, 255, 0);
    leds[5][i] = CRGB(0, 255, 0);
  }
}

void ledBlue() {
  for (int i = 0; i < NUM_LEDS; i++)
  {
    leds[0][i] = CRGB(0, 0, 255);
    leds[1][i] = CRGB(0, 0, 255);
    leds[2][i] = CRGB(0, 0, 255);
    leds[3][i] = CRGB(0, 0, 255);
    leds[4][i] = CRGB(0, 0, 255);
    leds[5][i] = CRGB(0, 0, 255);
  }
}

void ledWhite() {
  for (int i = 0; i < NUM_LEDS; i++)
  {
    leds[0][i] = CRGB(255, 255, 255); //ok
    leds[1][i] = CRGB(255, 255, 255); //ok
    leds[2][i] = CRGB(255, 255, 255); //ok
    leds[3][i] = CRGB(255, 255, 255); // was 5
    leds[4][i] = CRGB(255, 255, 255); //ok
    leds[5][i] = CRGB(255, 255, 255);
  }
}
