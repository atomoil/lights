# Lamps

## Fixing Teensyduino Conflicts

* Both IRRemote and Audio define `STATE_IDLE` so it's necessary to replace all occurances of `STATE_IDLE` with `STATE_WAIT` in `IRRemote` (aka `/Applications/Teensyduino.app/Contents/Java/hardware/teensy/avr/libraries/IRremote/`) 

* `/Applications/Teensyduino.app/Contents/Java/hardware/teensy/avr/libraries/FastLED/platforms/arm/common/m0clockless.h` - renamed `LOOP` to `LOOP_` twice (eg on `#define LOOP            "  loop_%=:"`) - line 221, 

* FastLED has a problem related to `delaycycles()` which [this GitHub issue](https://github.com/FastLED/FastLED/issues/870) says was fixed after 3.3.1 - Teensy includes 3.3.1 - as of 20 Feb 2020 3.3.3 is latest.

## Files

### justLEDS 

is a colour cycle that just runs round and round to check LEDs.

### bt-serial03 

lets you set the teensy as a serial passthrough device so you
can use the Arduino Serial Monitor to send an AT command to set the bt name.

### teensyTouchOnly

is just the touch code for observing the touch waveform
in the Arduino Serial Plotter.
