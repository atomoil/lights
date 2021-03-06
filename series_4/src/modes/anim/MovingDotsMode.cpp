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
      dots[i]->setBounds(leds->xMax(), leds->yMax(), 1.0, 4.0);
      dots[i]->setPalette(i % 5);
    }

    restart();

    //Serial.println("MovingDotsMode::setup");
}

void MovingDotsMode::restart()
{
  int i;
  float segment = PI/3;
  for(i=0;i<MAX_DOTS;i++) {
    dots[i]->setPosition( (NUM_COLUMNS/5)*i, (NUM_LEDS/5)*i );
    dots[i]->setDirection((PI/32)+(-segment/2)+((segment/5.0)*i), 1.0);
    dots[i]->setRadius(1.0+(0.6*i), 0.05);
  }
}

void MovingDotsMode::loop()
{
  
  int i;
  // update dot positions
  float frameSize = animation->getSpeed() / 6;
  for(i=0;i<MAX_DOTS;i++) {
    dots[i]->update(frameSize);
  }

  float frameRate = 1000.0 / 60.0;

  if (timeSw > frameRate) {
    timeSw = 0;

    // render grid
    int ix;
    int iy;
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
        leds->setHSV(ix, iy, colour.hue, colour.saturation, int(colour.brightness*255), frameRate/2.0);
      }
    }
  }
}