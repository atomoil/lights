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
      allLightsAnimating( 100 );
    }
    if (results.value == B)
    {
      Serial.println("B");
      allLightsAnimating( 1000 );
    }
    if (results.value == C)
    {
      Serial.println("C");
      allLightsAnimating( 10000 );
    }
    if (results.value == UP)
    {
      Serial.println("UP");
      
    }
    if (results.value == DOWN)
    {
      Serial.println("DOWN");
      
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

