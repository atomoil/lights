#include "PaletteManager.h"

#define PALETTE_SWATCH_HUE 0
#define PALETTE_SWATCH_SATURATION 1

int EEPROM_SWATCH_0_HUE = 0;
int EEPROM_SWATCH_0_SAT = 1;
int EEPROM_SWATCH_1_HUE = 2;
int EEPROM_SWATCH_1_SAT = 3;
int EEPROM_SWATCH_2_HUE = 4;
int EEPROM_SWATCH_2_SAT = 5;
int EEPROM_SWATCH_3_HUE = 6;
int EEPROM_SWATCH_3_SAT = 7;
int EEPROM_SWATCH_4_HUE = 8;
int EEPROM_SWATCH_4_SAT = 9;

void PaletteManager::setup()
{
    Serial.println("PaletteManager::setup");
    totalPalettes = sizeof(palette) / sizeof(palette[0]);
    loadPalette();
}

void PaletteManager::loadPalette()
{
    palette[0][0] = EEPROM.read(EEPROM_SWATCH_0_HUE);
    palette[0][1] = EEPROM.read(EEPROM_SWATCH_0_SAT);
    palette[1][0] = EEPROM.read(EEPROM_SWATCH_1_HUE);
    palette[1][1] = EEPROM.read(EEPROM_SWATCH_1_SAT);
    palette[2][0] = EEPROM.read(EEPROM_SWATCH_2_HUE);
    palette[2][1] = EEPROM.read(EEPROM_SWATCH_2_SAT);
    palette[3][0] = EEPROM.read(EEPROM_SWATCH_3_HUE);
    palette[3][1] = EEPROM.read(EEPROM_SWATCH_3_SAT);
    palette[4][0] = EEPROM.read(EEPROM_SWATCH_4_HUE);
    palette[4][1] = EEPROM.read(EEPROM_SWATCH_4_SAT);

}

void PaletteManager::savePalette() {
    EEPROM.put(0,palette);

    EEPROM.write(EEPROM_SWATCH_0_HUE, palette[0][0]);
    EEPROM.write(EEPROM_SWATCH_0_SAT, palette[0][1]);
    EEPROM.write(EEPROM_SWATCH_1_HUE, palette[1][1]);
    EEPROM.write(EEPROM_SWATCH_1_SAT, palette[1][1]);
    EEPROM.write(EEPROM_SWATCH_2_HUE, palette[2][0]);
    EEPROM.write(EEPROM_SWATCH_2_SAT, palette[2][1]);
    EEPROM.write(EEPROM_SWATCH_3_HUE, palette[3][0]);
    EEPROM.write(EEPROM_SWATCH_3_SAT, palette[3][1]);
    EEPROM.write(EEPROM_SWATCH_4_HUE, palette[4][0]);
    EEPROM.write(EEPROM_SWATCH_4_SAT, palette[4][1]);
}

// pl:10:11:20:21:30:31:40:41:50:51:60:61
// pl:10:100:20:100:30:100:40:100:50:100
//
void PaletteManager::setPaletteFromPlCode(String stringInput)
{
    char input[100];
    stringInput.toCharArray(input, 99);
    Serial.print("PaletteManager::setPaletteFromPlCode: '");
    Serial.print(input);
    Serial.print("' ");
    Serial.println(totalPalettes);
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
            palette[row][0] = importHue(col);
        }
        else
        {
            palette[row][1] = importSat(col);
        }
        Serial.print(row);
        Serial.print("/");
        Serial.print(elem);
        Serial.print(": ");
        Serial.println(col);
        i++;
    }
    savePalette();
}

int PaletteManager::importHue(int i)
{
    float f = float(i);
    return round((f / 360.0) * 255.0);
}

int PaletteManager::importSat(int i)
{
    float f = float(i);
    return round((f / 100.0) * 255.0);
}

String PaletteManager::getPaletteAsPlCode() {
    char palette_char[100];
    sprintf(palette_char, "pl:%i:%i:%i:%i:%i:%i:%i:%i:%i:%i", 
        exportHue(palette[0][0]), exportSat(palette[0][1]),
        exportHue(palette[1][0]), exportSat(palette[1][1]),
        exportHue(palette[2][0]), exportSat(palette[2][1]),
        exportHue(palette[3][0]), exportSat(palette[3][1]),
        exportHue(palette[4][0]), exportSat(palette[4][1])
    );
    return String(palette_char);
}

int PaletteManager::exportHue(int i)
{
    float f = float(i);
    return round((f / 255.0) * 360.0);
}

int PaletteManager::exportSat(int i)
{
    float f = float(i);
    return round((f / 255.0) * 100.0);
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