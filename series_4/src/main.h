#include <Arduino.h>

// --Timers
const int deltaSw = 500;
uint8_t  count = 1;
elapsedMillis timeSw;

// functions
void loopLEDs();
void ledRed();
void ledGreen();
void ledBlue();
void ledWhite();
void setAllLEDsTo(int r, int g, int b);