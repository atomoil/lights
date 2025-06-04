
#include "src/LampOS.h"


LampOS *lampOS;
int count;

void setup()
{
    //Serial.print("setup");
    //count = 0;
    lampOS = new LampOS();
    lampOS->setup();
}

void loop()
{
    //if (count < 200) {
        //Serial.print("loop ");
        //Serial.println(count);
        //count++;
    //}
    lampOS->loop();
}
