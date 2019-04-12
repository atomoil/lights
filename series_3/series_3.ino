#include "init.h"
#include <EEPROM.h>

void setup() {
  Serial.begin(9600);  //setup of Serial module
  pinMode(ledPin, OUTPUT);
  //--- bluetooth serial
  // btSerial.begin(57600); 
  btSerial.begin(9600);  
  Serial.println("BT serial started at 57600"); // Serial.println("BT serial started at 9600");
    
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

  
  // create the light structs
  initData();

  Serial.print("There are ");
  Serial.print(NUM_LEDS);
  Serial.println(" LEDs");

  isTouch = false;
  oldTouch = isTouch;
  currentState = STATE_INACTIVE;
  
  getSavedPalette();

  Serial.println("started lamp");
  allLightsOn();
}


void loop() {
 
  String ssData = "";

  // useful debug!
  if (Serial.available()) {
    ssData = Serial.readString();
    
    Serial.print("got Message (Serial):");
    Serial.println(ssData);
  }
  
  if (btSerial.available() > 0) { //bluetooth serial
    ssData = btSerial.readString();

    Serial.print("got Message (BT):");
    Serial.println(ssData);
  }

  processMessages(ssData);

  getRemote();

  // --- touch stuff---//
  if (timeSw >= deltaSw) { // scan for touch  every deltaSw mS
    getTouch();
    timeSw = 0;
  }

  // --- update LED arrays 60mS ---//
  if (timeUpdate >= deltaUpdate){
    
     updateLightDots();

     FastLED.show();
     timeUpdate = 0;
  }
}//loop



//-------- Functions --------//

void getSavedPalette(){
  int savedPalette[5][2] = {};
  EEPROM.get(0,savedPalette);
  int paletteSize = sizeof(savedPalette);
  
  if (paletteSize > 0){ // only update if we have values
    for(int i=0;i<totalPalettes;i++){
      palette[i][0] = savedPalette[i][0];
      palette[i][1] = savedPalette[i][1];
    }
  }
}


void setHueSat(int indx, float h, float s){
  palette[indx][0] = convertHue(h);
  palette[indx][1] = convertSat(s);
}


void getTouch() {
  sens = touchRead(sensPin) - sBias;
  sens = abs(sens);
  filt = (filtAlpha * sens ) + ((1 - filtAlpha) * filt);

  if (filt > touchTriggerOn ) { // touching
    isTouch = true;
  }

  if (filt < touchTriggerOff ) { // not touching
    isTouch = false;
    holdCount = 0;
  }
  
  if (sendTouchValue == true){
    sendTouchSensitivity(filt);
  }

  if (isTouch != oldTouch && isTouch == true) { // flip switch if change and on low->high touch
    sw = !sw;
    digitalWrite(ledPin, sw); //  flash led on teensy
  }

  if (isTouch != oldTouch){
    Serial.print("[touch:");
    Serial.print(isTouch);
    Serial.print("/filt:");
    Serial.print(filt);
    Serial.print("]");
    nextTouchState(isTouch);
  }
  oldTouch = isTouch;

}

