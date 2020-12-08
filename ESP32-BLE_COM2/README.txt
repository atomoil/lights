
    Setup:
Add to prefs additional board manager URL:
https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json

Boards Manager.type esp32 and install.

* Open Tools > Manage Libraries
* Type m5atom and install

   When flashing the M5Atom:
    Select M5stick-c from board menu.
    Upload speed 15000000
    Partitioning scheme: no OTA
    Port /dev/cu.usbserial-xxxxxxxx
    
    I used BLE terminal app on iPad to test.
    SERIAL MONITOR  - SET TO 57600

    Data from wireless (BLE receive) Handled by callback
    which then sends it to teensy via Serial2.