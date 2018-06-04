#include "init.h"


void setup() {
  Serial.begin(9600);  //setup of Serial module
  pinMode(ledPin, OUTPUT);
  filt = touchRead(sensPin);      //set filt for t=1
  sBias = touchRead(sensPin); nBias = touchRead(noisePin);
}

  void loop(){ 
  // --- touch stuff---//
  if (timeSw >= deltaSw) { // scan for touch  every deltaSw mS
    getTouch();
    timeSw = 0;
  }


}//loop

//-------- Functions --------//

void getTouch() {
  sens = touchRead(sensPin) - sBias;
  sens = abs(sens);
  noise =  touchRead(noisePin) - nBias;
  filt = (filtAlpha * (sens - noise) ) + ((1 - filtAlpha) * filt);


  if (filt > 1200 ) { // touching
    isTouch = true;
  }

  if (filt  < 900 ) { // not touching
    isTouch = false;
    holdCount = 0;
  }

Serial.println(sens);
//Serial.println(noise);
//Serial.println(filt);
//Serial.println(isTouch);

  if (isTouch != oldTouch && isTouch == true) { // flip switch if change and on low->high touch
    sw = !sw;
    
    digitalWrite(ledPin, sw);
  }

  if (isTouch != oldTouch){
//    Serial.print("[touch:");
//    Serial.print(isTouch);
//    Serial.print("]");
  }
  oldTouch = isTouch;

}



