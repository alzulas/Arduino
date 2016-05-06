/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.
 
  This example code is in the public domain.
 */
 
// Each pin on the board has blue white purple connected
int farRight = 11;
int right = 10;
int center = 9;
int left = 6;
int farLeft = 5;
int brightness = 0;
int fadeAmount = 5;
int counter = 0;

// the setup routine runs once when you press reset:
void setup() {                
  // initialize the digital pin as an output.
  pinMode(farRight, OUTPUT);  
  pinMode(right, OUTPUT);  
  pinMode(center, OUTPUT);
  pinMode(left, OUTPUT);
  pinMode(farLeft, OUTPUT);
}


void loop()  { 
  // set the brightness of the analog-connected LEDs:
  analogWrite(farRight, brightness);
  analogWrite(farLeft, brightness);
  analogWrite(right, 255-brightness); 
  analogWrite(left, 255-brightness); 
  
  // change the brightness for next time through the loop:
  brightness = brightness + fadeAmount;
 
  // reverse the direction of the fading at the ends of the fade: 
  if (brightness == 0 || brightness == 255) {
    fadeAmount = -fadeAmount; 
    counter++;
  }     
  // wait for 15 milliseconds to see the dimming effect    
  delay(15); 
 
// turns on the other LEDs every four times through the fade by 
  // checking the modulo of the counter.
  // the modulo function gives you the remainder of 
  // the division of two numbers:
  if (counter % 4 == 0) {
    digitalWrite(center, HIGH);
  } else {
   digitalWrite(center, LOW);
  }  
}

// the loop routine runs over and over again forever:
/*void loop() {
  digitalWrite(farRight, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);               // wait for a second
  digitalWrite(farRight, LOW);    // turn the LED off by making the voltage LOW
  delay(500);     // wait for a second
  digitalWrite(right, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);               // wait for a second
  digitalWrite(right, LOW);    // turn the LED off by making the voltage LOW
  delay(500);     // wait for a second
  digitalWrite(center, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);               // wait for a second
  digitalWrite(center, LOW);    // turn the LED off by making the voltage LOW
  delay(500);     // wait for a second
  digitalWrite(left, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);               // wait for a second
  digitalWrite(left, LOW);    // turn the LED off by making the voltage LOW
  delay(500);     // wait for a second
  digitalWrite(farLeft, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);               // wait for a second
  digitalWrite(farLeft, LOW);    // turn the LED off by making the voltage LOW
  delay(500);     // wait for a second
}*/

