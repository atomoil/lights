#ifdef USING_PLATFORMIO

#include "main.h"

LampOS *lampOS;
int count;

void setup()
{
    Serial.begin(115200);
    delay(2000);
    Serial.println("=== MAIN SETUP START ===");
    Serial.print("Free heap before LampOS: ");
    Serial.println(ESP.getFreeHeap());
    Serial.flush();
    
    // Try to allocate LampOS
    try {
        Serial.println("About to create LampOS...");
        lampOS = new LampOS();
        Serial.println("LampOS created successfully!");
    } catch (...) {
        Serial.println("ERROR: Failed to create LampOS!");
        return;
    }
    
    Serial.print("Free heap after LampOS: ");
    Serial.println(ESP.getFreeHeap());
    Serial.println("About to call lampOS->setup()...");
    Serial.flush();
    
    lampOS->setup();
    
    Serial.println("Setup complete!");
    Serial.flush();
}

void loop()
{

    lampOS->loop();
}

#endif