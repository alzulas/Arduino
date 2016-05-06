// Includes some code from here: https://learn.adafruit.com/chameleon-scarf/code

#include <Adafruit_NeoPixel.h>
#include "Adafruit_TCS34725.h"
#include <Wire.h>
#include "SoftwareSerial.h"

#define N_LEDS 16
#define PIN 6
#define CLEARTHRESHOLD 1000

Adafruit_NeoPixel ring = Adafruit_NeoPixel(N_LEDS, PIN, NEO_GRB + NEO_KHZ800);
Adafruit_TCS34725 sensor = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);

byte gammatable[256];

void setup()
{
  Serial.begin(9600);
  ring.begin();
  ring.setBrightness(100);
  ring.show();
  
  if (!sensor.begin())
  {
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
}

int pos = 0;    // Position

uint16_t clear, red, green, blue;

void loop()
{
  /* Color test - Dev purposes:
  - Collect a color from the color sensor, display that color on the color wheel
  - Used to compare color accuracy between sensor/NeoPixel ring
  - Only really applicable to solo play: users will calibrate the color wheel themselves in group play */
  ring.setBrightness(100);
  sensor.setInterrupt(false);  // turn on LED
      
  delay(50);    // wait 50 ms
      
  sensor.getRawData(&red, &green, &blue, &clear);    // grab colors
      
  sensor.setInterrupt(true);  // turn off LED

  if (clear < CLEARTHRESHOLD)
  {
    colorWipe(ring.Color(0, 0, 0), 200);
    return;
  }
    

      
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
  
  colorWipe(ring.Color(gammatable[(int)r], gammatable[(int)g], gammatable[(int)b]), 200);
}

void colorWipe(uint32_t c, uint8_t wait)
{
  for (uint16_t i = 0; i < ring.numPixels(); i++)
  {
    ring.setPixelColor(i, c);
    ring.show();
  }
  delay(wait);
}
