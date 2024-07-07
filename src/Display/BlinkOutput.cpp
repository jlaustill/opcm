// Created by jlaustill on 7/6/24

#include "BlinkOutput.h"

#include <Arduino.h>

long BlinkOutput::lastBlink = 0;

void BlinkOutput::initialize() {
  pinMode(LED_BUILTIN, OUTPUT);  // Teensy 4.1 onboard LED
  lastBlink = millis();
}

void BlinkOutput::blink() {
  long currentMillis = millis();
  if (currentMillis - lastBlink >= 1000) {
    lastBlink = currentMillis;
    digitalWrite(LED_BUILTIN,
                 !digitalRead(LED_BUILTIN));  // Toggle the LED state
  }
}
