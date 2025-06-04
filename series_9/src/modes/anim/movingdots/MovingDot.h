#include <Arduino.h>

class MovingDot
{
private:
    float x;
    float xMax;
    float y;
    float yMax;
    float radius;
    float radiusInc;
    float radiusMin;
    float radiusMax;

    float angle;
    float speed;

    int paletteId;


public:
    void setBounds(float _xMax, float _yMax, float _radiusMin, float _radiusMax);
    void setPalette(int _id);
    void setValues(float _x, float _y, float _xInc, float _yInc, float _radius, float _radiusInc);
    void setPosition(float _x, float _y);
    void setDirection(float _angle, float _speed);
    void setRadius(float _radius, float _radiusInc);

    void update(float animationTime);

    int getPalette();
    float getPercAt(float _x, float _y);
    MovingDot();
};