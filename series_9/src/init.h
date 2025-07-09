#ifndef INIT_HPP_INCLUDED
#define INIT_HPP_INCLUDED



//-------LEDS-------

#define NUM_COLUMNS 6
#define COLUMN_MAPPING {1,3,5,4,2,0}

#define CLOCK_PIN_1 23
#define DATA_PIN_1 22

#define CLOCK_PIN_2 1
#define DATA_PIN_2 3

#define CLOCK_PIN_3 21
#define DATA_PIN_3 19

#define CLOCK_PIN_4 18
#define DATA_PIN_4 5

#define CLOCK_PIN_5 17
#define DATA_PIN_5 16

#define CLOCK_PIN_6 4
#define DATA_PIN_6 0

// end:-------LEDS-------

// TOUCH

#define TOUCH_PIN 15

// REMOTE
#define IR_RECV_PIN 5


// Bluetooth
// See the following for generating UUIDs:
// https://www.uuidgenerator.net/
#define BLE_SERVICE_UUID        "F1A0EB1D-9F0D-4EE5-8271-08AB90716F53"
#define BLE_CHARACTERISTIC_UUID "2FB035BE-2E57-4616-A4ED-55EB6F036E30"
// #define SERVICE_UUID           "6E400001-B5A3-F393-E0A9-E50E24DCCA9E" // UART service UUID
// #define CHARACTERISTIC_UUID_RX "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"
#define BLE_CHARACTERISTIC_UUID_TX "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"



#include "lamps.h"

#endif