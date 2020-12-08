#ifndef LED_MANAGER_HPP_INCLUDED
#define LED_MANAGER_HPP_INCLUDED

#include "FastLED.h"
#include "init.h"

#define FRAME_RATE 60.0

/*
enum LedColourChangeType {
    MULTIPLY, LINEAR
};
*/

struct LedColour {
    float current;
    float desired;
    float changeValue;
    //LedColourChangeType changeType; // @TODO maybe add this... maybe
};

struct LedData {
    int x;
    int y;
    LedColour r;
    LedColour g;
    LedColour b;
};


class LEDManager
{
private:
    LedData matrix[NUM_COLUMNS][NUM_LEDS];
    CRGB leds[NUM_COLUMNS][NUM_LEDS];
    elapsedMillis frameMs;
    float frameSize;
    void updateLEDs();
    LedColour updateColour(LedColour colour);
public:
    void setup();
    void loop();
    int totalLEDs();
    void setLED(int x, int y, int r, int g, int b, float timeInSeconds);
};

#endif