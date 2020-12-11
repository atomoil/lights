#include "FFTPulseMode.h"

FFTPulseMode::FFTPulseMode(
    LEDManager *ledAttach,
    PaletteManager *paletteAttach,
    AudioManager *audioAttach) : BaseFFTMode(ledAttach, paletteAttach, audioAttach)
{
}

/*
void FFTPulseMode::setup() {

}
*/

void FFTPulseMode::loop()
{
    audio->update();
    int total_bands = 3;
    float number_in_band = NUM_LEDS / total_bands;
    for (int i = 0; i < NUM_LEDS; i++)
    {
        for (int x = 0; x < NUM_COLUMNS; x++)
        {
            int band = (i / number_in_band);
            int pal = (band + (x * NUM_COLUMNS)) % 5; //  % 5;
            int hue = palette->hueForSwatch(pal);
            int sat = palette->satForSwatch(pal);
            int val = audio->valueForLED(band % 4, band, total_bands);
            leds->setHSV(x, i, hue, sat, val, 0);
            //leds[x][i] = CHSV(palette[ pal ][ PALETTE_HUE ], palette[ pal ][ PALETTE_SATURATION ], valueForLED(fftVals[band % 4],band,total_bands));
        }
    }
}