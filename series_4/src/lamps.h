

#define TOUCH_ON 2400
#define TOUCH_OFF 1000
#define SUPPORTS_FFT 1


#define LAMP_HARDWARE_VERSION "4.2"

/*
#ifdef SUPPORTS_FFT
#define LAMP_SUPPORTS "<fft=1,2/>"
#endif
*/

#ifndef LAMP_SUPPORTS
#ifndef SUPPORTS_FFT
#define LAMP_SUPPORTS ""
#else
#define LAMP_SUPPORTS "<fft=1,2/>"
#endif // ifndef SUPPORTS_FFT
#endif // ifndef LAMP_SUPPORTS

