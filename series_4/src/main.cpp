#ifdef USING_PLATFORMIO

#include "main.h"

LampOS *lampOS;
int count;

void setup()
{
    lampOS = new LampOS();
    lampOS->setup();
}

void loop()
{

    lampOS->loop();
}

#endif