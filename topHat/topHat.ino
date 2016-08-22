#include <Wire.h>
#include <Adafruit_LSM303.h>

#include <AdafruitNeoPixel.h>
 
#define PIN 4
 
// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
AdafruitNeoPixel strip = AdafruitNeoPixel(11, PIN, NEO_GRB + NEO_KHZ800);
Adafruit_LSM303 lsm;



uint8_t myFavoriteColors[][3] = {
{200,   0, 200},   // purple
{255, 85, 0}, // orange?
{255, 255, 0},	// yellow
{0, 128, 0},	// green
{0, 0, 255},	// blue
{200,   0,   0},   // red 
{200, 200, 200},   // white
                               };
// don't edit the line below
#define FAVCOLORS sizeof(myFavoriteColors) / 3

#define MOVE_THRESHOLD 45
#define FAST 900

// IMPORTANT: Avoid connecting on a live circut...if you must, connect GND first.
 
void setup() {
  Serial.begin(9600);
  
  // Try to initialise and warn if we couldn't detect the chip
  if (!lsm.begin())
  {
    Serial.println("Oops ... unable to initialize the LSM303. Check your wiring!");
    while (1);
  }
  strip.begin();
  strip.setBrightness(80); //adjust brightness here
  strip.show(); // Initialize all pixels to 'off'
}
 
void loop() {
  //rainbow(20);
  lsm.read();
  Serial.print("Accel X: "); Serial.print(lsm.accelData.x); Serial.print(" ");
  Serial.print("Y: "); Serial.print(lsm.accelData.y);       Serial.print(" ");
  Serial.print("Z: "); Serial.print(lsm.accelData.z);     Serial.print(" ");
 
  // Get the magnitude (length) of the 3 axis vector
  // http://en.wikipedia.org/wiki/Euclidean_vector#Length
  double storedVector = lsm.accelData.x*lsm.accelData.x;
  storedVector += lsm.accelData.y*lsm.accelData.y;
  storedVector += lsm.accelData.z*lsm.accelData.z;
  storedVector = sqrt(storedVector);
  Serial.print("Len: "); Serial.println(storedVector);
  
  // wait a bit
  delay(100);
  
  // get new data!
  lsm.read();
  double newVector = lsm.accelData.x*lsm.accelData.x;
  newVector += lsm.accelData.y*lsm.accelData.y;
  newVector += lsm.accelData.z*lsm.accelData.z;
  newVector = sqrt(newVector);
  Serial.print("New Len: "); Serial.println(newVector);
  
  // are we moving 
  if(abs(newVector -storedVector) > FAST)
  {
     rainbow(20); 
  }
  else if(abs(newVector - storedVector) > MOVE_THRESHOLD) 
  {
    Serial.println("Twinkle!");
    flashRandom(30, 1);  // first number is 'wait' delay, shorter num == shorter twinkle
    flashRandom(30, 3);  // second number is how many neopixels to simultaneously light up
    flashRandom(30, 2);
    flashRandom(30, 1);  // first number is 'wait' delay, shorter num == shorter twinkle
    flashRandom(30, 3);  // second number is how many neopixels to simultaneously light up
    flashRandom(30, 2);
  }

}
 
void flashRandom(int wait, uint8_t howmany) {
 
  for(uint16_t i=0; i<howmany; i++) {
    // pick a random favorite color!
    int c = random(FAVCOLORS);
    int red = myFavoriteColors[c][0];
    int green = myFavoriteColors[c][1];
    int blue = myFavoriteColors[c][2]; 
 
    // get a random pixel from the list
    int j = random(strip.numPixels());
    //Serial.print("Lighting up "); Serial.println(j); 
    
    // now we will 'fade' it in 5 steps
    for (int x=0; x < 5; x++) {
      int r = red * (x+1); r /= 5;
      int g = green * (x+1); g /= 5;
      int b = blue * (x+1); b /= 5;
      
      strip.setPixelColor(j, strip.Color(r, g, b));
      strip.show();
      delay(wait);
    }
    // & fade out in 5 steps
    for (int x=5; x >= 0; x--) {
      int r = red * x; r /= 5;
      int g = green * x; g /= 5;
      int b = blue * x; b /= 5;
      
      strip.setPixelColor(j, strip.Color(r, g, b));
      strip.show();
      delay(wait);
    }
  }
}
 
 
void rainbow(uint8_t wait) {
  uint16_t i, j;
 
  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}
 
 
 
// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  if(WheelPos < 85) {
   return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170;
   return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}
