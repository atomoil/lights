#include <Arduino.h>
#include "../init.h"
#include "../managers/LedManager.h"
#include "../managers/PaletteManager.h"
#include "BaseMode.h"
#include <Audio.h>
#include <math.h>


class BaseFFTMode: public BaseMode
{
protected:
    /*
    AudioInputAnalog         adc1(A0);
    AudioAnalyzeFFT256       FFT;
    AudioConnection          patchCord1(adc1, FFT);
    */
    PaletteManager* palette;
    AudioAnalyzeFFT256 FFT; // why is this not a pointer? AudioConnection signature is called by reference.
    AudioConnection* patchCord;
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
    void loop();
    BaseFFTMode(LEDManager *ledAttach, PaletteManager *paletteAttach);
};

