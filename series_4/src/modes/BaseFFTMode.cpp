#include "BaseFFTMode.h"

BaseFFTMode::BaseFFTMode(
    LEDManager *ledAttach,
    PaletteManager *paletteAttach) : BaseMode(ledAttach),
                                     palette(paletteAttach)
{
    AudioInputAnalog adc1 = AudioInputAnalog(A0);
    FFT = AudioAnalyzeFFT256();
    patchCord = new AudioConnection(adc1, FFT);

}

void BaseFFTMode::updateFFT() {
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

void BaseFFTMode::getFFT(int i, float n) {
  float tmp = fftVals[i];
  if (tmp <= 0.01)(tmp = 0);

  if (n > tmp) // get peak value
  {
    fftVals[i] = n;
  } else {
    fftVals[i] = tmp * 0.95; //decay
  }
}

void BaseFFTMode::getMaxLevel() {
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