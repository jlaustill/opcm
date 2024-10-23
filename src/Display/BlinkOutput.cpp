// Created by jlaustill on 7/6/24
#include <Configuration.h>

#ifdef BLINK_OUTPUT

#include "BlinkOutput.h"

#include <Arduino.h>

uint32_t BlinkOutput::lastBlink = 0;

void BlinkOutput::initialize() {
  pinMode(LED_BUILTIN, OUTPUT);  // Teensy 4.1 onboard LED
  lastBlink = millis();
}

void BlinkOutput::blink() {
  uint32_t currentMillis = millis();
  if (currentMillis - lastBlink >= 1000) {
    lastBlink = currentMillis;
    digitalWrite(LED_BUILTIN,
                 !digitalRead(LED_BUILTIN));  // Toggle the LED state
  }
}

#endif  // BLINK_OUTPUT