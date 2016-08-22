#include <Wire.h>
#include <SPI.h>
#include <math.h>
#include <Adafruit_LSM9DS0.h>
#include <Adafruit_Sensor.h>  // not used in this demo but required!
#include <Adafruit_NeoPixel.h>

#define N1_LEDS 19 //One of the arm bands has 19, one has 20. 
//Count the number of LEDs on the wrist band, and change this variable accordingly
#define PIN1 9

#define N2_LEDS 37
#define PIN2 6

#define N3_LEDS 34
#define PIN3 5

#define N4_LEDS 41
#define PIN4 3

// NeoPixel strips
// might need to comment out strips 2-4 if not using them
Adafruit_NeoPixel strip1 = Adafruit_NeoPixel(N1_LEDS, PIN1, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip2 = Adafruit_NeoPixel(N2_LEDS, PIN2, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip3 = Adafruit_NeoPixel(N3_LEDS, PIN3, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip4 = Adafruit_NeoPixel(N4_LEDS, PIN4, NEO_GRB + NEO_KHZ800);

// i2c
Adafruit_LSM9DS0 lsm = Adafruit_LSM9DS0();

// You can also use software SPI
//Adafruit_LSM9DS0 lsm = Adafruit_LSM9DS0(13, 12, 11, 10, 9);
// Or hardware SPI! In this case, only CS pins are passed in
//Adafruit_LSM9DS0 lsm = Adafruit_LSM9DS0(10, 9);

void setupSensor()
{
  // 1.) Set the accelerometer range
  lsm.setupAccel(lsm.LSM9DS0_ACCELRANGE_2G);
  //lsm.setupAccel(lsm.LSM9DS0_ACCELRANGE_4G);
  //lsm.setupAccel(lsm.LSM9DS0_ACCELRANGE_6G);
  //lsm.setupAccel(lsm.LSM9DS0_ACCELRANGE_8G);
  //lsm.setupAccel(lsm.LSM9DS0_ACCELRANGE_16G);
  
  // 2.) Set the magnetometer sensitivity
  lsm.setupMag(lsm.LSM9DS0_MAGGAIN_2GAUSS);
  //lsm.setupMag(lsm.LSM9DS0_MAGGAIN_4GAUSS);
  //lsm.setupMag(lsm.LSM9DS0_MAGGAIN_8GAUSS);
  //lsm.setupMag(lsm.LSM9DS0_MAGGAIN_12GAUSS);

  // 3.) Setup the gyroscope
  lsm.setupGyro(lsm.LSM9DS0_GYROSCALE_245DPS);
  //lsm.setupGyro(lsm.LSM9DS0_GYROSCALE_500DPS);
  //lsm.setupGyro(lsm.LSM9DS0_GYROSCALE_2000DPS);
}

// startup all the LED strips
void beginAllTheThings()
{
  strip1.begin();
  strip1.setBrightness(50);
  strip1.show();

  strip2.begin();
  strip2.setBrightness(50);
  strip2.show();
  
  strip3.begin();
  strip3.setBrightness(50);
  strip3.show();

  strip4.begin();
  strip4.setBrightness(50);
  strip4.show();
}

void setup() 
{
  beginAllTheThings();
#ifndef ESP8266
  while (!Serial);     // will pause Zero, Leonardo, etc until serial console opens
#endif
  Serial.begin(9600);
  Serial.println("LSM raw read demo");
  
  // Try to initialise and warn if we couldn't detect the chip
  if (!lsm.begin())
  {
    Serial.println("Oops ... unable to initialize the LSM9DS0. Check your wiring!");
    while (1);
  }
  Serial.println("Found LSM9DS0 9DOF");
  Serial.println("");
  Serial.println("");
}

// set LEDs for the given strip based on its posision 
// (i.e. do we need to wrap around to the beginning of the strip or not) and brightness
void setLeds(Adafruit_NeoPixel *s, int n, int p, int b)
{
  // b is a value between 0-255

  // old brightness scale
  /*if (b > 127)
    s->setBrightness(75);
  else if (b > 63)
    s->setBrightness(65);
  else if (b > 31)
    s->setBrightness(55);
  else if (b > 15)
    s->setBrightness(45);
  else if (b > 7)
    s->setBrightness(35);
  else if (b > 3)
    s->setBrightness(25);
  else if (b > 1)
    s->setBrightness(15);
  else
    s->setBrightness(5);*/

  // just divide by 2.5 to get an (approximately) 0-100 range of stuff
  int bright = b / 2.5;
  // 255 / 2.5 = 102, so we need to set to 100 if it's greater
  if (bright > 100)
    bright = 100;
  // minimum brightness of 5 so the LEDs still show up a little even if the user is not moving
  if (bright < 5)
    bright = 5;
  s->setBrightness(bright);

  //color setting area
  // HOLY IF-STATEMENTS, BATMAN!
  if (p == n - 3)  // p+3 pixel needs to go to 0
    {
      s->setPixelColor(p - 2, 128, 0, 128);
      s->setPixelColor(p - 1, 0, 0, 255);
      s->setPixelColor(p, 0, 128, 0);
      s->setPixelColor(p + 1, 255, 255, 0);
      s->setPixelColor(p + 2, 255, 165, 0);
      s->setPixelColor(0, 255, 0, 0);
    }
    else if (p == n - 2) // p+2 pixel needs to go to 0, p+3 pixel needs to go to 1
    {
      s->setPixelColor(p - 2, 128, 0, 128);
      s->setPixelColor(p - 1, 0, 0, 255);
      s->setPixelColor(p, 0, 128, 0);
      s->setPixelColor(p + 1, 255, 255, 0);
      s->setPixelColor(0, 255, 165, 0);
      s->setPixelColor(1, 255, 0, 0);
    }
    else if (p == n - 1) // p+1 pixel at 0, p+2 pixel at 1, p+3 pixel at 2
    {
      s->setPixelColor(p - 2, 128, 0, 128);
      s->setPixelColor(p - 1, 0, 0, 255);
      s->setPixelColor(p, 0, 128, 0);
      s->setPixelColor(0, 255, 255, 0);
      s->setPixelColor(1, 255, 165, 0);
      s->setPixelColor(2, 255, 0, 0);
    }
    // p won't ever be == s->numPixels()
    else if (p == 0)  // p-1 at numPixels-1, p-2 at numPixels-2
    {
      s->setPixelColor(n - 2, 128, 0, 128);
      s->setPixelColor(n - 1, 0, 0, 255);
      s->setPixelColor(p, 0, 128, 0);
      s->setPixelColor(p + 1, 255, 255, 0);
      s->setPixelColor(p + 2, 255, 165, 0);
      s->setPixelColor(p + 3, 255, 0, 0);
    }
    else if (p == 1)  // p-2 at numPixels-1
    {
      s->setPixelColor(n - 1, 128, 0, 128);
      s->setPixelColor(p - 1, 0, 0, 255);
      s->setPixelColor(p, 0, 128, 0);
      s->setPixelColor(p + 1, 255, 255, 0);
      s->setPixelColor(p + 2, 255, 165, 0);
      s->setPixelColor(p + 3, 255, 0, 0);
    }
    else
    {
      s->setPixelColor(p - 2, 128, 0, 128);
      s->setPixelColor(p - 1, 0, 0, 255);
      s->setPixelColor(p, 0, 128, 0);
      s->setPixelColor(p + 1, 255, 255, 0);
      s->setPixelColor(p + 2, 255, 165, 0);
      s->setPixelColor(p + 3, 255, 0, 0);
    }

    /*s->setPixelColor(p-2, 255, 0, 0);
    s->setPixelColor(p-1, 255, 85, 0);
    s->setPixelColor(p, 255, 255, 0);
    s->setPixelColor(p+1, 0, 128, 0);
    s->setPixelColor(p+2, 0, 0, 255);
    s->setPixelColor(p+3, 128, 0, 128);*/
}

// clear all of the LEDs by setting their color to black
// this doesn't set the brightness value but I don't think we really need to
void clearAllLeds()
{
  for (int k = 0; k < N1_LEDS; k++)
  {
    strip1.setPixelColor(k, 0, 0, 0);
  }
  for (int k = 0; k < N2_LEDS; k++)
  {
    strip2.setPixelColor(k, 0, 0, 0);
  }
  for (int k = 0; k < N3_LEDS; k++)
  {
    strip3.setPixelColor(k, 0, 0, 0);
  }
  for (int k = 0; k < N4_LEDS; k++)
  {
    strip4.setPixelColor(k, 0, 0, 0);
  }
}

// this takes an int value that ranges from 0-65535, and returns a delay value based on that
// this changes the rate at which the rainbow travels along the NeoPixel strip
// so basically moving faster causes the rainbow to move faster
// the scale is honestly kind of arbitrary but it seems to look good
int getVariableDelay(int accel)
{
  // accel is a value from 0-65535

  int del = 100;
    // this config works pretty well
    // 0-1: no delay reduction
    // 2-3: small                       del -= 2 (98)
    // 4-7: more                        del -= 2 (96)
    // 8-15: more                       del -= 4 (92)
    // 16-31: more                      del -= 4 (88)
    // 32-63: you get the idea...       del -= 4 (84)
    // 64-127: ...                      del -= 4 (82)
    // 128-255: ...                     del -= 4 (78)
    
    // 256-511: ...                     del -= 8 (70)
    // 512-1023: ...                    del -= 8 (62)
    // 1024-2047: ...                   del -= 8 (54)
    // 2048-4095: ...                   del -= 16 (38)
    // 4096-8191                        del -= 16 (22)
    // 8192-16383                       del -= 8 (14)
    // 16384-32767                      del -= 2 (12)
    // 32768-65535: max delay reduction del -= 2 (10)
    // this is gonna be a lot of if statements...

    if (accel > 1)
      del -= 2;
    if (accel > 3)
      del -= 2;
    if (accel > 7)
      del -= 4;
    if (accel > 15)
      del -= 4;
    if (accel > 31)
      del -= 4;
    if (accel > 63)
      del -= 4;
    if (accel > 127)
      del -= 4;
    if (accel > 255)
      del -= 8;
    if (accel > 511)
      del -= 8;
    if (accel > 1023)
      del -= 8;
    if (accel > 2047)
      del -= 8;
    if (accel > 4095)
      del -= 16;
    if (accel > 8191)
      del -= 16;
    if (accel > 16383)
      del -= 2;
    if (accel > 32767)
      del -= 2;
    // YUP

    // del ends up being a value between 10-100
    return del;
}

// This needs to range from 0-255 based on accelerometer data
int c = 0, pos1 = 0, pos2 = 0, pos3 = 0, pos4 = 0;

int mode = 2;
// mode:
// 0 = brightness based on accel data
// 1 = speed of light movement based on accel data
// 2 = RAINBOW MODE

void loop() 
{
  // get accelerometer data (and gyroscope and magnetometer data too I guess...)
  lsm.read();
  //int i = 0;  // do I even use this?
  // no, no I don't

  // ~-2200 resting +- 30000?
  int acx = (int)lsm.accelData.x;
  
  // ~-5000 resting, +- 30000?
  int acy = (int)lsm.accelData.y;
  
  // ~15000 resting, +- 30000?
  int acz = (int)lsm.accelData.z;

  // just get the total magnitude
  int total = sqrt(pow(acx, 2) + pow(acy, 2) + pow(acz, 2));
  // when the accelerometer is at rest (on Earth), total = ~16000
  // this would probably be different on other celestial bodies
  // but of course we don't care about those use cases
  // nobody is going to be juggling on the moon with this hardware any time soon
  // we can say total varies between 0 and 65535

  // -= 16000 so total = ~0 when the accelerometer is at rest
  total -= 16000;
  total = abs(total); // abs of total in case we get negative values (it is possible for total to be significantly < 16000)

  if (mode == 0) // all white lights, brightness variable on accel. only one strip
  {
    c = total / 256;

    if (c < 0)
      c = 0;
    else if (c > 255)
      c = 255;

    for (int i = 0; i < N1_LEDS; i++)
    {
      strip1.setPixelColor(pos1 + i, c, c, c);
    }
    strip1.show();
    delay(10);
  }
  else if (mode == 1) // rotating white lights, speed of rotation variable on accel. only one strip
  {
    int del = getVariableDelay(total);

    clearAllLeds();

    //color setting area
    strip1.setPixelColor(pos1-3, 20, 20, 20);
    strip1.setPixelColor(pos1-2, 80, 80, 80);
    strip1.setPixelColor(pos1-1, 165, 165, 165);
    strip1.setPixelColor(pos1, 255, 255, 255);
    strip1.setPixelColor(pos1+1, 165, 165, 165);
    strip1.setPixelColor(pos1+2, 80, 80, 80);
    strip1.setPixelColor(pos1+3, 20, 20, 20);

    strip1.show();
    delay(del);

    pos1++;
    if (pos1 >= strip1.numPixels() - 2)
    pos1 = 0;
  }
  else if (mode == 2) // RAINBOW MODE YEEEEEEEAHHHHHH
  {
    int del = getVariableDelay(total);
    c = total / 256;
    if (c < 0)
      c = 0;
    else if (c > 255)
      c = 255;

    clearAllLeds();


    // need to remember that, for each strip you setLeds() for, you need to call strip.show(), then pos++ and check pos >= N_LEDS
    setLeds(&strip1, N1_LEDS, pos1, c);
    setLeds(&strip2, N2_LEDS, pos2, c);
    setLeds(&strip3, N3_LEDS, pos3, c);
    setLeds(&strip4, N4_LEDS, pos4, c);

    strip1.show();
    strip2.show();
    strip3.show();
    strip4.show();

    delay(del);

    pos1++;
    if (pos1 >= N1_LEDS-2)
      pos1 = 0;
    pos2++;
    if (pos2 >= N2_LEDS-2)
      pos2 = 0;
    pos3++;
    if (pos3 >= N3_LEDS-2)
      pos3 = 0;
    pos4++;
    if (pos4 >= N4_LEDS-2)
      pos4 = 0;
  }

  //test code below.
  /*Serial.print("Accel X: "); Serial.print((int)lsm.accelData.x); Serial.print(" ");
  Serial.print("Y: "); Serial.print((int)lsm.accelData.y);       Serial.print(" ");
  Serial.print("Z: "); Serial.println((int)lsm.accelData.z);     Serial.print(" ");
  Serial.print("Total: "); Serial.println(total);                Serial.print(" ");
  Serial.print("Mag X: "); Serial.print((int)lsm.magData.x);     Serial.print(" ");
  Serial.print("Y: "); Serial.print((int)lsm.magData.y);         Serial.print(" ");
  Serial.print("Z: "); Serial.println((int)lsm.magData.z);       Serial.print(" ");
  Serial.print("Gyro X: "); Serial.print((int)lsm.gyroData.x);   Serial.print(" ");
  Serial.print("Y: "); Serial.print((int)lsm.gyroData.y);        Serial.print(" ");
  Serial.print("Z: "); Serial.println((int)lsm.gyroData.z);      Serial.print(" ");
  Serial.print("Temp: "); Serial.print((int)lsm.temperature);    Serial.println(" ");*/
}
