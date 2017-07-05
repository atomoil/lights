#include "init.h"

void setup() {
  Wire.setSCL(16); Wire.setSDA(17); Wire.begin();
  Serial.begin(9600);  //setup of Serial module, 115200 bits/second
  btSerial.begin(9600); //bluetooth serial default 9600
  pinMode(ledPin, OUTPUT);
  filt = touchRead(sensPin);      //set filt for t=1
  sBias = touchRead(sensPin); nBias = touchRead(noisePin);
  FastLED.addLeds<APA102, DATA_PIN, CLOCK_PIN, BGR>(leds, NUM_LEDS);
}

void loop() {
  if (Serial.available() > 0) {
    sData = Serial.read();
  }

  if (btSerial.available() > 0) { //bluetooth serial
    sData = btSerial.read();
  }

  //on
  if (sData == '8') {
    sw = !sw;
    digitalWrite(ledPin, sw);
    setLED();
  }

  if (sData == '9') {
    getRTC();
  }
   sData = ' ';
  // --- touch stuff---//
  if (timeSw >= deltaSw) { // scan for touch  every deltaSw mS
    getTouch();
    timeSw = 0;
  }

  if (timeHold >= deltaHold) { //do stuff once a second
    if (isTouch){
     updateLEDS();
      holdCount++;
      Serial.print ("holding = ");
      Serial.println(holdCount); // for example
    }
    
    timeHold = 0;
  }
  
  // --- update LED arrays 60mS ---//
  if (timeUpdate >= deltaUpdate){
   
     
     if (sw == true){
     seqLEDS();
     }else{
    ledsOFF();
     }
     
     FastLED.show();
     timeUpdate = 0;
  }

}//loop

//-------- Functions --------//
void ledsOFF(){
     for (int dot = 0; dot < NUM_LEDS; dot++)
    {
      leds[ dot ] = CRGB::Black;
    }
}

void updateLEDS(){
    rVal = random(0, 255);
    gVal = random(0, 255);
    bVal = random(0, 255);
        for (int dot = 0; dot < NUM_LEDS; dot++)
    {
      leds[ dot ].setRGB( rVal, bVal, gVal );
      ledChange[ dot ] = (dot % 10) + 1;
      ledValues[ dot ] = 250;
      ledTimings[ dot ] = 1; //dot+1;
      //delay(1);
    }
   // delay(100);
   // FastLED.show();
  }//update`leds

void seqLEDS() {
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
 // FastLED.show();
//delay(10);
}//seq

void getTouch() {
  sens = touchRead(sensPin) - sBias;
  sens = abs(sens);
  noise =  touchRead(noisePin) - nBias;
  filt = (filtAlpha * (sens - noise) ) + ((1 - filtAlpha) * filt);


  if (filt > 1800 ) { // touching
    isTouch = true;
  }

  if (filt  < 1400 ) { // not touching
    isTouch = false;
    holdCount = 0;
  }

  if (isTouch != oldTouch && isTouch == true) { // flip switch if change and on low->high touch
    sw = !sw;
    digitalWrite(ledPin, sw);
    setLED();
  }
  oldTouch = isTouch;

  //  Serial.print(sens);
  //  Serial.print(" ");
  //  Serial.print(noise);
  //  Serial.print(" ");
  //  Serial.println(filt);
}

void setLED() {
  int i;
  if (sw == true) {

    for ( i = 0; i < NUM_LEDS; i++) {
      leds[i] = CRGB::Green;
    }
  } else {
    for ( i = 0; i < NUM_LEDS; i++) {
      leds[i] = CRGB::Black;
    }
  }
  FastLED.show();
}


void getRTC() {
  tmElements_t tm;

  if (RTC.read(tm)) {
    print2digits(tm.Hour);
    Serial.write(':');
    print2digits(tm.Minute);
    Serial.write(':');
    print2digits(tm.Second);
    Serial.print("  ");
    Serial.print(tm.Day);
    Serial.write('/');
    Serial.print(tm.Month);
    Serial.write('/');
    Serial.print(tmYearToCalendar(tm.Year));
    Serial.println();
  } else {
    if (RTC.chipPresent()) {
      Serial.println("The DS1307 is stopped.  Please run  SetTime");
      Serial.println();
    } else {
      Serial.println("DS1307 read error!  Please check the circuitry.");
      Serial.println();
    }
    delay(5000);
  }
}

void print2digits(int number) {
  if (number >= 0 && number < 10) {
    Serial.write('0');
  }
  Serial.print(number);
}

