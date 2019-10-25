/*
  Colour palettes - https://coolors.co/
  Spirit of the Rainbow - pl:86:86:153:99:195:100:241:80:289:100

  DOWNLOAD FILTER LIBRARY:
https://github.com/JonHub/Filters

Quit Arduino app. if running.

 put filters folder into local user arduino folder, in my case:
 ian/Documents/Arduino/libraries

#include <Filters.h>

NOTE: all serial coms now at 57600.

IRremote conflicts with Audio library

In IRremote do a search & replace change STATE_IDLE to STATE_OFF (or manually in irRev.cpp, IRremote.cpp & IRremoteInt.h)
