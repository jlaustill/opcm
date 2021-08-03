//
// Created by jlaustill on 7/20/21.
//

#include "SpeedometerInput.h"
#include <Arduino.h>
#include "../Configuration.h"

#ifdef SPEEDOMETER_INPUT
#define INPUT_PIN 18

volatile unsigned long pwm_value = 0;
volatile unsigned long prev_time = 0;
volatile unsigned long thisTime = 0;

void falling() {
    thisTime = micros();
    pwm_value = thisTime - prev_time;
    prev_time = thisTime;
}

void SpeedometerInput::initialize() {
    pinMode(INPUT_PIN, FALLING);
    attachInterrupt(digitalPinToInterrupt(INPUT_PIN), falling, FALLING);
}

double MicrosecondsToHertz(unsigned long _microseconds) {
    double hertz;
    hertz = (double)_microseconds * .000001;
    return (double)(1.0 / hertz);
}

int MicrosecondsToMph(unsigned long _microseconds) {
    double hertz = MicrosecondsToHertz(_microseconds);
    hertz = (hertz * 60 * 60) / SPEEDOMETER_INPUT_CLICKS_PER_MILE;
    return (int)(hertz > 1 ? hertz : 0);
}

int SpeedometerInput::getCurrentSpeedInMph() {
//    Serial.println("pwn_value? " + (String)pwm_value);
    return MicrosecondsToMph(pwm_value < 220000 ? pwm_value : 0);
}

#endif
