#include "init.h"
AudioInputAnalog         adc1(A0);
AudioAnalyzeFFT256       FFT;
AudioConnection          patchCord1(adc1, FFT);
//basic peak level
AudioAnalyzePeak         pk;
AudioConnection          patchCord2(adc1, pk);
elapsedMillis fps;

void setup() {
  AudioMemory(4);
  Serial.begin(57600);
  FastLED.addLeds<APA102, DATA_PIN1, CLOCK_PIN1, BGR>(leds[0], NUM_LEDS);
  FastLED.addLeds<APA102, DATA_PIN2, CLOCK_PIN2, BGR>(leds[1], NUM_LEDS);
  FastLED.addLeds<APA102, DATA_PIN3, CLOCK_PIN3, BGR>(leds[2], NUM_LEDS);
  FastLED.addLeds<APA102, DATA_PIN4, CLOCK_PIN4, BGR>(leds[3], NUM_LEDS);
  FastLED.addLeds<APA102, DATA_PIN5, CLOCK_PIN5, BGR>(leds[4], NUM_LEDS);
  FastLED.addLeds<APA102, DATA_PIN6, CLOCK_PIN6, BGR>(leds[5], NUM_LEDS);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 4000);

  FFT.averageTogether(8);
  FFT.windowFunction(AudioWindowHanning256);
  ledOff();
  FastLED.show();
}

void loop() {
  if (fps > 39)
  {
    fps = 0;
    // ---- Peak levels
    //     if (pk.available())
    //     {
    //      ledLevel();
    //     }

    // (125 - 17)/6 - 18
    // (125 - 29)/6 - 16
    // ---- FFT
    if (FFT.available())
    {
      hue++;

      /*
      int minVal = 14; // ~ half "29"
      int sizeOfBand = 16;
      for(int i=0;i<6;i++){
        int index = i+1;
        int small = minVal+(i * sizeOfBand);
        int large = minVal+((i+1) * sizeOfBand);
        getFFT( index, FFT.read( small, large) );
      }
      ledCycleLevels();
      // */

      // *
      getFFT( 1, FFT.read( 0, 1) );
      getFFT( 2, FFT.read( 2, 3) );
      getFFT( 3, FFT.read( 4, 9) );
      getFFT( 4, FFT.read( 10, 22) );
      getFFT( 5, FFT.read( 23, 53) );
      getFFT( 6, FFT.read( 54, 126) );
      //ledCycle();
      ledCycleLevels();
      // */
    }

  }
  FastLED.show();
}
//-------------Functions -----------------//
void getFFT(int i, float n) {
  float tmp = fftVals[i];
  if (tmp <= 0.02)(tmp = 0.03);

  if (n > tmp) // get peak value
  {
    fftVals[i] = n*255;
  } else {
    fftVals[i] = tmp * 0.95; //decay
  }
  for (int x; x < 6; x++)
  {
    Serial.print( fftVals[x + 1] );
    Serial.print(",");
  }
  Serial.println();
}

void ledLevel() { // simple levels
  hue++;
  float b = pk.read();
  if (b <= 0.01) (b = 0);
  for (int i = 0; i < NUM_LEDS; i++)
  {
    leds[0][i] = CHSV(hue, sat, bri * b);
    leds[1][i] = CHSV(hue, sat, bri * b);
    leds[2][i] = CHSV(hue, sat, bri * b);
    leds[3][i] = CHSV(hue, sat, bri * b);
    leds[4][i] = CHSV(hue, sat, bri * b);
    leds[5][i] = CHSV(hue, sat, bri * b);

  }
  Serial.println(b);
}

void ledCycle() { // simple fft pattern
  hue++;
  for (int i = 0; i < NUM_LEDS; i++)
  {
    leds[0][i] = CHSV(hue, sat, fftVals[1]);
    leds[1][i] = CHSV(hue, sat, fftVals[2]);
    leds[2][i] = CHSV(hue, sat, fftVals[3]);
    leds[3][i] = CHSV(hue, sat, fftVals[4]);
    leds[4][i] = CHSV(hue, sat, fftVals[5]);
    leds[5][i] = CHSV(hue, sat, fftVals[6]);
  }
}

void ledCycleLevels() { // simple fft pattern
  hue++;
  for (int i = 0; i < NUM_LEDS; i++)
  {
    leds[0][i] = CHSV(hue, sat, valueForLED(fftVals[1],i,NUM_LEDS));
    leds[1][i] = CHSV(hue, sat, valueForLED(fftVals[2],i,NUM_LEDS));
    leds[2][i] = CHSV(hue, sat, valueForLED(fftVals[3],i,NUM_LEDS));
    leds[3][i] = CHSV(hue, sat, valueForLED(fftVals[4],i,NUM_LEDS));
    leds[4][i] = CHSV(hue, sat, valueForLED(fftVals[5],i,NUM_LEDS));
    leds[5][i] = CHSV(hue, sat, valueForLED(fftVals[6],i,NUM_LEDS));
  }
}

float valueForLED(int value, int number, int maxnum) {
  float retval = ((value / 125.0) * maxnum) - number;
  if (retval > 1) {
    return 1;
  } else if (retval < 0) {
    return 0;
  } else {
    return fmod(retval,1.0);
  }
}

void ledOff() { // all off
  for (int i = 0; i < NUM_LEDS; i++)
  {
    leds[0][i] = CHSV(0, 0, 0);
    leds[1][i] = CHSV(0, 0, 0);
    leds[2][i] = CHSV(0, 0, 0);
    leds[3][i] = CHSV(0, 0, 0);
    leds[4][i] = CHSV(0, 0, 0);
    leds[5][i] = CHSV(0, 0, 0);
  }
}
