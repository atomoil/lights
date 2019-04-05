#include "init.h"

void setup() {
  FastLED.addLeds<APA102, DATA_PIN1, CLOCK_PIN1, BGR>(leds[0], NUM_LEDS);
  FastLED.addLeds<APA102, DATA_PIN2, CLOCK_PIN2, BGR>(leds[1], NUM_LEDS);
  FastLED.addLeds<APA102, DATA_PIN3, CLOCK_PIN3, BGR>(leds[2], NUM_LEDS);
  FastLED.addLeds<APA102, DATA_PIN4, CLOCK_PIN4, BGR>(leds[3], NUM_LEDS);
  FastLED.addLeds<APA102, DATA_PIN5, CLOCK_PIN5, BGR>(leds[4], NUM_LEDS);
  FastLED.addLeds<APA102, DATA_PIN6, CLOCK_PIN6, BGR>(leds[5], NUM_LEDS);
  LEDS.setBrightness(75);
}

void loop() {
  static uint8_t hue = 0;

  for (int strip = 0; strip < 6; strip++)
  {
    hue++;
    for (int i = 0; i < NUM_LEDS; i++)
    {
      leds[strip][i] = CHSV(hue, 255, 255);
    }
    FastLED.show();
    delay(10);
  }
}

