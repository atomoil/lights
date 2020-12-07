#include "FastLED.h"
#include "init.h"

class LEDManager
{
private:
    CRGB leds[NUM_COLUMNS][NUM_LEDS];

public:
    void setup();
    void loop();
    void setLED(int x, int y, int r, int g, int b, boolean fixed);
};