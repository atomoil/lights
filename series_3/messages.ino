


void processMessages(String ssData) {
  
  //on
  if (ssData == "st:on") {
    allLightsOn();
  }

  if (ssData == "st:off") {
    allLightsOff();
  }

  if (ssData == "an:slow"){
    setAnimatingSpeed(10000);
  } else if (ssData == "an:fast"){
    setAnimatingSpeed(100);
  } else if (ssData.startsWith("an")) { // an is for animation
    char input[100];
    ssData.toCharArray(input,99);
    char *text = strtok(input,":");
    text = strtok(0,":");
    int duration = atoi(text);
    setAnimatingSpeed(duration);
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

  if (ssData.startsWith("d:sendsens:1")){
    sendTouchValue = true;
  } else if (ssData.startsWith("d:sendsens:0")){
    sendTouchValue = false;
  }
   
}

void sendVersionOverBluetooth(){
  btSerial.write("<v=3.0/>");
  /*
  if (btSerial.available()) {
    Serial.println("sendVersionOverBluetooth");
    btSerial.write("3.0");
  } else {
    Serial.println("no bluetooth to send to");
  }*/
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

