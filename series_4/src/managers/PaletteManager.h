#ifndef PALETTE_MANAGER_HPP_INCLUDED
#define PALETTE_MANAGER_HPP_INCLUDED

#include <Arduino.h>
#include <EEPROM.h>

class PaletteManager
{
private:
    int palette[5][3] = { {40,255}, {80,255}, {120,255}, {140,255}, {180,255} };
    int totalPalettes;
    void loadPalette();
    void savePalette();
    int convertHue(float f);
    int convertSat(float f);
public:
    void setup();
    void setPaletteFromPlCode(char input[100]);
    int hueForSwatch(int swatch_id);
    int satForSwatch(int swatch_id);
    int totalSwatches();
};

#endif