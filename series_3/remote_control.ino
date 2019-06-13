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
