#include "AnimationMode.h"

AnimationMode::AnimationMode(
    LEDManager *ledAttach,
    PaletteManager *paletteAttach) : BaseMode(ledAttach),
                                     palette(paletteAttach)
{
}

void AnimationMode::setup()
{
    frameSize = 1000 / ANIMATION_FRAME_RATE;
    // create the dots
    for (int c = 0; c < NUM_COLUMNS; c++)
    {
        for (int i = 0; i < NUM_LEDS; i = i + 1)
        {
            // Serial.print ("LED: ");
            // Serial.println(i); // for example
            float minValue = 0.0;
            float maxValue = 255.0;
            int hue = 80;
            int sat = 255;
            LightDot light = {c, i, 0.0, 0.0, minValue, maxValue, 0, hue, sat};
            lights[c][i] = light;
        }
    }
    restart();
}

void AnimationMode::restart()
{
    float timeElapsed = currentAnimatingSpeed / 500.0;
    float margin = (timeElapsed / 10.0);
    // int deltaUpdate = 60; // @TODO every 60ms or 16fps - doesn't seem right

    for (int c = 0; c < NUM_COLUMNS; c++)
    {
        for (int d = 0; d < NUM_LEDS; d++)
        {
            LightDot dot = lights[c][d];

            // set the initial colour
            int colourInPalette = (d % palette->totalSwatches());
            dot.colourId = colourInPalette;
            dot.hue = palette->hueForSwatch(dot.colourId); // [ dot.colourId ][ PALETTE_HUE ];
            dot.sat = palette->satForSwatch(dot.colourId); // [ dot.colourId ][ PALETTE_SATURATION ];
            // set initial current value - make some on, some off and some in between
            dot.currentValue = ((c % 3) + (d % 3)) * (255 / 6.0);
            // make increment vary
            float durationToReach = timeElapsed + ((d % 10) * margin);
            dot.increment = 1 / (durationToReach / frameSize);
            // set varying minimum and maximum
            dot.minimumValue = -210.0 - (((d + 2) % 4) * 30);
            dot.maximumValue = 255.0 + (((d + 3) % 7) * 30);
            /*
      Serial.print("dotValues:");
      Serial.print(dot.minimumValue);
      Serial.print(":");
      Serial.println(dot.maximumValue);
      */
            // reassign the dot to the array
            lights[c][d] = dot;
        }
    }
    /*
    Serial.print("[animating:");
    Serial.print(timeElapsed);
    Serial.print("/");
    float showNum = 1 / ((timeElapsed + ((2 % 10) * margin)) / deltaUpdate);
    Serial.print(showNum);
    Serial.println("]");
    */
}

void AnimationMode::loop()
{
    // Serial.println("AnimationMode::loop");
    for (int c = 0; c < NUM_COLUMNS; c++)
    {
        for (int d = 0; d < NUM_LEDS; d++)
        {
            LightDot dot = lights[c][d];
            // update the dot
            dot.currentValue = dot.currentValue + dot.increment;
            // check for hitting boundaries
            if ((dot.increment < 0) && (dot.currentValue < dot.minimumValue))
            {
                // if we're heading down and have hit the bottom
                dot.currentValue = dot.minimumValue;
                dot.increment = abs(dot.increment);
                // recycle the dot
                dot.colourId = dot.colourId + 1;
                if (dot.colourId >= palette->totalSwatches())
                {
                    dot.colourId = 0;
                }
                dot.hue = palette->hueForSwatch(dot.colourId);
                dot.sat = palette->satForSwatch(dot.colourId);
            }
            else if ((dot.increment > 0) && dot.currentValue > dot.maximumValue)
            {
                // if we've heading up and have hit the top
                dot.currentValue = dot.maximumValue;
                dot.increment = -abs(dot.increment);
                //dot = dotHasReachedHighestValue(dot);
            }
            lights[c][d] = dot;

            // apply the colour to the LED
            // value < 0 means off, greater than 255 is on
            // keep colour within bounds as dot can go higher and lower (to stay on or off for longer)
            if (dot.currentValue > 0)
            {
                int rawVal = min(255, dot.currentValue);
                //int val = int(float(rawVal) * dotBrightness);
                //CHSV colour = CHSV(dot.hue, dot.sat, val);
                //CHSV colour = CHSV( 125, 255, val );
                //leds[c][dot.led] = colour;
                //leds->setRGB(dot.)
                // leds->setRGB( dot.column, dot.led, )
                leds->setHSV(dot.column, dot.led, dot.hue, dot.sat, rawVal, 0);
            }
            else
            {
                leds->setRGB(dot.column, dot.led, 0, 0, 0, 0);
            }
        }
    }
}

void AnimationMode::setAnimationSpeed(float newSpeed)
{
    if (newSpeed >= 150)
    {
        currentAnimatingSpeed = newSpeed;
    }
    else
    {
        currentAnimatingSpeed = 150;
    }
    Serial.print("currentAnimatingSpeed");
    Serial.println(currentAnimatingSpeed);

    float timeElapsed = currentAnimatingSpeed / 500.0; // eh?
    float margin = (timeElapsed / 10.0);

    for (int c = 0; c < NUM_COLUMNS; c++)
    {
        for (int d = 0; d < NUM_LEDS; d++)
        {
            LightDot dot = lights[c][d];
            float durationToReach = timeElapsed + ((d % 10) * margin);
            dot.increment = 1 / (durationToReach / frameSize);
            lights[c][d] = dot;
        }
    }
}