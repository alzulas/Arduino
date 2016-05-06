#include <Adafruit_NeoPixel.h>

#define PIN 1 //Special requirement, what is this? 
#define NUM_LIGHTS 26//Defines the total number of LEDs

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LIGHTS, PIN, NEO_GRB + NEO_KHZ800); //Defines a NeoPixel opject, our strip of lights. Parameters = number of lights, pin #, and special color code thing

void setup()
{
 strip.begin(); //Prepares strip for data, ***use strip.show() to initialize all pixels to off***
 strip.show(); 
}

void loop() 
{
  //Test to set a pixel color- arguments are: (LED #, red, green, blue)
  strip.setPixelColor(0, 255, 0, 255); //Sets first LED (at position 0) to magenta (all red and blue) 

  strip.show(); 

  strip.setBrightness(100); //set brightness lower
  
  colorWipe(0, 100);
  
 
}

 void colorWipe(uint32_t c, uint8_t wait) 
  {
    for(uint16_t i=0; i < NUM_LIGHTS; i++) 
    {
      strip.setPixelColor(i - 2, 0x100000); // Dark red
      strip.setPixelColor(i - 1, 0x800000); // Medium red
      strip.setPixelColor(i, 0xFF3000); // Center pixel is brightest
      strip.setPixelColor(i + 1, 0xFF3000); // Center pixel is brightest
      strip.setPixelColor(i + 2, 0x800000); // Medium red
      strip.setPixelColor(i + 3, 0x100000); // Dark red
      strip.show();
      for(int j=-3; j<= 3; j++) strip.setPixelColor(i+j, 0);
      delay(wait);
    }
  }
