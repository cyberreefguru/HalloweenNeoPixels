#include <Adafruit_NeoPixel.h>

// Do not remove the include below
#define DOWN   0
#define UP    1
#define RIGHT 0
#define LEFT  1
#define IN    0
#define OUT   1


#define WHITE 0x00FFFFFF
#define BLACK 0x00000000
#define RED   0x00FF0000
#define BLUE  0x000000FF
#define GREEN 0x0000FF00
#define MAGENTA RED | BLUE
#define CYAN  BLUE | GREEN
#define YELLOW  RED | GREEN
#define ORANGE  0xFF3300
#define PURPLE  0x990099

uint32_t Wheel(byte WheelPos);
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(57, 1);
uint8_t globalIntensity = 0xff;


void setWipeColor(uint32_t newColor, uint16_t index, uint32_t onTime, uint32_t offTime, uint8_t clearAfter);
void writeColor(uint32_t color, uint8_t show);
void colorWipe(uint8_t pattern, uint8_t direction, uint32_t color1, uint32_t color2, uint32_t onTime, uint32_t offTime, uint8_t clearAfter, uint8_t clearEnd);
void pattern(uint32_t runDuration, uint8_t pattern, uint8_t direction, uint32_t onColor, uint32_t offColor, uint32_t onTime, uint32_t offTime);
void colorBounce(uint8_t pattern, uint8_t direction, uint32_t color1, uint32_t color2, uint32_t onTime, uint32_t offTime, uint32_t bounceTime, uint8_t clearAfter, uint8_t clearEnd);
void middle(uint8_t direction, uint32_t color1, uint32_t color2, uint32_t onTime, uint32_t offTime, uint8_t clearAfter, uint8_t clearEnd);
void randomFlashColor(uint32_t onTime, uint32_t offTime, uint32_t onColor, uint32_t offColor);
void randomFlash(uint32_t runDuration, uint32_t onTime, uint32_t offTime);
void rainbowFadeOne(uint32_t runDuration, uint32_t fadeTime);
void rainbowFadeStrip(uint32_t runDuration, uint32_t fadeTime);
void commandDelay(uint32_t time);
void checkIntensity();
void fade(uint8_t direction, uint32_t time, uint32_t color);


//The setup function is called once at startup of the sketch
void setup()
{
  // Setup and configure neopixels
  pixels.begin();
  pixels.setBrightness(255);
  writeColor(0x0, true);
}

// The loop function is called in an endless loop
void loop()
{

  fade(UP, 15, 0xFF0000);
  fade(DOWN, 15, 0xFF0000);

  fade(UP, 15, 0x00FF00);
  fade(DOWN, 15, 0x00FF00);
  
  fade(UP, 15, 0x0000FF);
  fade(DOWN, 15, 0x0000FF);

  pixels.setBrightness(globalIntensity);

  rainbowFadeStrip(5000, 75);
  
  rainbowFadeOne(5000, 25);


}


/**
 * Sets strip to specified pattern
 */
void setPattern(uint8_t pattern, uint32_t onColor, uint32_t offColor)
{
  uint16_t i, j;

  j = 0;

  for(i=0; i<pixels.numPixels(); i++)
  {
    if( (pattern>>j) & 0x01 )
    {
      pixels.setPixelColor(i, onColor);
    }
    else
    {
      pixels.setPixelColor(i, offColor);
    }
    j = j+1;
    if( j == 8 )
    {
      j = 0;
    }
  }
  pixels.show();

}

/**
 * Rotates a pattern across the stripe; onTime determines pause between rotation
 */
void pattern(uint32_t runDuration, uint8_t pattern, uint8_t direction, uint32_t onColor, uint32_t offColor, uint32_t onTime, uint32_t offTime)
{
  uint16_t i;
  uint32_t end;
  end = millis() + runDuration;

  i = 0;
  while(millis() < end)
  {
    setPattern(pattern, onColor, offColor);
    commandDelay(onTime);
    if( direction == LEFT )
    {
      if( pattern & 0x80)
      {
        i = 0x01;
      }
      else
      {
        i = 0x00;
      }
      pattern = pattern<<1;
      pattern = pattern | i;
    }
    else if (direction == RIGHT )
    {
      if( pattern & 0x01)
      {
        i = 0x80;
      }
      else
      {
        i = 0x00;
      }
      pattern = pattern>>1;
      pattern = pattern | i;
    }
  }
  printf("pattern: end");
}

