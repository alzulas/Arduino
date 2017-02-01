// Source: https://learn.adafruit.com/larson-scanner-shades/software

    #include <Adafruit_NeoPixel.h>
     
    #define N_LEDS 24
    #define PIN 1
     
    // Parameter 1 = number of pixels in strip
    // Parameter 2 = pin number (most are valid)
    // Parameter 3 = pixel type flags, add together as needed:
    // NEO_KHZ800 800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
    // NEO_KHZ400 400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
    // NEO_GRB Pixels are wired for GRB bitstream (most NeoPixel products)
    // NEO_RGB Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
    Adafruit_NeoPixel strip = Adafruit_NeoPixel(N_LEDS, PIN, NEO_GRB + NEO_KHZ800);
     
    void setup() {
    strip.begin();
    strip.setBrightness(100);
    strip.show();
    }
     
    int pos = 0, dir = 1; // Position, direction of "eye"
    int mode = 1;
     
    void loop() {
    int j;


    if (mode == 1)
    {
      //    
      //               Rainbow mode
      //
        if (dir < 0)    // If dir is positive draw red -> purple
        {
            strip.setPixelColor(pos - 2, 255, 0, 0);
            strip.setPixelColor(pos - 1, 255, 85, 0);
            strip.setPixelColor(pos, 255, 255, 0);
            strip.setPixelColor(pos + 1, 0, 128, 0);
            strip.setPixelColor(pos + 2, 0, 0, 255);
            strip.setPixelColor(pos + 3, 128, 0, 128);
        }
        else            // If dir is negative, draw purple -> red
        {
            strip.setPixelColor(pos - 2, 128, 0, 128);
            strip.setPixelColor(pos - 1, 0, 0, 255);
            strip.setPixelColor(pos, 0, 128, 0);
            strip.setPixelColor(pos + 1, 255, 255, 0);
            strip.setPixelColor(pos + 2, 255, 165, 0);
            strip.setPixelColor(pos + 3, 255, 0, 0);
        }
    }
    else
    {
      //
      //              Cylon mode
      //

        strip.setPixelColor(pos - 3, 20, 0, 0);
        strip.setPixelColor(pos - 2, 80, 0, 0);
        strip.setPixelColor(pos - 1, 165, 0, 0);
        strip.setPixelColor(pos, 255, 0, 0);
        strip.setPixelColor(pos + 1, 165, 0, 0);
        strip.setPixelColor(pos + 2, 80, 0, 0);
        strip.setPixelColor(pos + 3, 20, 0, 0);
    }
    
    strip.show();
    delay(40);
     
    // Rather than being sneaky and erasing just the tail pixel,
    // it's easier to erase it all and draw a new one next time.
    for(j=-3; j<= 3; j++) strip.setPixelColor(pos+j, 0);
     
    // Bounce off ends of strip
    pos += dir;
    if(pos < 0) {
    pos = 1;
    dir = -dir;
    } else if(pos >= strip.numPixels() - 2) {
    pos = strip.numPixels() - 4;
    dir = -dir;
    }
    }
