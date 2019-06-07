#ifdef SUPPORTS_FFT


void updateFFT() {
  if (FFT.available())
  {
    getFFT( 0, FFT.read( 0, 2) );
    getFFT( 1, FFT.read( 3, 7) );
    getFFT( 2, FFT.read( 8, 19) );
    getFFT( 3, FFT.read( 20, 47) );
    getFFT( 4, FFT.read( 48, 110) );
    getFFT( 5, FFT.read( 111, 255) );
    getMaxLevel();
    // reportFFT();
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

void getMaxLevel() {
  float n = 0.0;
  // get highest band level
  for (int i=0; i<6; i++){
    n = max(fftVals[i],n);
  }
  //Serial.print(n);
  //Serial.print(",");
  // 1 / level to get value to multiply bands with
  if (n > fft_max_band){
    fft_max_band = n;
  } else {
    fft_max_band = fft_max_band * 0.9;
    fft_max_band = max( fft_max_band, 0.005 ); // stop value getting too small / mult getting too large
  }
  //Serial.print(fft_max_band);
  //Serial.print(">");

  fft_mult = 1/fft_max_band;
}

void reportFFT() {
  for (int x=0; x < 6; x++)
  {
    Serial.print( fftVals[x]*100 );
    Serial.print(",");
  }
  Serial.print(">");
  Serial.print(fft_mult);
  
  Serial.println();
}

void updateFFT_Bars() { // fft "equaliser bars"
  for (int i = 0; i < NUM_LEDS; i++)
  {
    for( int x = 0; x < NUM_COLUMNS; x++) {
      int pal = fmod(((x*NUM_LEDS) + i)/3, 5.0); // group the LED Colours
      leds[x][i] = CHSV(palette[ pal ][ PALETTE_HUE ], palette[ pal ][ PALETTE_SATURATION ], valueForLED(fftVals[x % 4],i,NUM_LEDS));
    }
  }
}

void updateFFT_Pulse() { // fft grouped into bands
  int total_bands = 3;
  float number_in_band = NUM_LEDS / total_bands;
  for (int i = 0; i < NUM_LEDS; i++)
  {
    for( int x = 0; x < NUM_COLUMNS; x++) {
      int band = (i / number_in_band);
      int pal = (band + (x*NUM_COLUMNS)) % 5; //  % 5;
      leds[x][i] = CHSV(palette[ pal ][ PALETTE_HUE ], palette[ pal ][ PALETTE_SATURATION ], valueForLED(fftVals[band % 4],band,total_bands));
    }
  }
}

int valueForLED(float value, int number, int maxnum) {
  float retval = 0.1 + (realValueForLED(value, number, maxnum) * 0.9);
  return (retval * 255) * dotBrightness;
}

float realValueForLED(float value, int number, int maxnum) {
  if (value < 0.004) { // silence out low values
    return 0.0;
  }
  float retval = ((value * fft_mult) * maxnum) - number;
  if (retval >= 1) {
    return 1.0;
  } else if (retval < 0) {
    return 0.0;
  } else {
    return fmod(retval,1.0);
  }
}

#endif
