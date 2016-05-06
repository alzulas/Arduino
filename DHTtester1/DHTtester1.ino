// Example testing sketch for various DHT humidity/temperature sensors
// Written by ladyada, public domain

#include "DHT.h"

#define DHTPIN1 2     // what digital pin we're connected to
#define DHTPIN2 3     // what digital pin we're connected to
#define DHTPIN3 5     // what digital pin we're connected to
#define DHTPIN4 6     // what digital pin we're connected to
#define DHTPIN5 7     // what digital pin we're connected to


// Uncomment whatever type you're using!
//#define DHTTYPE DHT11   // DHT 11
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)


//Connecting power to 3.5V and ground to one of two ground pins
// Connected pin 2 is sensor 1
// Connected pin 3 is sensor 2
// Connected pin 5 is sensor 3
// Connected pin 6 is sensor 4
// Connected pin 7 is sensor 5

// Initialize DHT sensors
DHT dht1(DHTPIN1, DHTTYPE);
DHT dht2(DHTPIN2, DHTTYPE);
DHT dht3(DHTPIN3, DHTTYPE);
DHT dht4(DHTPIN4, DHTTYPE);
DHT dht5(DHTPIN5, DHTTYPE);
int myround = 1;

void setup() { //intializer 
  Serial.begin(9600);
  Serial.println("DHTxx test!");

  dht1.begin();
  dht2.begin();
  dht3.begin();
  dht4.begin();
  dht5.begin();
}

void loop() {
  // Wait a few seconds between measurements.
  delay(2000);
  DHT dht(0,0);
  switch (myround){ 
  //Each round, ping a different sensor. 
  //Makes the loop more efficient 
    case 1: 
      dht = dht1; 
      Serial.println("Sensor 1");
      myround++; break;
    case 2: 
      dht = dht2; 
      Serial.println("Sensor 2");
      myround++; break;
    case 3: 
      dht = dht3; 
      Serial.println("Sensor 3");
      myround++; break;
    case 4: 
      dht = dht4; 
      Serial.println("Sensor 4");
      myround++; break;
    case 5: 
      dht = dht5; 
      Serial.println("Sensor 5");
      myround = 1; break;
  }
  
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  
 //Usually means it's not plugged in properly, check connections.
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);

  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" *C ");
  Serial.print(f);
  Serial.print(" *F\t");
  Serial.print("Heat index: ");
  Serial.print(hic);
  Serial.print(" *C ");
  Serial.print(hif);
  Serial.println(" *F");
}
