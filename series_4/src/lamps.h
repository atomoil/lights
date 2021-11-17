
#define LAMP_S7_01_to_05 1
#define LAMP_S7_05_and_06_STARBURST 2
#define LAMP_S7_07_and_08_and_09_and_10 3

#define LAMP_CURRENT LAMP_S7_07_and_08_and_09_and_10



#if LAMP_CURRENT == LAMP_S7_01_to_05

#define NUM_LEDS 15
#define TOUCH_ON 1200
#define TOUCH_OFF 700
#define SUPPORTS_FFT 1

#endif

  #if LAMP_CURRENT == LAMP_S7_05_and_06_STARBURST

  #define NUM_LEDS 14
  #define TOUCH_ON 900
  #define TOUCH_OFF 500
  #define SUPPORTS_FFT 1

  #endif

#if LAMP_CURRENT == LAMP_S7_07_and_08_and_09_and_10

#define NUM_LEDS 15
#define TOUCH_ON 1200
#define TOUCH_OFF 1000
#define SUPPORTS_FFT 1

#endif


#define LAMP_HARDWARE_VERSION "5.1"