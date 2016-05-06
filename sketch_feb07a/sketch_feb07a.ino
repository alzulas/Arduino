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

  //If it's not bright, you can set the brightness of the strip from 0-255 through strip.setBrightness(...)
  strip.setBrightness(20); //Brightness set way low because battery is TINY. 

}
