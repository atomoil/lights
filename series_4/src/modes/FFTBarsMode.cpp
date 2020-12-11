#include "FFTBarsMode.h"

FFTBarsMode::FFTBarsMode(
    LEDManager *ledAttach,
    PaletteManager *paletteAttach,
    AudioManager *audioAttach) : BaseFFTMode(ledAttach, paletteAttach, audioAttach)
{
}

/*
void FFTBarsMode::setup() {
    
}
*/

void FFTBarsMode::loop()
{
    for (int i = 0; i < NUM_LEDS; i++)
    {
        for (int x = 0; x < NUM_COLUMNS; x++)
        {
            int pal = fmod(((x * NUM_LEDS) + i) / 3, 5.0); // group the LED Colours
            int hue = palette->hueForSwatch(pal);
            int sat = palette->satForSwatch(pal);
            int val = audio->valueForLED(x % 4,i,NUM_LEDS);
            leds->setHSV( x, i, hue, sat, val, 0);
        }
    }
}