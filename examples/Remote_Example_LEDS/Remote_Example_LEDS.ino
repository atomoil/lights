/*
Needs IRemote library - add from Arduino library manger if not installed.

  https://github.com/shirriff/Arduino-IRremote/

A turns leds on, B turns led off, up down arrow cycles through brightness.
Serial monitor shows buton name when pressed.
*/
#include "init.h"
#include "func.h"

void setup()
{
  Serial.begin(9600);
  irrecv.enableIRIn(); // Start the IR receiver

  FastLED.addLeds<APA102, DATA_PIN1, CLOCK_PIN1, BGR>(leds[0], NUM_LEDS);
  FastLED.addLeds<APA102, DATA_PIN2, CLOCK_PIN2, BGR>(leds[1], NUM_LEDS);
  FastLED.addLeds<APA102, DATA_PIN3, CLOCK_PIN3, BGR>(leds[2], NUM_LEDS);
  FastLED.addLeds<APA102, DATA_PIN4, CLOCK_PIN4, BGR>(leds[3], NUM_LEDS);
  FastLED.addLeds<APA102, DATA_PIN5, CLOCK_PIN5, BGR>(leds[4], NUM_LEDS);
  FastLED.addLeds<APA102, DATA_PIN6, CLOCK_PIN6, BGR>(leds[5], NUM_LEDS);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 3000);
}

void loop()
{
  getRemote(); // pressed button on remote -  in func.h, sets leds_on

  if (leds_on)
  {
      hue++;
      for (int i = 0; i < NUM_LEDS; i++)
      {
        leds[0][i] = CHSV(hue, 255, bri);
        leds[1][i] = CHSV(hue, 255, bri);
        leds[2][i] = CHSV(hue, 255, bri);
        leds[3][i] = CHSV(hue, 255, bri);
        leds[4][i] = CHSV(hue, 255, bri);
        leds[5][i] = CHSV(hue, 255, bri);
      }
  }

  if (!leds_on)
  {
      for (int i = 0; i < NUM_LEDS; i++)
      {
        leds[0][i] = CHSV(0, 0, 0);
        leds[1][i] = CHSV(0, 0, 0);
        leds[2][i] = CHSV(0, 0, 0);
        leds[3][i] = CHSV(0, 0, 0);
        leds[4][i] = CHSV(0, 0, 0);
        leds[5][i] = CHSV(0, 0, 0);
      }
  }
     FastLED.show();
      delay(10);
}
//---------------------------------------------------//


