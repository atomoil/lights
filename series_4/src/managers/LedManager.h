#ifndef LED_MANAGER_HPP_INCLUDED
#define LED_MANAGER_HPP_INCLUDED

#include "FastLED.h"
#include "../init.h"

#define LED_FRAME_RATE 60.0

enum LedColourTweenType {
    NO_TWEEN, SINE
};

struct LedColourTween {
    float current;
    LedColourTweenType tweenType;
    float begin;
    float change;
    float duration;
};

struct LedData {
    int x;
    int y;
    LedColourTween r;
    LedColourTween g;
    LedColourTween b;
};

class LEDManager
{
private:
    LedData matrix[NUM_COLUMNS][NUM_LEDS];
    CRGB leds[NUM_COLUMNS][NUM_LEDS];
    elapsedMillis frameMs;
    elapsedMillis tweenMs;
    float frameSize;
    float brightness;
    void updateLEDs();
    LedColourTween updateColour(float time, LedColourTween colour, boolean debug);
public:
    void setup();
    void loop();
    int totalLEDs();
    void setRGB(int x, int y, int r, int g, int b, float timeInSeconds);
    void setHSV(int x, int y, int h, int s, int v, float timeInSeconds);
    void setBrightness(float level);
    float getBrightness();
};

#endif