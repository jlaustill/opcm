#define _VERSION_MAJOR 0
#define _VERSION_MINOR 1
#define _VERSION_PATCH 1

#include <Arduino.h>

#include "Domain/opcm.h"

<<<<<<< Updated upstream
void setup() {
=======
//Begin Led Blink

// Teensy 2.0 has the LED on pin 11
// Teensy++ 2.0 has the LED on pin 6
// Teensy 3.x / Teensy LC have the LED on pin 13
const int ledPin = 13;
// End Led Blink

void setup()
{
>>>>>>> Stashed changes
    opcm::setup();
     // Led Blink Setup
  // initialize the digital pin as an output.
  pinMode(ledPin, OUTPUT);
  for(;;)
//Led Blink Setup End

{
 //Led Loop Begins
  digitalWrite(ledPin, HIGH);   // set the LED on
  delay(1000);                  // wait for a second
  digitalWrite(ledPin, LOW);    // set the LED off
  delay(1000);                  // wait for a second
//Led Loop  Ends
}
}

<<<<<<< Updated upstream
__attribute__((unused)) void loop() {
=======
__attribute__((unused)) void loop()

{
>>>>>>> Stashed changes
    opcm::loop();
}

