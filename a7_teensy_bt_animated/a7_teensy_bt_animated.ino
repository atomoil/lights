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
void ledsOFF(){
     for (int dot = 0; dot < NUM_LEDS; dot++)
    {
      leds[ dot ] = CRGB::Black;
    }

  
}

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
      dotHasReachedLowestValue( dot );
    } else if ((dot.increment > 0) && dot.currentValue > dot.maximumValue){
      // if we've heading up and have hit the top
      dot.currentValue = dot.maximumValue;
      dot.increment = -abs( dot.increment );
      dotHasReachedHighestValue( dot );
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

void dotHasReachedLowestValue(LightDot dot){
  
}

void dotHasReachedHighestValue(LightDot dot){
  
}


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
    // mutate the state, mate...
    
    digitalWrite(ledPin, sw);
    // setLED();
  }

  if (isTouch != oldTouch){
    changeState();
  }
  oldTouch = isTouch;

  //  Serial.print(sens);
  //  Serial.print(" ");
  //  Serial.print(noise);
  //  Serial.print(" ");
  //  Serial.println(filt);
}

void changeState(){
  
  Serial.print("changeState from ");
  Serial.print( currentState );
  
  switch( currentState ) {
    case STATE_INACTIVE:
      allLightsBreathing();
      currentState = STATE_TOUCH_ON;
      break;
      
    case STATE_TOUCH_ON:
      if (timeElapsedInState < 1000){
        allLightsOn();
        currentState = STATE_ON_BRIGHT;
      } else {
        allLightsAnimating(timeElapsedInState - 1000);
        currentState = STATE_ON_ANIMATED;
      }
      break;
      
    case STATE_ON_BRIGHT:
    case STATE_ON_ANIMATED:
      allLightsFadeDown();
      currentState = STATE_TOUCH_OFF;
      break;
      
    case STATE_TOUCH_OFF:
      allLightsOff();
      currentState = STATE_INACTIVE;
      break;
    
  }
  
  timeElapsedInState = 0;
  
  Serial.print(" to ");
  Serial.println( currentState );

}

void allLightsOn(){
  for (int d = 0; d < NUM_LEDS; d++ ){
    LightDot dot = lights[ d ];
    dot.hue = 55;
    dot.sat = 255;
    dot.currentValue = 255.0;
    dot.increment = 0.0;
    dot.minimumValue = 255.0;
    dot.maximumValue = 255.0;
    lights[ d ] = dot;
  }
  Serial.println("[on]");
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
  Serial.println("[off]");
}

void allLightsFadeDown(){
  for (int d = 0; d < NUM_LEDS; d++ ){
    LightDot dot = lights[ d ];
    dot.increment = -8.0;
    dot.minimumValue = -10.0;
    dot.maximumValue = -1.0;
    lights[ d ] = dot;
  }
  Serial.println("[fade]");
}

void allLightsBreathing(){
  for (int d = 0; d < NUM_LEDS; d++ ){
    LightDot dot = lights[ d ];
    dot.hue = 125;
    dot.sat = 125;
    dot.currentValue = 125.0;
    dot.increment = 2.5;
    dot.minimumValue = 80.0;
    dot.maximumValue = 125.0;
    lights[ d ] = dot;
  }
  Serial.println("[breathing]");
}

void allLightsAnimating(float timeElapsedIn){

  float timeElapsed = timeElapsedIn / 50.0;
  float margin = (timeElapsed / 100.0);
  
  for (int d = 0; d < NUM_LEDS; d++ ){
    LightDot dot = lights[ d ];

    // set the initial colour
    int colourInPalette = (d % totalPalettes);
    dot.colourId = colourInPalette;
    dot.hue = palette[ colourInPalette ][ PALETTE_HUE ];
    dot.sat = palette[ colourInPalette ][ PALETTE_SATURATION ];
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
  Serial.println("[animating:");
  Serial.print(timeElapsed);
  Serial.print("/");
  float showNum = 1 / ((timeElapsed + ((2 % 10) * margin)) / deltaUpdate);
  Serial.print(showNum);
  Serial.println("]");
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

