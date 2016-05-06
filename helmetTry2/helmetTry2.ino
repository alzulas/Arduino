#include <Adafruit_NeoPixel.h>
 
#define PIN 13 
 
Adafruit_NeoPixel strip = Adafruit_NeoPixel(35, PIN, NEO_GRB + NEO_KHZ800);
 
int snakeLength=10; // How long of trails do you want?
int frameDelay = 30; // Lower numbers = faster animation
int waveDirection = 0; // 0 = right, 1 = left
 
int rgb[3] = {255,0,0}; // Color for the main pixel
int trailingRGB[3] = {0,0,0}; // Color for the trailing pixels
 
int hue = 60; // We're going to start off with red
 
int currentLED = 0; // Current LED.
int trailingLED = 0; // LED that is trailing the current LED
 
uint16_t i, j;
 
void setup() {
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}
 
void loop() {  
  if (currentLED > 0 && currentLED <= strip.numPixels()) {
    strip.setPixelColor(currentLED, rgb[0], rgb[1], rgb[2]);
  }
  if (waveDirection == 0) { // Wave travelling right
    for (j = 0; j < snakeLength; j++) {
      trailingLED = currentLED - j;
      if (trailingLED > 0) {
        hsi2rgb(hue,1,1/(j+0.1),trailingRGB);
        if(trailingLED > 0 && trailingLED <=strip.numPixels()) {
          strip.setPixelColor(trailingLED, trailingRGB[0], trailingRGB[1], trailingRGB[2]);
        }
      }
    }
    trailingLED--;
    if(trailingLED > 0) {
      strip.setPixelColor(trailingLED,0,0,0);
    }
  } else { // Wave travelling left
    for (j = 0; j < snakeLength; j++) {
      trailingLED = currentLED + j;
      if (trailingLED < strip.numPixels()) {
        hsi2rgb(hue,1,1/(j+0.1),trailingRGB);
        if(trailingLED > 0 && trailingLED <=strip.numPixels()) {
          strip.setPixelColor(trailingLED, trailingRGB[0], trailingRGB[1], trailingRGB[2]);
        }
      }
    }
    trailingLED++;
    if(trailingLED < strip.numPixels()) {
      strip.setPixelColor(trailingLED,0,0,0);
    }
  }
 
  strip.show();
  
  if (waveDirection == 0) {
    currentLED++;
  } else {
    currentLED--;
  }
  if (currentLED > strip.numPixels()) {
    waveDirection = 1;
    currentLED-=2;
  }
  if (currentLED < 0) {
    waveDirection = 0;
    currentLED+=2;
  }
  hue+=4;
  if(hue > 360) {
    hue = 0;
  }
  delay(frameDelay);
}
 
// (FUNCTION STOLEN FROM http://blog.saikoled.com/post/43693602826/why-every-led-light-should-be-using-hsi-colorspace on 10/27/14)
// This function will make it easier to create lighter and darker versions of the same color.
// 
// Function example takes H, S, I, and a pointer to the 
// returned RGB colorspace converted vector. It should
// be initialized with:
//
// int rgb[3];
//
// in the calling function. After calling hsi2rgb
// the vector rgb will contain red, green, and blue
// calculated values.
 
void hsi2rgb(float H, float S, float I, int* rgb) {
  int r, g, b;
  H = fmod(H,360); // cycle H around to 0-360 degrees
  H = 3.14159*H/(float)180; // Convert to radians.
  S = S>0?(S<1?S:1):0; // clamp S and I to interval [0,1]
  I = I>0?(I<1?I:1):0;
    
  // Math! Thanks in part to Kyle Miller.
  if(H < 2.09439) {
    r = 255*I/3*(1+S*cos(H)/cos(1.047196667-H));
    g = 255*I/3*(1+S*(1-cos(H)/cos(1.047196667-H)));
    b = 255*I/3*(1-S);
  } else if(H < 4.188787) {
    H = H - 2.09439;
    g = 255*I/3*(1+S*cos(H)/cos(1.047196667-H));
    b = 255*I/3*(1+S*(1-cos(H)/cos(1.047196667-H)));
    r = 255*I/3*(1-S);
  } else {
    H = H - 4.188787;
    b = 255*I/3*(1+S*cos(H)/cos(1.047196667-H));
    r = 255*I/3*(1+S*(1-cos(H)/cos(1.047196667-H)));
    g = 255*I/3*(1-S);
  }
  rgb[0]=r;
  rgb[1]=g;
  rgb[2]=b;
}
