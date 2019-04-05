void getRemote() {
  if (irrecv.decode(&results))
  {
    if (results.value == POWER)
    {
      Serial.println("POWER");
    }
    if (results.value == A)
    {
      Serial.println("A");
      sw = true;
    }
    if (results.value == B)
    {
      Serial.println("B");
      sw = false;
    }
    if (results.value == C)
    {
      Serial.println("C");
    }
    if (results.value == UP)
    {
      Serial.println("UP");
      bri+= 16;
    }
    if (results.value == DOWN)
    {
      Serial.println("DOWN");
      bri-= 16;
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

