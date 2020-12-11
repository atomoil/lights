#ifndef BASE_FFT_MODE_HPP_INCLUDED
#define BASE_FFT_MODE_HPP_INCLUDED


#include <Arduino.h>
#include "BaseMode.h"
#include <Audio.h>
#include <math.h>
#include "../managers/PaletteManager.h"


class BaseFFTMode: public BaseMode
{
protected:
    /*
    AudioInputAnalog         adc1(A0);
    AudioAnalyzeFFT256       FFT;
    AudioConnection          patchCord1(adc1, FFT);
    */
    static AudioAnalyzeFFT256 FFT; // why is this not a pointer? AudioConnection signature is called by reference.
    static AudioConnection* patchCord;
    static boolean fftInitialised;
    
    static void initFFT();
    //
    PaletteManager* palette;
    float fft_max_band = 0.0;
    float fft_mult = 1.0;
    float fftVals[6];
    void updateFFT();
    void getFFT(int i, float n);
    void getMaxLevel();
    int valueForLED(float value, int number, int maxnum);
    float realValueForLED(float value, int number, int maxnum);

public:

    void setup();
    void restart();
    BaseFFTMode(LEDManager *ledAttach, PaletteManager *paletteAttach);
};

#endif