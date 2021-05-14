
#include "RandomPixelMode.h"

RandomPixelMode::RandomPixelMode(
    LEDManager *ledAttach,
    PaletteManager *paletteAttach,
    AnimationManager *animationAttach) : BaseAnimationMode(ledAttach, paletteAttach, animationAttach)
{
}

void RandomPixelMode::setup()
{
    modeId = 22;
    modeName = "splatter";

    Serial.println("RandomPixelMode::setup");
    int i=0;
    for(int c=0; c < NUM_COLUMNS; c++){
        for(int l=0; l < NUM_LEDS; l++){
            dots[i] = std::tuple<int, int>(c, l);
            i++;
        }
    }
    Serial.println("RandomPixelMode::setup created dots array");
    // randomise
    int noise = 80; //analogRead(38); // internal temperature sensor from https://forum.pjrc.com/threads/41835-randomSeed(-elapsedMillis)
    Serial.print("RandomPixelMode::setup random seed is ");
    Serial.println(noise);
    randomSeed(noise);
    Serial.println("RandomPixelMode::setup set randomSeed");
    //restart();
}

void RandomPixelMode::restart()
{
    currentDot = 0;
    currentSwatch = 0;
    shuffleDots();
    // randomise leds
}

void RandomPixelMode::loop()
{
    if (timeSw > (animation->getSpeed()/NUM_COLUMNS)) {
        timeSw = 0;

        // get position
        int column;
        int led;
        std::tie(column, led) = dots[currentDot]; // get the output of this

        // get time
        float transitionTimeMs = animation->getSpeed()*4;

        // get colour
        int hue = palette->hueForSwatch(currentSwatch);
        int sat = palette->satForSwatch(currentSwatch);

        // set leds at position to colour
        leds->setHSV(column, led, hue, sat, 255, transitionTimeMs);

        currentDot += 1;
        if (currentDot >= (NUM_COLUMNS*NUM_LEDS)) {
            shuffleDots();
            // randomise the array of Leds
            currentDot = 0;

            currentSwatch += 1;
            if (currentSwatch >= palette->totalSwatches()){
                currentSwatch = 0;
            }
        } 
    }
}

void RandomPixelMode::shuffleDots()
{
    int size = sizeof(std::tuple<int, int>);
    int total = sizeof(dots) / size;
    shuffle(dots, total, size);
}

void RandomPixelMode::shuffle_swap(int index_a, int index_b, std::tuple<int, int> *array, int size)
{
   char *x, *y, tmp[size];

   if (index_a == index_b) return;

   x = (char*)array + index_a * size;
   y = (char*)array + index_b * size;

   memcpy(tmp, x, size);
   memcpy(x, y, size);
   memcpy(y, tmp, size);
}

// shuffle an array using fisher-yates method, O(n)
void RandomPixelMode::shuffle(std::tuple<int, int> *array, int nmemb, int size)
{
   int r;
   
   while (nmemb > 1) {                                                                      
       r = rand_range(nmemb--);                                                              
       shuffle_swap(nmemb, r, array, size);
   }
}

// generate a value between 0 <= x < n, thus there are n possible outputs
int RandomPixelMode::rand_range(int n)
{
   int r, ul;
   ul = RAND_MAX - RAND_MAX % n;
   //while ((r = random(RAND_MAX)) >= ul);
   r = random(ul);
   return r % n;
}