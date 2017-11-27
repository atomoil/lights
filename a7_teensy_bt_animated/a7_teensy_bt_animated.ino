#include "init.h"


void setup() {
  Wire.setSCL(16); Wire.setSDA(17); Wire.begin();
  Serial.begin(9600);  //setup of Serial module, 115200 bits/second
  btSerial.begin(9600); //bluetooth serial default 9600
  pinMode(ledPin, OUTPUT);
  filt = touchRead(sensPin);      //set filt for t=1
  sBias = touchRead(sensPin); nBias = touchRead(noisePin);
  FastLED.addLeds<APA102, DATA_PIN, CLOCK_PIN, BGR>(leds, NUM_LEDS);
  // create the light structs
  initData();

  isTouch = false;
  oldTouch = isTouch;
  currentState = STATE_INACTIVE;
}

void loop() {
  if (Serial.available() > 0) {
    sData = Serial.read();
  }

  String ssData;
  //sData = '';
  if (btSerial.available() > 0) { //bluetooth serial
    ssData = btSerial.readString();
    /*
    while(btSerial.available() > 0){
      sData += btSerial.read();
    }
    */
    Serial.print("got BT ");
    Serial.println(ssData);
  }

  //on
  if (ssData == "on") {
    allLightsOn();
  }

  if (ssData == "off") {
    allLightsOff();
  }

  if (ssData == "anim:slow"){
    allLightsAnimating(10000);
  }

  if (ssData == "anim:fast"){
    allLightsAnimating(100);
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

/*
  if (timeHold >= deltaHold) { //do stuff once a second
    

    
    if (isTouch){
      updateLEDS();
      holdCount++;
      Serial.print ("holding = ");
      Serial.println(holdCount); // for example
    }
    timeHold = 0;
    
  }
  */
  // --- update LED arrays 60mS ---//
  if (timeUpdate >= deltaUpdate){
    
    updateLightDots();

     FastLED.show();
     timeUpdate = 0;
  }

}//loop

//-------- Functions --------//

void updateLightDots(){
  //Serial.print(">> ");
  // loop through all of the dots
  for (int d = 0; d < NUM_LEDS; d++ ){
    LightDot dot = lights[ d ];
    // update the dot
    dot.currentValue = dot.currentValue + dot.increment;
    // check for hitting boundaries
    if ((dot.increment < 0) && (dot.currentValue < dot.minimumValue)){
      // if we're heading down and have hit the bottom
      dot.currentValue = dot.minimumValue;
      dot.increment = abs( dot.increment );
      // if we're animating, then recycle the dot
      dot = dotHasReachedLowestValue( dot );
    } else if ((dot.increment > 0) && dot.currentValue > dot.maximumValue){
      // if we've heading up and have hit the top
      dot.currentValue = dot.maximumValue;
      dot.increment = -abs( dot.increment );
      dot = dotHasReachedHighestValue( dot );
    }
    lights[ d ] = dot;
    
    // apply the colour to the LED
    // value < 0 means off, greater than 255 is on 
    // keep colour within bounds as dot can go higher and lower (to stay on or off for longer)
    if (dot.currentValue > 0){
      int val = min( 255, dot.currentValue );
      CHSV colour = CHSV( dot.hue, dot.sat, val );
      //CHSV colour = CHSV( 125, 255, val );
      leds[ dot.led ] = colour;
    } else {
      leds[ dot.led ] = CRGB::Black;
    }
  }
  //Serial.println(". ");
}

LightDot dotHasReachedLowestValue(LightDot dot){
  switch ( currentState ){
    case STATE_TOUCH_ON:
      // slow down the dot
      dot.maximumValue = max( dot.maximumValue - 20, 125.0 );
      dot.increment = max( min(dot.increment - 0.5, 10.5), 2.0 ); // no more than 10.5, so it drops fast and then slows
      dot.sat = min(dot.sat + 10,255);

      break;
    case STATE_ON_ANIMATED:
      // switch to the next colour in the palette
      dot.colourId = dot.colourId + 1;
      if (dot.colourId > totalPalettes){
        dot.colourId = 0;
      }
      dot.hue = palette[ dot.colourId ][ PALETTE_HUE ];
      dot.sat = palette[ dot.colourId ][ PALETTE_SATURATION ];
    
      break;
  }
  return dot;
}

LightDot dotHasReachedHighestValue(LightDot dot){
  switch ( currentState ){
    case STATE_TOUCH_INITIAL:
      allLightsBreathing();
      break;
  }
  return dot;
}


void getTouch() {
  sens = touchRead(sensPin) - sBias;
  sens = abs(sens);
  noise =  touchRead(noisePin) - nBias;
  filt = (filtAlpha * (sens - noise) ) + ((1 - filtAlpha) * filt);

  /*
  Serial.print("[filt:");
  Serial.print(filt);
  Serial.print("]");
  */
    
  if (filt > touchTriggerOn ) { // touching
    isTouch = true;
  }

  if (filt < touchTriggerOff ) { // not touching
    isTouch = false;
    holdCount = 0;
  }

  // this is old - Ian, what's the digitalWrite doing?
  if (isTouch != oldTouch && isTouch == true) { // flip switch if change and on low->high touch
    sw = !sw;
    // mutate the state, mate...
    
    digitalWrite(ledPin, sw); // what does this do?
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

void nextTouchState(boolean touchDown){
  
  Serial.print("changeState from ");
  Serial.print( currentState );
  
  switch( currentState ) {
    case STATE_INACTIVE:
      if (touchDown == true){
        allLightsFadeIn();
      }
      
      // allLightsBreathing();
      break;
    /*
    case STATE_TOUCH_ON:
      if (timeElapsedInState < 1000){
        allLightsOn();
      } else {
        allLightsAnimating(timeElapsedInState - 1000);
      }
      break;
     */
    case STATE_TOUCH_INITIAL:
      allLightsOn();
      break;

    case STATE_TOUCH_ON:
      allLightsAnimating(timeElapsedInState);
      break;
      
    case STATE_ON_BRIGHT:
    case STATE_ON_ANIMATED:
      allLightsFadeDown();
      
      break;
      
    case STATE_TOUCH_OFF:
      allLightsOff();
      break;
    
  }
  
  timeElapsedInState = 0;
  
  Serial.print(" to ");
  Serial.println( currentState );

}

void allLightsFadeIn(){
  for (int d = 0; d < NUM_LEDS; d++ ){
    LightDot dot = lights[ d ];
    dot.hue = 125; // 125
    dot.sat = 0;  // 0
    dot.currentValue = 0.0;
    dot.increment = 15.0;
    dot.minimumValue = 254.0;
    dot.maximumValue = 254.0;
    lights[ d ] = dot;
  }
  currentState = STATE_TOUCH_INITIAL;
  Serial.print("[fade:in]");
}

void allLightsBreathing(){
  for (int d = 0; d < NUM_LEDS; d++ ){
    LightDot dot = lights[ d ];
    dot.hue = 125;
    dot.sat = 0;
    //dot.currentValue = 125.0;
    dot.increment = -abs(dot.increment);
    dot.minimumValue = 80.0;
    //dot.maximumValue = 125.0;
    lights[ d ] = dot;
  }
  currentState = STATE_TOUCH_ON;
  timeElapsedInState = 0; // bit nasty :-/
  Serial.print("[breathing]");
}

void allLightsOn(){
  for (int d = 0; d < NUM_LEDS; d++ ){
    LightDot dot = lights[ d ];
    dot.hue = 255; // 55 is best value for the crank light
    dot.sat = 0; // 0
    dot.currentValue = 254.0;
    dot.increment = 0.0;
    dot.minimumValue = 254.0;
    dot.maximumValue = 254.0;
    lights[ d ] = dot;
  }
  currentState = STATE_ON_BRIGHT;
  Serial.print("[on]");
}


void allLightsAnimating(float timeElapsedIn){

  float timeElapsed = timeElapsedIn / 500.0;
  float margin = (timeElapsed / 10.0);
  
  for (int d = 0; d < NUM_LEDS; d++ ){
    LightDot dot = lights[ d ];

    // set the initial colour
    int colourInPalette = (d % totalPalettes);
    dot.colourId = colourInPalette;
    dot.hue = palette[ dot.colourId ][ PALETTE_HUE ];
    dot.sat = palette[ dot.colourId ][ PALETTE_SATURATION ];
    // set initial current value - make some on, some off and some in between
    dot.currentValue = (d % 4) * (255 / 4.0);
    // make increment vary 
    float durationToReach = timeElapsed + ((d % 10) * margin);
    dot.increment = 1 / (durationToReach / deltaUpdate);
    // set varying minimum and maximum
    dot.minimumValue = -10.0 - (((d+2) % 5) * 10 );
    dot.maximumValue = 255.0 + (((d+3) % 4) * 10 );
    // reassign the dot to the array
    lights[ d ] = dot;
  }
  currentState = STATE_ON_ANIMATED;
  Serial.println("[animating:");
  Serial.print(timeElapsed);
  Serial.print("/");
  float showNum = 1 / ((timeElapsed + ((2 % 10) * margin)) / deltaUpdate);
  Serial.print(showNum);
  Serial.print("]");
}


void allLightsFadeDown(){
  for (int d = 0; d < NUM_LEDS; d++ ){
    LightDot dot = lights[ d ];
    dot.increment = -8.0;
    dot.minimumValue = -10.0;
    dot.maximumValue = -1.0;
    lights[ d ] = dot;
  }
  currentState = STATE_TOUCH_OFF;
  Serial.print("[fade:out]");
}

void allLightsOff(){
  for (int d = 0; d < NUM_LEDS; d++ ){
    LightDot dot = lights[ d ];
    dot.currentValue = -1;
    dot.increment = 0.0;
    dot.minimumValue = -10.0;
    dot.maximumValue = -1.0;
    lights[ d ] = dot;
  }
  currentState = STATE_INACTIVE;
  Serial.print("[off]");
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

