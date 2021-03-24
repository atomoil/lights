#include "AudioManager.h"


// BAND_MINIMUM around 0.191 doesn't pick up my laptop fan anymore - any lower and it does!
#define BAND_MINIMUM 0.0191
#define BAND_DECAY 0.9
#define BAND_OFF_DECAY 0.6
#define FFT_DECAY 0.95

// extern in .h
//AudioAnalyzeFFT256 FFT;

AudioInputAnalog adc1(A0);   
AudioAnalyzeFFT256 FFT; 
AudioConnection patchCord(adc1, FFT);
float fft_max_band = BAND_MINIMUM;
float ff_max_band_decayed = BAND_MINIMUM;


AudioManager::AudioManager()
{
}

void AudioManager::setup()
{
    Serial.println("AudioManager::setup");
    // following was in setup, but maybe better here(?)
    AudioMemory(4);
    Serial.println("AudioManager::setup created instances");
    FFT.averageTogether(8);
    Serial.println("AudioManager::setup ran .averageTogether(8)");
    FFT.windowFunction(AudioWindowHanning256);
    Serial.println("AudioManager::setup ran .windowFunction(AudioWindowHanning256)");

    Serial.println("AudioManager::setup complete");
}

void AudioManager::update()
{
    if (FFT.available())
    {
        getFFT(0, FFT.read(0, 2));
        getFFT(1, FFT.read(3, 7));
        getFFT(2, FFT.read(8, 19));
        getFFT(3, FFT.read(20, 47));
        getFFT(4, FFT.read(48, 110));
        getFFT(5, FFT.read(111, 255));
        getMaxLevel();
    }
}

void AudioManager::getFFT(int i, float n)
{
    float tmp = fftVals[i];
    if (tmp <= 0.01)
        (tmp = 0);

    if (n > tmp) // get peak value
    {
        fftVals[i] = n;
    }
    else
    {
        fftVals[i] = tmp * FFT_DECAY; //decay
    }
}

void AudioManager::getMaxLevel()
{
    float n = 0.0;
    // get highest band level
    for (int i = 0; i < 6; i++)
    {
        n = max(fftVals[i], n);
    }
    
    if (n > fft_max_band)
    {
        fft_max_band = n;
    }
    else
    {
        fft_max_band = fft_max_band * BAND_DECAY;
        fft_max_band = max(fft_max_band, BAND_MINIMUM); // stop value getting too small / mult getting too large
    }

    //int debug_updated = 0;
    if (fft_max_band > BAND_MINIMUM) {
        // debug_updated = 1;
        ff_max_band_decayed = fft_max_band;
        fft_mult = 1 / fft_max_band;
    } else {
        // actually need to keep tailing this off!
        ff_max_band_decayed = max(ff_max_band_decayed*BAND_OFF_DECAY, 0.01);
        if (ff_max_band_decayed > 0.01) {
            fft_mult = 1 / ff_max_band_decayed;
        } else {
            fft_mult = 0;
        }
    }

    /*
    Serial.print("fft_max_band: ");
    Serial.print(debug_updated);
    Serial.print(",");
    Serial.print(fft_max_band);
    Serial.print(",");
    Serial.print(ff_max_band_decayed);
    Serial.print(",");
    Serial.print(n);
    Serial.print(",");
    Serial.println(fft_mult);
    //Serial.print(">");
    */
    
}

int AudioManager::valueForLED(int band, int number, int maxnum)
{
    float value = valueFor(band);
    float retval = 0.1 + (realValueForLED(value, number, maxnum) * 0.9);
    return int(retval * 255);
}

float AudioManager::realValueForLED(float value, int number, int maxnum)
{
    if (value < 0.004)
    { // silence out low values
        return 0.0;
    }
    float retval = ((value * fft_mult) * maxnum) - number;
    if (retval >= 1)
    {
        return 1.0;
    }
    else if (retval < 0)
    {
        return 0.0;
    }
    else
    {
        return fmod(retval, 1.0); // ?
    }
}

float AudioManager::valueFor(int band)
{
    return fftVals[band];
}