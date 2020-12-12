
//const int touchTriggerOn = 700; // 700 // 1800
//const int touchTriggerOff = 500; // 500
#define TOUCH_ON 700
#define TOUCH_OFF 500
// #define SUPPORTS_FFT 1

//char hardware_version[] = "4.2";
#define LAMP_HARDWARE_VERSION "4.2"

#ifdef SUPPORTS_FFT
//char supports[] = "<fft=1,2/>";
#define LAMP_SUPPORTS "<fft=1,2/>"
#endif

#ifndef SUPPORTS_FFT
//char supports[] = "";
#define LAMP_SUPPORTS ""
#endif

