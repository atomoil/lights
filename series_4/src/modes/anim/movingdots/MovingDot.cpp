#include "MovingDot.h"
#include "math.h"

MovingDot::MovingDot() 
{

};

void MovingDot::setBounds(float _xMax, float _yMax, float _radiusMin, float _radiusMax)
{
  xMax = _xMax;
  yMax = _yMax;
  radiusMin = _radiusMin;
  radiusMax = _radiusMax;
}

void MovingDot::setValues(float _x, float _y, float _xInc, float _yInc, int _radius, int _radiusInc)
{
  x = _x;
  y = _y;
  xInc = _xInc;
  yInc = _yInc;
  radius = _radius;
  radiusInc = _radiusInc;
}

void MovingDot::setPalette(int _id) 
{
  paletteId = _id;
}

int MovingDot::getPalette() 
{
  return paletteId;
}

void MovingDot::update()
{
  Serial.println("MovingDot::update");
  
  x += xInc;
  if (x > xMax) x = 0;
  if (x < 0) x = xMax;

  y += yInc;
  if (y > yMax) y = 0;
  if (y < 0) y = yMax;

  radius += radiusInc;
  if (radius > radiusMax) radius = radiusMin;
  if (radius < radiusMin) radius = radiusMax;

  Serial.println("MovingDot::update done");
}

float MovingDot::getPercAt(float _x, float _y) {
  float xDistLeft = x-_x;
  float xDistRight = x-_x+xMax;
  float xDist = 0;
  if (fabs(xDistLeft) < fabs(xDistRight)) {
    xDist = pow(xDistLeft, 2);
  } else {
    xDist = pow(xDistRight, 2);
  }

  float yDistLeft = y-_y;
  float yDistRight = y-_y+yMax;
  float yDist = 0;
  if (fabs(yDistLeft) < fabs(yDistRight)) {
    yDist = pow(yDistLeft, 2);
  } else {
    yDist = pow(yDistRight, 2);
  }

  float distance = sqrt(xDist + yDist);
  
  float perc = radius-distance;
  if (perc > 1) return 1;
  if (perc < 0) return 0;
  return perc;
}