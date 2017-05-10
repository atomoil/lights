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
bool sw, animateLeds = false;
// Define the array of leds
CRGB leds[NUM_LEDS];
float ledIncrement[NUM_LEDS];
float ledValues[NUM_LEDS];
int ledHowOftenToTrigger[NUM_LEDS];
int ledPaletteId[NUM_LEDS];

#define PALETTE_HUE 0
#define PALETTE_SATURATION 1

int palette[][2] = { {40,255}, {80,255}, {120,255} };

/*
#define LED_INCREMENT 0
#define LED_VALUE 1
#define LED_FRAME 2
#define LED_MINVALUE 3
#define LED_MAXVALUE 4
#define LED_PALETTE 5
//int ledSettings[][];
*/

int counter = 0;
int touchDuration = 0;

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
    if (oldFlag == false){
      touchDuration = 0;
    } else {
      touchDuration++;
    }
    flag = true;
  } else {
    flag = false;
  }

  if (oldFlag != flag) {
    if (flag == true){  // touch down
      animateLeds = false;
      if (sw == false){ // get ready to turn the lights on
        for (int dot = 0; dot < NUM_LEDS; dot++)
        {
            leds[ dot ].setRGB( 10, 10, 10 );
        }
        FastLED.show();
      } else { // turn the lights off
        for (int dot = 0; dot < NUM_LEDS; dot++)
        {
          leds[ dot ] = CRGB::Black;
        }
        FastLED.show();
      }
      
    } else if (flag == false) { // release
      sw = !sw;

      if (sw == true) { // actually turn the lights on!

        if (touchDuration < 5){
          
          // if they only briefly touch then turn light full on!
          for (int dot = 0; dot < NUM_LEDS; dot++)
          {
              leds[ dot ].setRGB( 255, 255, 255 );
          }
          FastLED.show();
          
        } else {
          animateLeds = true;
          //rVal = 40; //random(0, 255);
          //gVal = 255; //random(0, 255);
          //bVal = random(0, 255);
  
          for (int dot = 0; dot < NUM_LEDS; dot++)
          {
            //leds[ dot ].setRGB( rVal, bVal, gVal ); // this isn't right here - should set to initial colours!
            ledIncrement[ dot ] = ((dot % 10) + 1)/min(1,(touchDuration/100.0));
            ledValues[ dot ] = 125;
            ledHowOftenToTrigger[ dot ] = (dot % 4) +1;
            ledPaletteId[ dot ] = 0;
          }
          seq();

        }
        //FastLED.show();
      } else { // lights have been off already!
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

  if (animateLeds == true)( seq() );

  delay(1);
}//end loop

void seq() {
  for (int dot = 0; dot < NUM_LEDS; dot++) {
    int currentVal = counter % ledHowOftenToTrigger[dot];
    if (currentVal == 0){
      float dotChange = ledIncrement[ dot ];
      float dotValue = ledValues[ dot ];
      int paletteId = ledPaletteId[ dot ];
      //dotValue += dotChange;
      dotValue = dotValue + dotChange;
      if (dotValue <= 1) {
        dotValue = 1.0;
        dotChange = abs( dotChange );
        // increment palette
        paletteId += 1;
        if (paletteId > 2){
          paletteId = 0;
        }
        ledPaletteId[ dot ] = paletteId;
      } else if (dotValue >= 255) {
        dotValue = 255.0;
        dotChange = -abs( dotChange );
      }

      if (dotValue < 1){
        // allow for pixel going black for a while!
        
        leds[ dot ] = CRGB::Black;
      } else {

        int hue = palette[ paletteId ][ PALETTE_HUE ];
        int sat = palette[ paletteId ][ PALETTE_SATURATION ];
        int dotValueInt = dotValue;
        
        CHSV color = CHSV( hue, sat, dotValueInt );
        leds[ dot ] = color;
      }
  
      
      ledIncrement[ dot ] = dotChange;
      ledValues[ dot ] = dotValue;
    }
  }
  counter += 1;
  FastLED.show();

}


