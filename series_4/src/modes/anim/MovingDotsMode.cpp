#include "MovingDotsMode.h"

MovingDotsMode::MovingDotsMode(
    LEDManager *ledAttach,
    PaletteManager *paletteAttach,
    AnimationManager *animationAttach) : BaseAnimationMode(ledAttach, paletteAttach, animationAttach)
{
}

void MovingDotsMode::setup()
{
    modeId = 23;
    modeName = "lava";

    int i;
    for(i=0;i<MAX_DOTS;i++) {
      dots[i] = new MovingDot();
      dots[i]->setBounds(leds->xMax(), leds->yMax(), 1.0, 5.0);
      dots[i]->setPalette(i % 5);
    }

    restart();

    //Serial.println("MovingDotsMode::setup");
}

void MovingDotsMode::restart()
{
  dots[0]->setValues(3.5, 4,  /* 0.01 */0.1, 0.002, 2.0, 0.0);
  //dots[1]->setValues(1,   1, -0.02, 0.001, 2.0, -0.001);
}

void MovingDotsMode::loop()
{
    if (timeSw > animation->getSpeed()) {
      timeSw = 0;
      
      dots[0]->update();
      //dots[1]->update();

      // update grid
      int ix;
      int iy;
      int i;
      DotColour colour;
      MovingDot *dot;
      float perc = 0;
      int swatch;
      for(ix=0;ix<leds->xMax();ix++){
        for(iy=0;iy<leds->yMax();iy++){
          colour = {0,0,0};

          for(i=0;i<MAX_DOTS;i++) {
            dot = dots[i];
            perc = dot->getPercAt(float(ix), float(iy));
            if (perc > colour.brightness){
              // this dot is the brightest, so update the pixels
              swatch = dot->getPalette();
              colour.hue = palette->hueForSwatch(swatch);
              colour.saturation = palette->satForSwatch(swatch);
              colour.brightness = perc;
            }
          }
          leds->setHSV(ix, iy, colour.hue, colour.saturation, int(colour.brightness*255), 0);
        }
      }
    }
}