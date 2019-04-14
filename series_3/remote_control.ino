void getRemote() {
  if (irrecv.decode(&results))
  {
    if (results.value == POWER)
    {
      Serial.println("POWER");
      if (currentState == STATE_ON_BRIGHT){
        allLightsOff();
      } else if (currentState == STATE_ON_ANIMATED){
        allLightsFadeDown();
      } else {
        allLightsOn();
      }
    }
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
      dotBrightness -= 0.1;
      if (dotBrightness < 0.1){
        dotBrightness = 0.1;
      }
    }
    if (results.value == RIGHT)
    {
      Serial.println("RIGHT");
      dotBrightness += 0.1;
      if (dotBrightness > 1.0){
        dotBrightness = 1.0;
      }
    }
    if (results.value == SELECT)
    {
      Serial.println("SELECT");
    }
    irrecv.resume();
  }
}

