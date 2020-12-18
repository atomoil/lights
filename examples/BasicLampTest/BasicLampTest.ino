#include "init.h"

void setup() {
    AudioMemory(8);
    
  serialUART.begin(57600); //UART
  serialUSB.begin(57600); //USB
  
  pinMode(ledPin, OUTPUT);

  FastLED.addLeds<APA102, DATA_PIN1, CLOCK_PIN1, BGR, DATA_RATE_MHZ(5)>(leds[0], NUM_LEDS);
  FastLED.addLeds<APA102, DATA_PIN2, CLOCK_PIN2, BGR, DATA_RATE_MHZ(5)>(leds[1], NUM_LEDS);
  FastLED.addLeds<APA102, DATA_PIN3, CLOCK_PIN3, BGR, DATA_RATE_MHZ(5)>(leds[2], NUM_LEDS);
  FastLED.addLeds<APA102, DATA_PIN4, CLOCK_PIN4, BGR, DATA_RATE_MHZ(5)>(leds[3], NUM_LEDS);
  FastLED.addLeds<APA102, DATA_PIN5, CLOCK_PIN5, BGR, DATA_RATE_MHZ(5)>(leds[4], NUM_LEDS);
  FastLED.addLeds<APA102, DATA_PIN6, CLOCK_PIN6, BGR, DATA_RATE_MHZ(5)>(leds[5], NUM_LEDS);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 4000);

 irrecv.enableIRIn(); // Start the IR receiver
   
  // setup touch
  filt = touchRead(sensPin);      //set filt for t=1
  sBias = touchRead(sensPin); 
}

void loop() {
  // Serial data coms //
  // teensy USB back to Atom serial --> BLE
  if (serialUSB.available()) {
    char ch = serialUSB.read();
    if(ch =='1')(LED_ENABLE = true);
    if(ch =='0')(LED_ENABLE = false);
    serialUART.print(ch);
  }


  // From atom BLE --> serial to Teensy
  if (serialUART.available()) {
    char ch = serialUART.read();
    if(ch =='1')(LED_ENABLE = true);
    if(ch =='0')(LED_ENABLE = false);
    serialUSB.print(ch); // send to Arduino serial monitor idf you like
  }
  // audio peak
    if (fps > 24) {
    if (peak1.available()) {
      fps = 0;
      int monoPeak = peak1.read() * 100.0;
      if(monoPeak > 20) 
      {
        Serial.print("AUDIO: ");
        Serial.println(monoPeak);
      }
//       if(monoPeak > 80) 
//       {
//        LED_ENABLE = !LED_ENABLE;
//       }
    }
  }

// LEDs
  if (timeSw >= deltaSw) {
     getRemote();
     getTouch();
    timeSw = 0;
    if (count == 5)(count = 1);
    
    if (!LED_ENABLE){
        ledOFF();
    }else{
    if (count == 1)(ledRed());
    if (count == 2)(ledGreen());
    if (count == 3)(ledBlue());
    if (count == 4)(ledWhite());

    if (count == 5) (ledOne());
    if (count == 6) (ledTwo());
    if (count == 7) (ledThree());
    if (count == 8) (ledFour());
    if (count == 9) (ledFive());
    if (count == 10) (ledSix());
     //SerialUSB.println(count);
    }
    
    count++;
    FastLED.show();
  }

}//loop

//-------------Functions -----------------//
void ledOFF() {
  for (int i = 0; i < NUM_LEDS; i++)
  {
    leds[0][i] = CRGB(0, 0, 0);
    leds[1][i] = CRGB(0, 0, 0);
    leds[2][i] = CRGB(0, 0, 0);
    leds[3][i] = CRGB(0, 0, 0);
    leds[4][i] = CRGB(0, 0, 0);
    leds[5][i] = CRGB(0, 0, 0);
  }
}

void ledRed() {
  for (int i = 0; i < NUM_LEDS; i++)
  {
    leds[0][i] = CRGB(255, 0, 0);
    leds[1][i] = CRGB(255, 0, 0);
    leds[2][i] = CRGB(255, 0, 0);
    leds[3][i] = CRGB(255, 0, 0);
    leds[4][i] = CRGB(255, 0, 0);
    leds[5][i] = CRGB(255, 0, 0);
  }
}

void ledGreen() {
  for (int i = 0; i < NUM_LEDS; i++)
  {
    leds[0][i] = CRGB(0, 255, 0);
    leds[1][i] = CRGB(0, 255, 0);
    leds[2][i] = CRGB(0, 255, 0);
    leds[3][i] = CRGB(0, 255, 0);
    leds[4][i] = CRGB(0, 255, 0);
    leds[5][i] = CRGB(0, 255, 0);
  }
}

void ledBlue() {
  for (int i = 0; i < NUM_LEDS; i++)
  {
    leds[0][i] = CRGB(0, 0, 255);
    leds[1][i] = CRGB(0, 0, 255);
    leds[2][i] = CRGB(0, 0, 255);
    leds[3][i] = CRGB(0, 0, 255);
    leds[4][i] = CRGB(0, 0, 255);
    leds[5][i] = CRGB(0, 0, 255);
  }
}

void ledWhite() {
  for (int i = 0; i < NUM_LEDS; i++)
  {
    leds[0][i] = CRGB(255, 255, 255); //ok
    leds[1][i] = CRGB(255, 255, 255); //ok
    leds[2][i] = CRGB(255, 255, 255); //ok
    leds[3][i] = CRGB(255, 255, 255); // was 5
    leds[4][i] = CRGB(255, 255, 255); //ok
    leds[5][i] = CRGB(255, 255, 255);
  }
}

void ledOne() {
  ledOFF();
  for (int i = 0; i < NUM_LEDS; i++)
  {
    leds[0][i] = CRGB(255, 255, 255); //ok
  }
}

void ledTwo() {
  ledOFF();
  for (int i = 0; i < NUM_LEDS; i++)
  {
    leds[1][i] = CRGB(255, 255, 255); //ok
  }
}

void ledThree() {
  ledOFF();
  for (int i = 0; i < NUM_LEDS; i++)
  {
    leds[2][i] = CRGB(255, 255, 255); //ok
  }
}

void ledFour() {
  ledOFF();
  for (int i = 0; i < NUM_LEDS; i++)
  {
    leds[3][i] = CRGB(255, 255, 255); //ok
  }
}

void ledFive() {
  ledOFF();
  for (int i = 0; i < NUM_LEDS; i++)
  {
    leds[4][i] = CRGB(255, 255, 255); //ok
  }
}

void ledSix() {
  ledOFF();
  for (int i = 0; i < NUM_LEDS; i++)
  {
    leds[5][i] = CRGB(255, 255, 255); //ok
  }
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
  }
  
  if (isTouch != oldTouch && isTouch == true) { // flip switch if change and on low->high touch
    LED_ENABLE = !LED_ENABLE;
  }

//Serial.println(LED_ENABLE);
  oldTouch = isTouch;

}

// IR decode
void getRemote() {
  if (irrecv.decode(&results))
  {
    Serial.print("REMOTE: ");
    if (results.value == POWER)
    {
      Serial.println("POWER");
      LED_ENABLE = !LED_ENABLE;
    }
    if (results.value == A)
    {
      Serial.println("A");
    }
    if (results.value == B)
    {
      Serial.println("B");
    }
    if (results.value == C)
    {
      Serial.println("C");
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
