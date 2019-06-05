#ifdef SUPPORTS_FFT


void updateFFT() {
  if (FFT.available())
  {

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

    /*
    getFFT( 0, FFT.read( 0, 1) );
    getFFT( 1, FFT.read( 2, 3) );
    getFFT( 2, FFT.read( 4, 9) );
    getFFT( 3, FFT.read( 10, 22) );
    getFFT( 4, FFT.read( 23, 53) );
    getFFT( 5, FFT.read( 54, 126) );
    */
    getFFT( 0, FFT.read( 0, 2) );
    getFFT( 1, FFT.read( 3, 7) );
    getFFT( 2, FFT.read( 8, 19) );
    getFFT( 3, FFT.read( 20, 47) );
    getFFT( 4, FFT.read( 48, 110) );
    getFFT( 5, FFT.read( 111, 255) );
    reportFFT();
  } else {
    Serial.println("FFT not ready");
  }
}

void getFFT(int i, float n) {
  float tmp = fftVals[i];
  if (tmp <= 0.01)(tmp = 0);

  if (n > tmp) // get peak value
  {
    fftVals[i] = n;
  } else {
    fftVals[i] = tmp * 0.95; //decay
  }
}

void reportFFT() {
  for (int x; x < 6; x++)
  {
    Serial.print( fftVals[x + 1] );
    Serial.print(",");
  }
  Serial.println();
}

void updateFFT_Bars() { // fft "equaliser bars"
  int hue = 20;
  int sat = 255;
  for (int i = 0; i < NUM_LEDS; i++)
  {
    leds[0][i] = CHSV(palette[ 0 ][ PALETTE_HUE ], palette[ 0 ][ PALETTE_SATURATION ], valueForLED(fftVals[0],i,NUM_LEDS));
    leds[1][i] = CHSV(palette[ 1 ][ PALETTE_HUE ], palette[ 1 ][ PALETTE_SATURATION ], valueForLED(fftVals[1],i,NUM_LEDS));
    leds[2][i] = CHSV(palette[ 2 ][ PALETTE_HUE ], palette[ 2 ][ PALETTE_SATURATION ], valueForLED(fftVals[2],i,NUM_LEDS));
    leds[3][i] = CHSV(palette[ 3 ][ PALETTE_HUE ], palette[ 3 ][ PALETTE_SATURATION ], valueForLED(fftVals[3],i,NUM_LEDS));
    leds[4][i] = CHSV(palette[ 4 ][ PALETTE_HUE ], palette[ 4 ][ PALETTE_SATURATION ], valueForLED(fftVals[4],i,NUM_LEDS));
    leds[5][i] = CHSV(palette[ 2 ][ PALETTE_HUE ], palette[ 2 ][ PALETTE_SATURATION ], valueForLED(fftVals[5],i,NUM_LEDS));
  }
}

int valueForLED(float value, int number, int maxnum) {
  float retval = 0.2 + (realValueForLED(value, number, maxnum) * 0.8);
  return (retval * 255) * dotBrightness;
}

float realValueForLED(float value, int number, int maxnum) {
  float retval = (value * maxnum) - number;
  if (retval >= 1) {
    return 1.0;
  } else if (retval < 0) {
    return 0.0;
  } else {
    return fmod(retval,1.0);
  }
}

#endif
