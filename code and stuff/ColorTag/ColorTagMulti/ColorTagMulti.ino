// Includes some code from here: https://learn.adafruit.com/chameleon-scarf/code

#include <Adafruit_NeoPixel.h>
#include <Adafruit_TCS34725.h>
#include <Wire.h>

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
  int num_players;
}

int pos = 0;    // Position

uint16_t clear, red, green, blue;



void loop()
{
    /* Group play - Color tag: 
    - Color wheel flashes
    - Click the color button X number of times (up to 4) to indicate X "not It" players
    - Color wheel flashes again
    - Collect the colors of each "not It" person's shirt, once X colors have been collected...
    - Color wheel flashes again, and the game begins
    - As each person is tagged, their color lights up on the color wheel
    - Once all people have been tagged, the color wheel rotates and the game is won */
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
