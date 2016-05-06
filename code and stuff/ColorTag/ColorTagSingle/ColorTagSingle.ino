// Includes some code from here: https://learn.adafruit.com/chameleon-scarf/code

#include <Adafruit_NeoPixel.h>
#include "Adafruit_TCS34725.h"
#include <Wire.h>
#include <stdlib.h>
#include "SoftwareSerial.h"

#define N_LEDS 16
#define PIN 6
#define CLEARTHRESHOLD 1000

Adafruit_NeoPixel ring = Adafruit_NeoPixel(N_LEDS, PIN, NEO_GRB + NEO_KHZ800);
Adafruit_TCS34725 sensor = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);

uint8_t r[15];
uint8_t g[15];
uint8_t b[15];

int numdimmed;

byte gammatable[256];
boolean dimmed[15] = {false};

void setup()
{
  Serial.begin(9600);
  ring.begin();
  ring.setBrightness(40);
  ring.show();
  
  randomSeed(analogRead(0));
  
  if (!sensor.begin())
  {
    while (1);
  }
  numdimmed = 0;
  
  // thanks PhilB for this gamma table!
  // it helps convert RGB colors to what humans see
  for (int i=0; i<256; i++) {
  float x = i;
  x /= 255;
  x = pow(x, 2.5);
  x *= 255;
  gammatable[i] = x; 
  }

  for (int i = 0; i < 15; i++)
  {
    r[i] = random(256) / 6;
    g[i] = random(256) / 6;
    b[i] = random(256) / 6;
  }
}

uint16_t clear, red, green, blue;


void loop()
{
  /* Solo mode - Color scavenger hunt: 16 (randomly-generated) colors light up on the NeoPixel ring.
  The player must find those colors in the world around them and collect them. Collecting a color
  causes its respective light to dim, indicating it has been caught. Once all colors have been
  collected, the player wins. */
  
  sensor.setInterrupt(false);  // turn LED on
  
  delay(50);   // wait 10 ms
  
  sensor.getRawData(&red, &green, &blue, &clear);  // get colors
  
  sensor.setInterrupt(true);    // turn LED off
  
  if (clear < CLEARTHRESHOLD)    // not close enough to properly detect a color
  {
    red = 0;
    green = 0;
    blue = 0;
    clear = 0;
  }
  
  // Figure out some basic hex code for visualization
  uint32_t sum = red;
  sum += green;
  sum += blue;
  sum += clear;
  float re, gr, bl;
  re = red; re /= sum;
  gr = green; gr /= sum;
  bl = blue; bl /= sum;
  re *= 256; gr *= 256; bl *= 256;
  
  ring.setPixelColor(15, ring.Color(gammatable[(int)re], gammatable[(int)gr], gammatable[(int)bl]));
  
  for (int i = 0; i < 15; i++)    // check for a color match to within +-5 RGB values, light up the pixels, dim collected colors by a factor of 3
  {
    if (((r[i] - 10 <= (uint8_t)(red / 257)) && (r[i] + 10 >= (uint8_t)(red / 257))) && ((g[i] - 10 <= (uint8_t)(green / 257)) && (g[i] + 10 >= (uint8_t)(green / 257))) && ((b[i] - 10 <= (uint8_t)(blue / 257)) && (b[i] + 10 >= (uint8_t)(blue / 257))))   // match within +-5 RGB values = color match?
    {
      if (!dimmed[i])
      {
        dimmed[i] = true;
        numdimmed++;
      }
    }
    if (dimmed[i])
    {
      ring.setPixelColor(i, 0, 0, 0);
    }
    else
    {
      ring.setPixelColor(i, r[i], g[i], b[i]);
    }
  }
  if (numdimmed == 15)
  {
    winLoop();
  }
  ring.show();
  delay(100);

}

void colorWipe(uint8_t* r, uint8_t* g, uint8_t* b, uint8_t wait)
{
  for (uint16_t i = 0; i < ring.numPixels(); i++)
  {
    ring.setPixelColor(i, r[i], g[i], b[i]);
    ring.show();
  }
  delay(wait);
}

void winLoop()
{
  uint8_t i[16] = {255, 255, 255, 255, 0, 0, 0, 0, 255, 255, 255, 255, 0, 0, 0, 0};
  uint8_t j[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
  uint8_t k[16] = {0, 0, 0, 0, 255, 255, 255, 255, 0, 0, 0, 0, 255, 255, 255, 255};
  while (1)
  {
    colorWipe(i, j, j, 200);
    colorWipe(j, j, k, 200);
  }
}
