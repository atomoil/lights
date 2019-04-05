


void processMessages(String ssData) {
  
  //on
  if (ssData == "st:on") {
    allLightsOn();
  }

  if (ssData == "st:off") {
    allLightsOff();
  }

  if (ssData == "an:slow"){
    allLightsAnimating(10000);
  }

  if (ssData == "an:fast"){
    allLightsAnimating(100);
  }

  if (ssData.startsWith("an")) { // an is for animation
    char input[100];
    ssData.toCharArray(input,99);
    char *text = strtok(input,":");
    text = strtok(0,":");
    int duration = atoi(text);
    allLightsAnimating(duration);
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
    // save the palette to permanent storage
    EEPROM.put(0,palette);
  }
   
}


int convertHue(float f){
  return (f/360.0)*255.0;
}
int convertSat(float f){
  return (f/100.0)*255.0;
}

