#include <Arduino.h>
#include "init.h"
#include <EEPROM.h>

#ifdef SUPPORTS_FFT
AudioInputAnalog         adc1(A0);
AudioAnalyzeFFT256       FFT;
AudioConnection          patchCord1(adc1, FFT);
#endif

// func def
int convertHue(float f);
int convertSat(float f);
void getSavedPalette();
void setHueSat(int indx, float h, float s);
void getTouch();
void processMessages(String ssData);
void getRemote();
void updateFFT();
void updateFFT_Bars();
void updateLightDots();
void updateFFT_Pulse();
void sendTouchSensitivity(int sens);
void nextTouchState(bool touchDown);
void allLightsFadeIn();
void allLightsFadeIn();
void allLightsFadeDown();
void allLightsOn();
void setAnimatingSpeed(float timeElapsedIn);
void allLightsOff();
LightDot dotHasReachedLowestValue(LightDot dot);
LightDot dotHasReachedHighestValue(LightDot dot);
LightDot resetPalette(LightDot dot);
void getFFT(int i, float n);
void getMaxLevel();
int valueForLED(float value, int number, int maxnum);
float realValueForLED(float value, int number, int maxnum);
void sendVersionOverBluetooth();
void sendLevelsOverBluetooth();
//////////////////// series_3

void setup() {
  Serial.begin(57600);  //setup of Serial module
  ssData.reserve(200);
  pinMode(ledPin, OUTPUT);
  //--- bluetooth serial
  btSerial.begin(57600);  
  Serial.println("BT serial started at 57600");
    
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

#ifdef SUPPORTS_FFT
  AudioMemory(4);
  FFT.averageTogether(8);
  FFT.windowFunction(AudioWindowHanning256);
#endif
  
  delay(500); // let power supply settle
  sBias = touchRead(sensPin); // DC offset, noise cal.
  Serial.println("started lamp");
  //allLightsOn(); // seems to interfere with DC offset noise cal above.(??)
}


