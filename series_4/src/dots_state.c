
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