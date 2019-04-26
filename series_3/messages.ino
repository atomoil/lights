


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
   
}

void sendVersionOverBluetooth(){
  btSerial.write("<v=3.0/>");
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
