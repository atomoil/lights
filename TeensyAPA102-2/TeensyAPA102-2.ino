// --- FOR TEENSYTOUCH PCB ONLY ---//
#include "FastLED.h"
int rVal, gVal, bVal;
#define NUM_LEDS 55 // 50 leds in brown one.
#define DATA_PIN 21
#define CLOCK_PIN 20
#define DAC_PIN A12 //sets sensitivity of touch chip high val = low sens.
CRGB leds[NUM_LEDS];
const int kLed = 13;
bool sw;  bool oldsw = true;  bool touch;
int ledChange[NUM_LEDS]; int ledValues[NUM_LEDS]; int ledTimings[NUM_LEDS];
int counter = 0;

void setup() {
  Serial.begin(9600);
  //analogWriteResolution(12); // 12 bit resolution
  analogWrite(DAC_PIN, 4095); //sensitivity
  pinMode(kLed, OUTPUT);
  pinMode(14, OUTPUT);
  pinMode(15, INPUT);
  FastLED.addLeds<APA102, DATA_PIN, CLOCK_PIN, BGR>(leds, NUM_LEDS);
  delay(500);
  digitalWrite(14, HIGH); // enable MTCH101 touch sens
}

void loop() {
  touch = digitalRead(15);
  if (touch != oldsw ) { // has touch changed?
    if (touch == false) (sw = !sw); // flip sw on high to low touch(i.e release)
    oldsw = touch;
  }

  if (sw == true && touch == false) {
   // Serial.println("ON");
    digitalWrite(kLed, HIGH);
    rVal = random(0, 255);
    gVal = random(0, 255);
    bVal = random(0, 255);

    for (int dot = 0; dot < NUM_LEDS; dot++)
    {
      leds[ dot ].setRGB( rVal, bVal, gVal );
      ledChange[ dot ] = (dot % 10) + 1;
      ledValues[ dot ] = 250;
      ledTimings[ dot ] = 1; //dot+1;
      delay(1);
    }
    delay(100);
    FastLED.show();
  }

  if (sw == false) {
     //Serial.println("OFF");
    digitalWrite(kLed, LOW);
    for (int dot = 0; dot < NUM_LEDS; dot++)
    {
      leds[ dot ] = CRGB::Black;
    }
    FastLED.show();
  }

  if (sw == true)( seq() );
  delay(10);
  //Serial.println(counter);
}
//-------------------------------------------//
void seq() {
  for (int dot = 0; dot < NUM_LEDS; dot++) {
    int currentVal = counter % ledTimings[dot];
    if (currentVal == 0) {
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
      CHSV color = CHSV( rVal, bVal, dotValue );

      leds[ dot ] = color;
      ledChange[ dot ] = dotChange;
      ledValues[ dot ] = dotValue;
    }
  }
   counter += 1;
   if (counter > 255)(counter = 0);
  FastLED.show();

}
