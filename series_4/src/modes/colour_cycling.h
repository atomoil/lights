#include <Arduino.h>
#include "init.h"
#include "led_manager.h"
#include "BaseMode.h"


class ColourCyclingMode: public BaseMode
{
private:
    int* colours;
    int countColours;
    uint8_t  count = 1;
    elapsedMillis timeSw;
    unsigned int howOftenToChange;

    // void loopLEDs();
    void ledRed();
    void ledGreen();
    void ledBlue();
    void ledWhite();
    void setAllLEDsTo(int r, int g, int b);

public:
    void setup();
    void loop();
    ColourCyclingMode(LEDManager &ledAttach, unsigned int howOftenToChangeAttach, int coloursAttach[6], int countColours);
};