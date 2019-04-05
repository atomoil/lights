
//-- onboard LED
const int ledPin = 13;

//-- touchRead
// sensPin = 3 new boards, 23 old boards
 //noisePin = 2 new boards, 22 old boards
const int sensPin = 3; int sens = 0; int sBias = 0;
const int noisePin = 2;  int noise = 0; int nBias = 0;

// -- Filter
float filtAlpha = 0.1; 
int filt = 0;       //initialization of EMA S
 
//-- Switch
int holdCount;
boolean sw = true ; boolean oldTouch; boolean isTouch;

// --Timers
const int deltaSw = 10; //check touch every 10mS
elapsedMillis timeSw;



