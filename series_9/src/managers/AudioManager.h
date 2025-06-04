#ifndef AUDIO_MANAGER_HPP_INCLUDED
#define AUDIO_MANAGER_HPP_INCLUDED

#include <Arduino.h>
#include <Audio.h>
#include <math.h>

class AudioManager {

private:
    float fft_max_band = 0.0;
    float fft_mult = 1.0;
    float fftVals[6];
    void getFFT(int i, float n);
    void getMaxLevel();
    float valueFor(int band);
    float realValueForLED(float value, int number, int maxnum);

public:
    void setup();
    void update();
    int valueForLED(int band, int number, int maxnum);
    AudioManager();
};

#endif