/* To change name, load this sketch into the teensy.
 *  Then:
 *  open Arduino Serial monitor,
 * make sure its set to NL+CR.  
 * Then type: AT+NAMEnewName
 * press 'send' and when rebooted the bluetooth
 *  should have the new name.
 *  you can then replace this serial ->bluetooth sketch
 *  with the lamp code.
*/
const int ledPin = 13;
#define btSerial Serial1

char c=' ';
boolean NL = true;
 
void setup() 
{
    Serial.begin(57600);
    Serial.print("Sketch:   ");   Serial.println(__FILE__);
    Serial.print("Uploaded: ");   Serial.println(__DATE__);
    Serial.println(" ");
 
    btSerial.begin(9600);  
    Serial.println("BTserial started at 9600");
      pinMode(ledPin, OUTPUT);
}
 
void loop()
{
    // Read from the Bluetooth module and send to the Arduino Serial Monitor
    if (btSerial.available())
    {
        c = btSerial.read();
        Serial.write(c);
    }
 
 
    // Read from the Serial Monitor and send to the Bluetooth module
    if (Serial.available())
    {
        c = Serial.read();
 
        // do not send line end characters to the HM-10
       // if (c!=10 & c!=13 ) 
       // {  
             btSerial.write(c);
       // }
 
        // Echo the user input to the main window. 
        // If there is a new line print the ">" character.
//        if (NL) { Serial.print("\r\n>");  NL = false; }
//        Serial.write(c);
//        if (c==10) { NL = true; }
    }
}
