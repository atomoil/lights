#include <Arduino.h>

class MovingDot
{
private:
    float x;
    float xInc;
    float xMax;
    float y;
    float yMax;
    float yInc;
    float radius;
    float radiusInc;
    float radiusMin;
    float radiusMax;

    int paletteId;


public:
    void setBounds(float _xMax, float _yMax, float _radiusMin, float _radiusMax);
    void setPalette(int _id);
    void setValues(float _x, float _y, float _xInc, float _yInc, float _radius, float _radiusInc);

    void update();

    int getPalette();
    float getPercAt(float _x, float _y);
    MovingDot();
};