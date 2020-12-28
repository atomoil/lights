#include <Arduino.h>
#include "../init.h"
#include "../managers/LedManager.h"
#include "../managers/PaletteManager.h"
#include "../managers/AnimationManager.h"
#include "BaseAnimationMode.h"
#include <tuple>


class RandomPixelMode: public BaseAnimationMode
{
private:
    elapsedMillis timeSw;
    int currentSwatch;
    int currentDot;
    std::tuple<int, int> dots[NUM_COLUMNS*NUM_LEDS]; // needs to be a tuple

    void shuffleDots();
    void shuffle(std::tuple<int, int> *array, int nmemb, int size);
    void shuffle_swap(int index_a, int index_b, std::tuple<int, int> *array, int size);
    int rand_range(int n);

public:
    void setup();
    void restart();
    void loop();
    RandomPixelMode(LEDManager *ledAttach, PaletteManager *paletteAttach, AnimationManager *animation);
};