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
      incAnimatingSpeed( 100 );
    }
    if (results.value == DOWN)
    {
      Serial.println("DOWN");
      incAnimatingSpeed( -100 );
    }
    if (results.value == LEFT)
    {
      Serial.println("LEFT");
    }
    if (results.value == RIGHT)
    {
      Serial.println("RIGHT");
    }
    if (results.value == SELECT)
    {
      Serial.println("SELECT");
    }
    irrecv.resume();
  }
}

