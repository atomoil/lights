
void updateLightDots(){
  //Serial.print(">> ");
  // loop through all of the dots
  for (int c = 0; c < NUM_COLUMNS; c++){
    for (int d = 0; d < NUM_LEDS; d++ ){
      LightDot dot = lights[ c ][ d ];
      // update the dot
      dot.currentValue = dot.currentValue + dot.increment;
      // check for hitting boundaries
      if ((dot.increment < 0) && (dot.currentValue < dot.minimumValue)){
        // if we're heading down and have hit the bottom
        dot.currentValue = dot.minimumValue;
        dot.increment = abs( dot.increment );
        // if we're animating, then recycle the dot
        dot = dotHasReachedLowestValue( dot );
      } else if ((dot.increment > 0) && dot.currentValue > dot.maximumValue){
        // if we've heading up and have hit the top
        dot.currentValue = dot.maximumValue;
        dot.increment = -abs( dot.increment );
        dot = dotHasReachedHighestValue( dot );
      }
      lights[ c ][ d ] = dot;
      
      // apply the colour to the LED
      // value < 0 means off, greater than 255 is on 
      // keep colour within bounds as dot can go higher and lower (to stay on or off for longer)
      if (dot.currentValue > 0){
        int rawVal = min( 255, dot.currentValue );
        int val = int(float(rawVal) * dotBrightness);
        CHSV colour = CHSV( dot.hue, dot.sat, val );
        //CHSV colour = CHSV( 125, 255, val );
        leds[ c ][ dot.led ] = colour;
      } else {
        leds[ c ][ dot.led ] = CRGB::Black;
      }
    }
  }
  //Serial.println(". ");
}

void resetPaletteOnAllDots(){
  for (int c = 0; c < NUM_COLUMNS; c++){
    for (int d = 0; d < NUM_LEDS; d++ ){
      LightDot dot = lights[ c ][ d ];
      dot = resetPalette(dot);
      lights[ c ][ d ] = dot;
    }
  }
}

LightDot dotHasReachedLowestValue(LightDot dot){
  switch ( currentState ){
    case STATE_TOUCH_ON:
      // slow down the dot
      dot.increment = max( min(dot.increment - 0.5, 10.5), 2.0 ); // no more than 10.5, so it drops fast and then slows
      dot.sat = min(dot.sat + 10,255);

      break;
    case STATE_ON_ANIMATED:
      // switch to the next colour in the palette
      dot.colourId = dot.colourId + 1;
      if (dot.colourId >= totalPalettes){
        dot.colourId = 0;
      }
      dot.hue = palette[ dot.colourId ][ PALETTE_HUE ];
      dot.sat = palette[ dot.colourId ][ PALETTE_SATURATION ];
    
      break;
  }
  return dot;
}

LightDot dotHasReachedHighestValue(LightDot dot){
  switch ( currentState ){
    case STATE_TOUCH_INITIAL:
      allLightsBreathing();
      break;
  }
  return dot;
}

LightDot resetPalette(LightDot dot){
  dot.hue = palette[ dot.colourId ][ PALETTE_HUE ];
  dot.sat = palette[ dot.colourId ][ PALETTE_SATURATION ];
  return dot;
}
