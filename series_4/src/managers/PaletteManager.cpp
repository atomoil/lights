#include "PaletteManager.h"

#define PALETTE_SWATCH_HUE 0
#define PALETTE_SWATCH_SATURATION 1

void PaletteManager::setup()
{
    totalPalettes = sizeof(palette) / sizeof(palette[0]);
    loadPalette();
}

void PaletteManager::loadPalette()
{
    int savedPalette[5][2] = {};
    EEPROM.get(0, savedPalette);
    int paletteSize = sizeof(savedPalette);

    if (paletteSize > 0)
    { // only update if we have values
        for (int i = 0; i < totalPalettes; i++)
        {
            palette[i][0] = savedPalette[i][0];
            palette[i][1] = savedPalette[i][1];
        }
    }
}

void PaletteManager::savePalette() {
    EEPROM.put(0,palette);
}

// pl:10:11:20:21:30:31:40:41:50:51:60:61
// pl:10:100:20:100:30:100:40:100:50:100
//
void PaletteManager::setPaletteFromPlCode(char input[100])
{
    char *text = strtok(input, ":"); // remove first 'pl'
    int col = 0;
    int i = 0;
    int row = 0;
    int elem = 0;
    while (text != 0 && i < totalPalettes * 2)
    {
        row = i / 2;
        elem = i % 2;
        text = strtok(0, ":");
        col = atoi(text);
        if (elem == 0)
        {
            palette[row][0] = convertHue(col);
        }
        else
        {
            palette[row][1] = convertSat(col);
        }
        Serial.print(row);
        Serial.print("/");
        Serial.print(elem);
        Serial.print(": ");
        Serial.println(col);
        i++;
    }
}

int PaletteManager::convertHue(float f)
{
    return (f / 360.0) * 255.0;
}

int PaletteManager::convertSat(float f)
{
    return (f / 100.0) * 255.0;
}

int PaletteManager::hueForSwatch(int swatch_id)
{
    if (swatch_id >= 5) return 0;
    return palette[swatch_id][PALETTE_SWATCH_HUE];
}

int PaletteManager::satForSwatch(int swatch_id)
{
    if (swatch_id >= 5) return 0;
    return palette[swatch_id][PALETTE_SWATCH_SATURATION];
}

int PaletteManager::totalSwatches() {
    return 5;
}