/**
 * Cycles all colors through the entire strip.
 *
 *
 */
void rainbowFadeStrip(uint32_t runDuration, uint32_t fadeTime)
{
  uint16_t i, j;
  uint32_t end;
  end = millis() + runDuration;

  while( millis() < end )
  {
    for(j=0; j<256; j++)
    {
      for(i=0; i<pixels.numPixels(); i++)
      {
        pixels.setPixelColor(i, Wheel((j) & 255));
        checkIntensity();
      }
      pixels.show();
      commandDelay(fadeTime);

    } // end for
  }
}

/**
 * Cycles a rainbow through the length of a strip
 *
 */
void rainbowFadeOne(uint32_t runDuration, uint32_t fadeTime)
{
  uint16_t i, j;
  uint32_t end;
  end = millis() + runDuration;

  while( millis() < end )
  {
    for(j=0; j<(256*5); j++) { // 5 cycles of all colors on wheel
      for(i=0; i< pixels.numPixels(); i++) {
        pixels.setPixelColor(i, Wheel(((i * 256 / pixels.numPixels()) + j) & 255));
        checkIntensity();
      }
      pixels.show();
      commandDelay(fadeTime);
    }
  }
}

/**
 * Flashes random LED with random color
 */
void randomFlash(uint32_t runDuration, uint32_t onTime, uint32_t offTime)
{
  uint32_t end;
  end = millis() + runDuration;

  while(millis() < end)
  {
    randomFlashColor(onTime, offTime, Wheel( random(0xff) ), 0x0);
    checkIntensity();
  }

} // randomFlash


/**
 * Flashes random LED with specified color
 */
void randomFlashColor(uint32_t runDuration, uint32_t onTime, uint32_t offTime, uint32_t onColor, uint32_t offColor)
{
  uint8_t i;
  uint32_t end;
  end = millis() + runDuration;

  writeColor( offColor, true);

  while(millis() < end)
  {
    i = random(pixels.numPixels());
    pixels.setPixelColor(i, onColor);
    pixels.show();
    commandDelay(onTime);
    pixels.setPixelColor(i, offColor);
    commandDelay(offTime);
  }

  writeColor( offColor, true);

} // randomFlash


/**
 * Starts in the middle and works out; or starts in the end and works in
 */
void middle(uint8_t direction, uint32_t color1, uint32_t color2, uint32_t onTime, uint32_t offTime, uint8_t clearAfter, uint8_t clearEnd)
{
  uint8_t numPixels = pixels.numPixels();
  uint8_t halfNumPixels = numPixels/2;

  writeColor(color2, true);
  if(direction == IN)
  {
    for(uint8_t i=0; i<halfNumPixels; i++)
    {
      pixels.setPixelColor(i, color1);
      pixels.setPixelColor((numPixels-1)-i, color1);
      pixels.show();
      commandDelay(onTime);

      if( clearAfter == true )
      {
        pixels.setPixelColor(i, color2);
        pixels.setPixelColor((numPixels-1)-i, color2);
        pixels.show();
        commandDelay(offTime);
      }
    }
  }
  else if( direction == OUT )
  {
    for(uint8_t i=0; i<halfNumPixels; i++)
    {
      pixels.setPixelColor(halfNumPixels-i, color1);
      pixels.setPixelColor(halfNumPixels+i, color1);
      pixels.show();
      commandDelay(onTime);

      if( clearAfter == true )
      {
        pixels.setPixelColor(halfNumPixels-i, color2);
        pixels.setPixelColor(halfNumPixels+i, color2);
        pixels.show();
        commandDelay(offTime);
      }
    }
  }
}



/**
 *
 */
