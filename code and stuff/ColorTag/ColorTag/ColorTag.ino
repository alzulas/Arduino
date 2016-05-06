// Includes some code from here: https://learn.adafruit.com/chameleon-scarf/code

#include <Adafruit_NeoPixel.h>
#include <Adafruit_TCS34725.h>
#include <Wire.h>
#include <stdlib.h>
#include <Time.h>

#define N_LEDS 16
// #define pins

Adafruit_NeoPixel ring = Adafruit_NeoPixel(N_LEDS, 6, NEO_GRB + NEO_KHZ800);
Adafruit_TCS34725 sensor = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);

const int mode = 2; // Mode (0 = solo play, 1 = group play, 2 = color test)

typedef struct color
{
  int r;
  int g;
  int b;
} COLOR;

byte gammatable[256];

void setup()
{
  ring.begin();
  ring.setBrightness(100);
  ring.show();
  
  if (sensor.begin())
  {
    Serial.println("Found sensor");
  }
  else
  {
    Serial.println("No sensor found");
    while (1);
  }
  
  // thanks PhilB for this gamma table!
  // it helps convert RGB colors to what humans see
  for (int i=0; i<256; i++) {
  float x = i;
  x /= 255;
  x = pow(x, 2.5);
  x *= 255;
  gammatable[i] = x; 
  }
  
  switch(mode)    // Initialise variables for different modes (save memory
  {
    case 0:    // single
    {
      COLOR cArr[16];

      for (int i = 0; i < 16; i++)
      {
        cArr[i].r = rand() % 256;
        cArr[i].g = rand() % 256;
        cArr[i].b = rand() % 256;
      }
    }
    case 1:    // multi
    {
      int num_players
    }
    case 2:    // test
    {
    }
  }
}

int pos = 0;    // Position

uint16_t clear, red, green, blue;



void loop()
{
  switch(mode)
  {
    /* Solo mode - Color scavenger hunt: 16 (randomly-generated) colors light up on the NeoPixel ring.
    The player must find those colors in the world around them and collect them. Collecting a color
    causes its respective light to dim, indicating it has been caught. Once all colors have been
    collected, the player wins. */
    case 0:
    {
      
    }
    
    /* Group play - Color tag: 
    - Color wheel flashes
    - Click the color button X number of times (up to 4) to indicate X "not It" players
    - Color wheel flashes again
    - Collect the colors of each "not It" person's shirt, once X colors have been collected...
    - Color wheel flashes again, and the game begins
    - As each person is tagged, their color lights up on the color wheel
    - Once all people have been tagged, the color wheel rotates and the game is won */
    case 1:
    {
    }
    
    /* Color test - Dev purposes:
    - Collect a color from the color sensor, display that color on the color wheel
    - Used to compare color accuracy between sensor/NeoPixel ring
    - Only really applicable to solo play: users will calibrate the color wheel themselves in group play */
    case 2:
    {
      ring.setBrightness(100);
      sensor.setInterrupt(false);  // turn on LED
      
      delay(50);    // wait 50 ms
      
      sensor.getRawData(&red, &green, &blue, &clear);    // grab colors
      
      sensor.setInterrupt(true);  // turn off LED
      
      // Figure out some basic hex code for visualization
      uint32_t sum = red;
      sum += green;
      sum += blue;
      sum += clear;
      float r, g, b;
      r = red; r /= sum;
      g = green; g /= sum;
      b = blue; b /= sum;
      r *= 256; g *= 256; b *= 256;
      
      colorWipe(ring.Color(gammatable[(int)r], gammatable[(int)g], gammatable[(int)b]), 10);
    }
    
    // Do nothing?
    default:
    {
    }
  }
}

void colorWipe(uint32_t c, uint8_t wait)
{
  for (uint16_t i = 0; i < ring.numPixels(); i++)
  {
    ring.setPixelColor(i, c);
    ring.show();
    delay(wait);
  }
}

