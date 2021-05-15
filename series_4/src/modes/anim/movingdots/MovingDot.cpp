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

void MovingDot::setPosition(float _x, float _y)
{
  x = _x;
  y = _y;
}

void MovingDot::setDirection(float _angle, float _speed)
{
  angle = _angle;
  speed = _speed;
}

void MovingDot::setRadius(float _radius, float _radiusInc)
{
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

void MovingDot::update(float animationTime)
{
  float distance = speed / animationTime;
  float xMove = sinf(angle) * distance;
  float yMove = cosf(angle) * distance;

  x += xMove;
  if (x > xMax) x = 0;
  if (x < 0) x = xMax;

  y += yMove;
  if (y > yMax) y = 0;
  if (y < 0) y = yMax;

  radius += radiusInc;
  if (radius > radiusMax){
    radius = radiusMax;
    radiusInc = -abs(radiusInc);
  } 
  if (radius < radiusMin){
    radius = radiusMin;
    radiusInc = abs(radiusInc);
  } 
}

float MovingDot::getPercAt(float _x, float _y) {

  float xOffset = (_x < xMax/2) ? -xMax : xMax;
  float xDistLeft = x-_x;
  float xDistRight = x-_x+xOffset;
  float xDist = 0;
  if (fabs(xDistLeft) < fabs(xDistRight)) {
    xDist = pow(xDistLeft, 2);
  } else {
    xDist = pow(xDistRight, 2);
  }

  float yOffset = (_y < yMax/2) ? -yMax : yMax;
  float yDistLeft = y-_y;
  float yDistRight = y-_y+yOffset;
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