void loop() {

  // useful debug!
  if (Serial.available()) {
    ssData = Serial.readString();
    stringComplete = true;
    
    Serial.print("got Message (Serial):");
    Serial.println(ssData);
  }

  // process one message at a time, hope there isn't a massive backlog :-/
  while (btSerial.available() && stringComplete == false) {
    // get the new byte:
    char inChar = (char)btSerial.read();
    // add it to the String:
    if (inChar == '\n') {
      stringComplete = true;
    } else {
      ssData += inChar;
    }
    Serial.print("BT: ");
    Serial.print(inChar);
    Serial.print(" > ");
    Serial.println(ssData);
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    
  }
  
  if (stringComplete) {
    Serial.print("got Message (BT):");
    Serial.println(ssData);
    processMessages(ssData);
    // clear the string:
    ssData = "";
    stringComplete = false;
  }

  getRemote();

  // --- touch stuff---//
  if (timeSw >= deltaSw) { // scan for touch  every deltaSw mS
    getTouch();
    timeSw = 0;
  }

  // --- update LED arrays 60mS ---//
  if (timeUpdate >= deltaUpdate){

    if (app_mode == MODE_REACTIVE){
       updateLightDots();
    }
#ifdef SUPPORTS_FFT
    else if (app_mode == MODE_FFT_BARS){
      updateFFT();
      updateFFT_Bars();
    } else if (app_mode == MODE_FFT_PULSE){
      updateFFT();
      updateFFT_Pulse();
    }
#endif

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
  filterLP.input( sens );
  filt = filterLP.output();
  filterLP2.input(filt);
  filt = filterLP2.output();
  if (filt < 10) (filt = 0);

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

//////////// dots_state


void nextTouchState(bool touchDown){
  
  Serial.print("changeState from ");
  Serial.print( currentState );
  
  switch( currentState ) {
    case STATE_INACTIVE:
      if (touchDown == true){
        allLightsFadeIn();
      }
      break;
      
    case STATE_TOUCH_INITIAL:
      allLightsOn();
      break;

    case STATE_TOUCH_ON:
      setAnimatingSpeed(timeElapsedInState);
      
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
  for (int c = 0; c < NUM_COLUMNS; c++){
    for (int d = 0; d < NUM_LEDS; d++ ){
      LightDot dot = lights[ c ][ d ];
      dot.hue = 125; // 125
      dot.sat = 0;  // 0
      dot.currentValue = 0.0;
      dot.increment = 15.0;
      dot.minimumValue = 200.0;
      dot.maximumValue = 200.0;
      lights[ c ][ d ] = dot;
    }
  }
  currentState = STATE_TOUCH_INITIAL;
  Serial.print("[fade:in]");
}

void allLightsBreathing(){
  // float maxBreathing = ((255.0*10) / float(NUM_LEDS));
  for (int c = 0; c < NUM_COLUMNS; c++){
    for (int d = 0; d < NUM_LEDS; d++ ){
      LightDot dot = lights[ c ][ d ];
      dot.hue = 125;
      dot.sat = 0;
      //dot.currentValue = 125.0;
      dot.increment = -abs(dot.increment);
      // dot.maximumValue = 125.0;
      dot.minimumValue = 20.0;
      // dot.currentValue = 255.0 - (float(d) * maxBreathing);
      lights[ c ][ d ] = dot;
    }
  }
  currentState = STATE_TOUCH_ON;
  timeElapsedInState = 0; // bit nasty :-/
  Serial.print("[breathing]");
}

void allLightsOn(){
  
  for (int c = 0; c < NUM_COLUMNS; c++){
    for (int d = 0; d < NUM_LEDS; d++ ){
      LightDot dot = lights[ c ][ d ];
      dot.hue = 255; // 55 is best value for the crank light
      dot.sat = 0; // 0
      dot.currentValue = 254.0;
      dot.increment = 0.0;
      dot.minimumValue = 254.0;
      dot.maximumValue = 254.0;
      lights[ c ][ d ] = dot;
    }
  }
  currentState = STATE_ON_BRIGHT;
  Serial.print("[on]");
}

// don't call this directly, use setAnimatingSpeed!!
void allLightsAnimating(float timeElapsedIn){
  float timeElapsed = timeElapsedIn / 500.0;
  float margin = (timeElapsed / 10.0);
  
  for (int c = 0; c < NUM_COLUMNS; c++){
    for (int d = 0; d < NUM_LEDS; d++ ){
      LightDot dot = lights[ c ][ d ];
  
      // set the initial colour
      int colourInPalette = (d % totalPalettes);
      dot.colourId = colourInPalette;
      dot.hue = palette[ dot.colourId ][ PALETTE_HUE ];
      dot.sat = palette[ dot.colourId ][ PALETTE_SATURATION ];
      // set initial current value - make some on, some off and some in between
      dot.currentValue = ((c % 3)+(d % 3)) * (255 / 6.0);
      // make increment vary 
      float durationToReach = timeElapsed + ((d % 10) * margin);
      dot.increment = 1 / (durationToReach / deltaUpdate);
      // set varying minimum and maximum
      dot.minimumValue = -210.0 - (((d+2) % 4) * 30 );
      dot.maximumValue = 255.0 + (((d+3) % 7) * 30 );
      /*
      Serial.print("dotValues:");
      Serial.print(dot.minimumValue);
      Serial.print(":");
      Serial.println(dot.maximumValue);
      */
      // reassign the dot to the array
      lights[ c ][ d ] = dot;
    }
  }
  currentState = STATE_ON_ANIMATED;
  Serial.print("[animating:");
  Serial.print(timeElapsed);
  Serial.print("/");
  float showNum = 1 / ((timeElapsed + ((2 % 10) * margin)) / deltaUpdate);
  Serial.print(showNum);
  Serial.println("]");
}

void setAnimatingSpeed(float timeElapsedIn){
  Serial.print("setAnimatingSpeed");
  Serial.println(timeElapsedIn);
  if (timeElapsedIn >= 150) {
    currentAnimatingSpeed = timeElapsedIn;
  } else {
    currentAnimatingSpeed = 150;
  }
  Serial.print("currentAnimatingSpeed");
  Serial.println(currentAnimatingSpeed);
  
  if (currentState != STATE_ON_ANIMATED){
    allLightsAnimating(currentAnimatingSpeed);
  } else {
    float timeElapsed = currentAnimatingSpeed / 500.0;
    float margin = (timeElapsed / 10.0);
  
    for (int c = 0; c < NUM_COLUMNS; c++){
      for (int d = 0; d < NUM_LEDS; d++ ){
        LightDot dot = lights[ c ][ d ];
        float durationToReach = timeElapsed + ((d % 10) * margin);
        dot.increment = 1 / (durationToReach / deltaUpdate);
        lights[ c ][ d ] = dot;
      }
    }
  }
}


void allLightsFadeDown(){
  for (int c = 0; c < NUM_COLUMNS; c++){
    for (int d = 0; d < NUM_LEDS; d++ ){
      LightDot dot = lights[ c ][ d ];
      dot.increment = -8.0;
      dot.minimumValue = -10.0;
      dot.maximumValue = -1.0;
      lights[ c ][ d ] = dot;
    }
  }
  currentState = STATE_TOUCH_OFF;
  Serial.print("[fade:out]");
}

void allLightsOff(){
  app_mode = MODE_REACTIVE;
  for (int c = 0; c < NUM_COLUMNS; c++){
    for (int d = 0; d < NUM_LEDS; d++ ){
      LightDot dot = lights[ c ][ d ];
      dot.currentValue = -1;
      dot.increment = 0.0;
      dot.minimumValue = -10.0;
      dot.maximumValue = -1.0;
      lights[ c ][ d ] = dot;
    }
  }
  currentState = STATE_INACTIVE;
  Serial.print("[off]");
}


void incAnimatingSpeed(float timeInc){
  float newSpeed = currentAnimatingSpeed + timeInc;
  setAnimatingSpeed(newSpeed);
}

void multAnimatingSpeed(float mult){
  float newSpeed = currentAnimatingSpeed * mult;
  setAnimatingSpeed(newSpeed);
}

void incDotBrightness(float inc){
  dotBrightness += inc;
  if (dotBrightness < 0.1){
    dotBrightness = 0.1;
  } else if (dotBrightness > 1.0){
    dotBrightness = 1.0;
  }
}

//////////// end: dots_state

//////////// dots_update

void updateLightDots(){
  //Serial.print(">> ");
  // loop through all of the dots
  for (int c = 0; c < NUM_COLUMNS; c++){
    for (int d = 0; d < NUM_LEDS; d++ ){
      LightDot dot = lights[ c ][ d ];
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
      lights[ c ][ d ] = dot;
      
      // apply the colour to the LED
      // value < 0 means off, greater than 255 is on 
      // keep colour within bounds as dot can go higher and lower (to stay on or off for longer)
      if (dot.currentValue > 0){
        int rawVal = min( 255, dot.currentValue );
        int val = int(float(rawVal) * dotBrightness);
        CHSV colour = CHSV( dot.hue, dot.sat, val );
        //CHSV colour = CHSV( 125, 255, val );
        leds[ c ][ dot.led ] = colour;
      } else {
        leds[ c ][ dot.led ] = CRGB::Black;
      }
    }
  }
  //Serial.println(". ");
}

void resetPaletteOnAllDots(){
  for (int c = 0; c < NUM_COLUMNS; c++){
    for (int d = 0; d < NUM_LEDS; d++ ){
      LightDot dot = lights[ c ][ d ];
      dot = resetPalette(dot);
      lights[ c ][ d ] = dot;
    }
  }
}

LightDot dotHasReachedLowestValue(LightDot dot){
  switch ( currentState ){
    case STATE_TOUCH_ON:
      // slow down the dot
      dot.increment = max( min(dot.increment - 0.5, 10.5), 2.0 ); // no more than 10.5, so it drops fast and then slows
      dot.sat = min(dot.sat + 10,255);

      break;
    case STATE_ON_ANIMATED:
      // switch to the next colour in the palette
      dot.colourId = dot.colourId + 1;
      if (dot.colourId >= totalPalettes){
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

LightDot resetPalette(LightDot dot){
  dot.hue = palette[ dot.colourId ][ PALETTE_HUE ];
  dot.sat = palette[ dot.colourId ][ PALETTE_SATURATION ];
  return dot;
}

//////////// end: dots_update

//////////// fft

#ifdef SUPPORTS_FFT


void updateFFT() {
  if (FFT.available())
  {
    getFFT( 0, FFT.read( 0, 2) );
    getFFT( 1, FFT.read( 3, 7) );
    getFFT( 2, FFT.read( 8, 19) );
    getFFT( 3, FFT.read( 20, 47) );
    getFFT( 4, FFT.read( 48, 110) );
    getFFT( 5, FFT.read( 111, 255) );
    getMaxLevel();
    // reportFFT();
  } else {
    Serial.println("FFT not ready");
  }
}

void getFFT(int i, float n) {
  float tmp = fftVals[i];
  if (tmp <= 0.01)(tmp = 0);

  if (n > tmp) // get peak value
  {
    fftVals[i] = n;
  } else {
    fftVals[i] = tmp * 0.95; //decay
  }
}

void getMaxLevel() {
  float n = 0.0;
  // get highest band level
  for (int i=0; i<6; i++){
    n = max(fftVals[i],n);
  }
  //Serial.print(n);
  //Serial.print(",");
  // 1 / level to get value to multiply bands with
  if (n > fft_max_band){
    fft_max_band = n;
  } else {
    fft_max_band = fft_max_band * 0.9;
    fft_max_band = max( fft_max_band, 0.005 ); // stop value getting too small / mult getting too large
  }
  //Serial.print(fft_max_band);
  //Serial.print(">");

  fft_mult = 1/fft_max_band;
}

void reportFFT() {
  for (int x=0; x < 6; x++)
  {
    Serial.print( fftVals[x]*100 );
    Serial.print(",");
  }
  Serial.print(">");
  Serial.print(fft_mult);
  
  Serial.println();
}

void updateFFT_Bars() { // fft "equaliser bars"
  for (int i = 0; i < NUM_LEDS; i++)
  {
    for( int x = 0; x < NUM_COLUMNS; x++) {
      int pal = fmod(((x*NUM_LEDS) + i)/3, 5.0); // group the LED Colours
      leds[x][i] = CHSV(palette[ pal ][ PALETTE_HUE ], palette[ pal ][ PALETTE_SATURATION ], valueForLED(fftVals[x % 4],i,NUM_LEDS));
    }
  }
}

void updateFFT_Pulse() { // fft grouped into bands
  int total_bands = 3;
  float number_in_band = NUM_LEDS / total_bands;
  for (int i = 0; i < NUM_LEDS; i++)
  {
    for( int x = 0; x < NUM_COLUMNS; x++) {
      int band = (i / number_in_band);
      int pal = (band + (x*NUM_COLUMNS)) % 5; //  % 5;
      leds[x][i] = CHSV(palette[ pal ][ PALETTE_HUE ], palette[ pal ][ PALETTE_SATURATION ], valueForLED(fftVals[band % 4],band,total_bands));
    }
  }
}

int valueForLED(float value, int number, int maxnum) {
  float retval = 0.1 + (realValueForLED(value, number, maxnum) * 0.9);
  return (retval * 255) * dotBrightness;
}

float realValueForLED(float value, int number, int maxnum) {
  if (value < 0.004) { // silence out low values
    return 0.0;
  }
  float retval = ((value * fft_mult) * maxnum) - number;
  if (retval >= 1) {
    return 1.0;
  } else if (retval < 0) {
    return 0.0;
  } else {
    return fmod(retval,1.0);
  }
}

#endif

//////////// end: fft

//////////// messages

void processMessages(String ssData) {
  
  //on
  if (ssData == "st:on") {
    allLightsOn();
  }

  if (ssData == "st:off") {
    allLightsOff();
  }

  if (ssData == "an:slow"){ // an is for animation
    setAnimatingSpeed(10000);
  } else if (ssData == "an:fast"){
    setAnimatingSpeed(100);
  } else if (ssData.startsWith("an")) { 
    char input[100];
    ssData.toCharArray(input,99);
    char *text = strtok(input,":");
    text = strtok(0,":");
    int duration = atoi(text);
    setAnimatingSpeed(duration);
  }

  if (ssData.startsWith("am")){ // am is for animation multiplier
    char input[100];
    ssData.toCharArray(input,99);
    char *text = strtok(input,":");
    text = strtok(0,":");
    float mult = atof(text);
    multAnimatingSpeed(mult);
    Serial.print("multAnimatingSpeed ");
    Serial.println(mult);
  }

  
  if (ssData.startsWith("br")){ // am is for brightness
    char input[100];
    ssData.toCharArray(input,99);
    char *text = strtok(input,":");
    text = strtok(0,":");
    float mult = atof(text);
    incDotBrightness(mult);
    Serial.print("incDotBrightness ");
    Serial.println(mult);
  }

  // pl:10:11:20:21:30:31:40:41:50:51:60:61
  // pl:10:100:20:100:30:100:40:100:50:100
  // 
  if (ssData.startsWith("pl")) { // pl is for palette
    char input[100];
    ssData.toCharArray(input,99);
    char *text = strtok(input,":");
    int col = 0;
    int i = 0;
    int row = 0;
    int elem = 0;
    while (text != 0 && i < totalPalettes*2){
      row = i / 2;
      elem = i % 2;
      text = strtok(0,":");
      col = atoi( text );
      if (elem == 0){
        palette[row][0] = convertHue(col);
      } else {
        palette[row][1] = convertSat(col);
      }
      Serial.print(row);
      Serial.print("/");
      Serial.print(elem);
      Serial.print(": ");
      Serial.println(col);
      i++;
    }
    resetPaletteOnAllDots();
    // save the palette to permanent storage
    EEPROM.put(0,palette);
  }

  if (ssData.startsWith("v:get")){
    Serial.println("v:get");
    sendVersionOverBluetooth();
  }

  if (ssData.startsWith("v:lvl")){
    Serial.println("v:lvl");
    sendLevelsOverBluetooth();

  }

  if (ssData.startsWith("d:sendsens:1")){
    sendTouchValue = true;
  } else if (ssData.startsWith("d:sendsens:0")){
    sendTouchValue = false;
  }

#ifdef SUPPORTS_FFT
  if (ssData.startsWith("md:0")){
    app_mode = MODE_REACTIVE;
    Serial.println("switching to MODE_REACTIVE");
  } else if (ssData.startsWith("md:1")){
    app_mode = MODE_FFT_BARS;
    Serial.println("switching to FFT_BARS");
  } else if (ssData.startsWith("md:2")){
    Serial.println("switching to FFT_PULSE");
    app_mode = MODE_FFT_PULSE;
  }
#endif

   
}

void sendVersionOverBluetooth(){
  btSerial.write("<v=");
  btSerial.write(hardware_version);
  btSerial.write("/>");
  btSerial.write(supports);
}

void sendLevelsOverBluetooth(){
  // send 2 message to allow more in the future
  //int br = (int) dotBrightness; // * 100;
  //int sp = (int) currentAnimatingSpeed * 100;
 /* btSerial.write("<s=");
  //serialFloatPrint(currentAnimatingSpeed);
  btSerial.write(sp);
  btSerial.write("/>"); */
  
  char buff[20];
  btSerial.write("<s=");
  dtostrf(currentAnimatingSpeed, 3, 2, buff);
  btSerial.write(buff);
  btSerial.write("/>");
  
  btSerial.write("<b=");
  dtostrf(dotBrightness, 5, 3, buff);
  btSerial.write(buff);
  btSerial.write("/>");
}


void serialFloatPrint(float f) {
  byte * b = (byte *) &f;
  Serial.print("f:");
  for(int i=0; i<4; i++) {
    
    byte b1 = (b[i] >> 4) & 0x0f;
    byte b2 = (b[i] & 0x0f);
    
    char c1 = (b1 < 10) ? ('0' + b1) : 'A' + b1 - 10;
    char c2 = (b2 < 10) ? ('0' + b2) : 'A' + b2 - 10;
    
    btSerial.print(c1);
    btSerial.print(c2);
  }
}

void sendTouchSensitivity(int sens){
  btSerial.print("<t=");
  btSerial.print(sens);
  btSerial.print("/>");
}


int convertHue(float f){
  return (f/360.0)*255.0;
}
int convertSat(float f){
  return (f/100.0)*255.0;
}

//////////// end: messages

//////////// remote_control

void getRemote() {
  if (irrecv.decode(&results))
  {
    if (results.value == POWER)
    {
      Serial.println("POWER");
      if (app_mode != MODE_REACTIVE){
        // we're in FFT mode
        allLightsOff();
      } else {
        // we're in REACTIVE mode
        if (currentState == STATE_ON_BRIGHT){
          allLightsOff();
        } else if (currentState == STATE_ON_ANIMATED){
          allLightsFadeDown();
        } else {
          allLightsOn();
        }
      }
    }
#ifdef SUPPORTS_FFT
    if (results.value == A)
    {
      Serial.println("A");
      app_mode = MODE_REACTIVE;
      multAnimatingSpeed( 1.0 ); // slight cheat to switch into STATE_ON_ANIMATED
    }
    if (results.value == B)
    {
      Serial.println("B");
      app_mode = MODE_FFT_BARS;
    }
    if (results.value == C)
    {
      Serial.println("C");
      app_mode = MODE_FFT_PULSE;
    }
#else
    if (results.value == A)
    {
      Serial.println("A");
      setAnimatingSpeed( 200 );
    }
    if (results.value == B)
    {
      Serial.println("B");
      setAnimatingSpeed( 5000 );
    }
    if (results.value == C)
    {
      Serial.println("C");
      setAnimatingSpeed( 20000 );
    }
#endif

    if (results.value == UP)
    {
      Serial.println("UP");
      // incAnimatingSpeed( 100 );
      multAnimatingSpeed( 1.25 );
    }
    if (results.value == DOWN)
    {
      Serial.println("DOWN");
      // incAnimatingSpeed( -100 );
      multAnimatingSpeed( 0.75 );
    }
    if (results.value == LEFT)
    {
      Serial.println("LEFT");
      incDotBrightness( -0.1 );
    }
    if (results.value == RIGHT)
    {
      Serial.println("RIGHT");
      incDotBrightness( 0.1 );
    }
    if (results.value == SELECT)
    {
      Serial.println("SELECT");
    }
    irrecv.resume();
  }
}

//////////// end: remote_control




