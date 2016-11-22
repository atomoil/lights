#include <ADCTouch.h>
int ref0, ref1;     //reference values to remove offset
int rVal, gVal, bVal;
#include "FastLED.h"
// How many leds in your strip?
#define NUM_LEDS 30

// For led chips like Neopixels, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806 define both DATA_PIN and CLOCK_PIN
#define DATA_PIN 11
#define CLOCK_PIN 12

bool flag, oldFlag;
bool sw = false;
// Define the array of leds
CRGB leds[NUM_LEDS];
int ledChange[NUM_LEDS];
int ledValues[NUM_LEDS];
int ledTimings[NUM_LEDS];
int counter = 0;

void setup() {

  Serial.begin(9600);
  ref0 = ADCTouch.read(A0, 500);
  ref1 = ADCTouch.read(A1, 500);
  FastLED.addLeds<APA102, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);
}

void loop() {
  int value0 = ADCTouch.read(A0);
  int value1 = ADCTouch.read(A1);
  value0 -= ref0;
  value1 -= ref1;

  if (value0 > 40) {
    flag = true;
  } else {
    flag = false;
  }

  if (oldFlag != flag) {
    if (flag == true) {
      sw = !sw;

      if (sw == true) {
        rVal = 40; //random(0, 255);
        gVal = 255; //random(0, 255);
        bVal = random(0, 255);

        for (int dot = 0; dot < NUM_LEDS; dot++)
        {
          leds[ dot ].setRGB( rVal, bVal, gVal );
          ledChange[ dot ] = (dot % 10) + 1;
          ledValues[ dot ] = 125;
          ledTimings[ dot ] = (dot % 4) +1;
        }
        FastLED.show();
      } else {
        for (int dot = 0; dot < NUM_LEDS; dot++)
        {
          leds[ dot ] = CRGB::Black;
        }
        FastLED.show();
      }
    }
  } else {
    // Serial.println("same");
  }

  oldFlag = flag;

  if (sw == true)( seq() );

  delay(1);
}//end loop

void seq() {
  for (int dot = 0; dot < NUM_LEDS; dot++) {
    int currentVal = counter % ledTimings[dot];
    if (currentVal == 0){
      int dotChange = ledChange[ dot ];
      int dotValue = ledValues[ dot ];
      //dotValue += dotChange;
      dotValue = dotValue + dotChange;
      if (dotValue <= 1) {
        dotValue = 1;
        dotChange = abs( dotChange );
      } else if (dotValue >= 255) {
        dotValue = 255;
        dotChange = -abs( dotChange );
      }
      //CRGB color = CRGB( rVal, bVal, gVal );
      //color.fadeToBlackBy( dotValue );
      CHSV color = CHSV( rVal, gVal, dotValue );

  
      leds[ dot ] = color;
  
      ledChange[ dot ] = dotChange;
      ledValues[ dot ] = dotValue;
    }
  }
  counter += 1;
  FastLED.show();

}


