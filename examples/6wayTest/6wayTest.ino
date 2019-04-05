#include "init.h"
#include "remote.h"

void setup() {
  Serial.begin(9600);  //setup of Serial module
  pinMode(ledPin, OUTPUT);
  //--- bluetooth serial
    btSerial.begin(9600);  
    Serial.println("BT serial started at 9600");
    
  filt = touchRead(sensPin);      //set filt for t=1
  sBias = touchRead(sensPin); 

  irrecv.enableIRIn(); // Start the IR receiver
  
  FastLED.addLeds<APA102, DATA_PIN1, CLOCK_PIN1, BGR>(leds[0], NUM_LEDS);
  FastLED.addLeds<APA102, DATA_PIN2, CLOCK_PIN2, BGR>(leds[1], NUM_LEDS);
  FastLED.addLeds<APA102, DATA_PIN3, CLOCK_PIN3, BGR>(leds[2], NUM_LEDS);
  FastLED.addLeds<APA102, DATA_PIN4, CLOCK_PIN4, BGR>(leds[3], NUM_LEDS);
  FastLED.addLeds<APA102, DATA_PIN5, CLOCK_PIN5, BGR>(leds[4], NUM_LEDS);
  FastLED.addLeds<APA102, DATA_PIN6, CLOCK_PIN6, BGR>(leds[5], NUM_LEDS);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 4000);
}

  void loop(){ 
    getRemote(); // pressed button on remote -  in func.h, sets leds_on
    
  // --- touch stuff---//
  if (timeSw >= deltaSw) 
  { // scan for touch  every deltaSw mS, update leds
    getTouch();
    if(sw)(ledCycle());
    if(!sw)(ledOff());
    timeSw = 0;
      hue++;
    FastLED.show();
  }

// -- BLUETOOTH
   if (btSerial.available())
    {
        c = btSerial.read();
        Serial.write(c);
    }
    
}//loop

//-------------Functions -----------------//
void ledCycle(){ // simple pattern
 
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

void ledOff(){ // all off
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

void getTouch() {
  sens = touchRead(sensPin) - sBias;
  sens = abs(sens);
  filt = (filtAlpha * sens ) + ((1 - filtAlpha) * filt);

  if (filt > 700 ) 
  { // touching
    isTouch = true;
    holdCount++;
  }

  if (filt  < 500 ) 
  { // not touching
    isTouch = false;
    holdCount = 0;
  }

//Serial.println(sens);
//Serial.println(filt);
//Serial.println(isTouch);

  if (isTouch != oldTouch && isTouch == true) 
  { // flip switch if change and on low->high touch
    sw = !sw;
    digitalWrite(ledPin, sw);// flash led on teensy
  }

  oldTouch = isTouch;

}