void colorBounce(uint8_t pattern, uint8_t direction, uint32_t color1, uint32_t color2, uint32_t onTime, uint32_t offTime, uint32_t bounceTime, uint8_t clearAfter, uint8_t clearEnd)
{
  printf("colorBounce: pattern=0x%02x, direction=%d, colorOn=0x%08lx, colorOff=0x%08lx, onTime=0x%08lx, offTime=0x%08lx, bounceTime=0x%08lx, clearAfter=%d, clearEnd=%d\n",
                   pattern, direction, color1, color2, onTime, offTime, bounceTime, clearAfter, clearEnd);

  if( direction == LEFT )
  {
    colorWipe(pattern, LEFT, color1, color2, onTime, offTime, clearAfter, clearEnd);
    commandDelay(bounceTime);
    colorWipe(pattern, RIGHT, color1, color2, onTime, offTime, clearAfter, clearEnd);
    commandDelay(bounceTime);
  }
  else if (direction == RIGHT )
  {
    colorWipe(pattern, RIGHT, color1, color2, onTime, offTime, clearAfter, clearEnd);
    commandDelay(bounceTime);
    colorWipe(pattern, LEFT, color1, color2, onTime, offTime, clearAfter, clearEnd);
    commandDelay(bounceTime);
  }
}



/**
 * Turns on LEDs one at time in sequence.  LEFT = 0->n; RIGHT = n -> 0
 *
 * @total - number of LEDs to wipe
 * @direction - left (up) or right (down)
 * @color - color to fill LEDs with
 * @waitTime - time to keep LED on
 * @clearAfter - turn LED off after waiting
 * @clearBetween - clear string in between repeats
 */
void colorWipe(uint8_t pattern, uint8_t direction, uint32_t color1, uint32_t color2, uint32_t onTime, uint32_t offTime, uint8_t clearAfter, uint8_t clearEnd)
{

  printf("colorWipe: %d, %d\n\r", clearAfter, clearEnd);
  writeColor(color2, true);
  if( direction == LEFT)
  {
    for(uint8_t i=0; i<pixels.numPixels(); i++)
    {
      setWipeColor(color1, i, onTime, offTime, clearAfter);
      checkIntensity();
    }
  }
  else if (direction == RIGHT )
  {
    for(uint8_t i=pixels.numPixels(); i>0; i--)
    {
      setWipeColor(color1, i, onTime, offTime, clearAfter);
      checkIntensity();
    }
  }

  if( clearEnd )
  {
    writeColor(color2, true);
  }
}

/**
 * Sets the color to wipe string with
 */
void setWipeColor(uint32_t newColor, uint16_t index, uint32_t onTime, uint32_t offTime, uint8_t clearAfter)
{
  uint32_t curColor;

  curColor = pixels.getPixelColor(index);
  pixels.setPixelColor(index, newColor);
  pixels.show();
  commandDelay(onTime);
  if(clearAfter == true)
  {
    pixels.setPixelColor(index,curColor);
    pixels.show();
  }
  commandDelay(offTime);

}

/**
 * sets a color to write to all pixels
 *
 * @color - color to set
 * @show - if true, sets color immediately
 */
void writeColor(uint32_t color, uint8_t show)
{
  for(uint8_t i=0; i<pixels.numPixels(); i++)
  {
    pixels.setPixelColor(i, color);
  }
  if(show)
  {
    pixels.show();
  }
}


void fade(uint8_t direction, uint32_t time, uint32_t color)
{
  uint8_t i;

  for(i=0; i<255; i++)
  {
    if( direction == DOWN )
    {
      pixels.setBrightness(255-i);
    }
    else if( direction == UP)
    {
      pixels.setBrightness(i);
    }
    writeColor( color, true );
    commandDelay(time);
  }

}


/**
 * Input a value 0-255 and get a RGB color back
 */
uint32_t Wheel(byte WheelPos)
{
  if(WheelPos < 85) {
    return pixels.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if(WheelPos < 170) {
    WheelPos -= 85;
    return pixels.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
    WheelPos -= 170;
    return pixels.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}


void commandDelay(uint32_t time)
{
  delay(time);
  checkIntensity();
}

void checkIntensity()
{
  globalIntensity = 0xff;
}

