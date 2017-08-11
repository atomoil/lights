int rVal, gVal, bVal;
#include "FastLED.h"
// How many leds in your strip?
#define NUM_LEDS 55
// How sensitive should it be?
#define TOUCH_SENSITIVITY 4095


// For led chips like Neopixels, which have a data line, ground, and power, you just
// need to define DATA_PIN.  For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806 define both DATA_PIN and CLOCK_PIN
#define DATA_PIN 21
#define CLOCK_PIN 20
#define DAC_PIN A12 //sets sensitivity of touch chip high val = low sens.
#define BOARD_LED_PIN 13
#define ENABLE_TOUCH_PIN 14
#define READ_TOUCH_PIN 15

// actual lights logic...
#define TOUCH_DURATION_FOR_NORMAL 1000
#define TOUCH_DURATION_DIVIDE 100.0
#define TOUCH_DURATION_DAMPENING 50.0

#define MINIMUM_LIGHT_BRIGHTNESS -40.0

bool flag, oldFlag;
bool sw, animateLeds, notTouching = false;
// Define the array of leds
CRGB leds[NUM_LEDS];
float ledIncrement[NUM_LEDS];
float ledValues[NUM_LEDS];
int ledHowOftenToTrigger[NUM_LEDS];
int ledPaletteId[NUM_LEDS];

#define PALETTE_HUE 0
#define PALETTE_SATURATION 1

int palette[][2] = { {40,255}, {80,255}, {120,255}, {140,255}, {180,255} };
int totalPalettes = 5;

int counter = 0;
int touchDuration = 0;

void setup() {
  Serial.begin(9600);
  //analogWriteResolution(12); // 12 bit resolution
  analogWrite(DAC_PIN, TOUCH_SENSITIVITY); //sensitivity
  pinMode(BOARD_LED_PIN, OUTPUT);
  pinMode(ENABLE_TOUCH_PIN, OUTPUT);
  pinMode(READ_TOUCH_PIN, INPUT);
  FastLED.addLeds<APA102, DATA_PIN, CLOCK_PIN, BGR>(leds, NUM_LEDS);
  delay(500);
  digitalWrite(ENABLE_TOUCH_PIN, HIGH); // enable MTCH101 touch sens 
}

void loop() {
  notTouching = digitalRead(READ_TOUCH_PIN);
  

  if (notTouching == false) {
    Serial.println("ON");
    if (oldFlag == false){
      touchDuration = 0;
    } else {
      touchDuration++;
    }
    flag = true;
  } else {
    Serial.println("OFF");
    flag = false;
  }

  if (oldFlag != flag) {
    if (flag == true){  // touch down
      animateLeds = false; // stop the animation
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

        if (touchDuration < TOUCH_DURATION_FOR_NORMAL){
          
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
            ledIncrement[ dot ] = (((dot % 10) + 1)/min(1,(touchDuration/TOUCH_DURATION_DIVIDE)))/TOUCH_DURATION_DAMPENING;
            ledValues[ dot ] = 0;
            ledHowOftenToTrigger[ dot ] = (dot % 4) +1;
            ledPaletteId[ dot ] = (dot % totalPalettes);
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
      if (dotValue <= MINIMUM_LIGHT_BRIGHTNESS) {
        dotValue = MINIMUM_LIGHT_BRIGHTNESS;
        dotChange = abs( dotChange );
        // increment palette
        paletteId += 1;
        if (paletteId > (totalPalettes-1)){
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

