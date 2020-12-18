
// MAC USB <---> TEENSY <--->BLE

#define serialBLE Serial1 // hardware serial port to/from BLE chip
#define serialUSB Serial // USB serial  to/from mac

void setup() {
  serialBLE.begin(57600); //BLE
  serialUSB.begin(57600); //USB
}

void loop() {
  
if (serialBLE.available()) {
    char ch = serialBLE.read(); // get chars from BLE
    serialUSB.print(ch); // send to Arduino serial monitor
 }

 
}
// SerialEvent ONLY works on the default serial i.e USB to mac.
