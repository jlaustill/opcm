//
// Created by jlaustill on 8/2/21.
//
#include <Arduino.h>
#include "../Configuration.h"
#include "TachometerInput60Minus2.h"

#ifdef TACHOMETER_INPUT_60_MINUS_2
#define INPUT_PIN 19

volatile unsigned long previous_pwm_value2 = 0;
volatile unsigned long pwm_value2 = 0;
volatile unsigned long prev_time2 = 0;
volatile unsigned long thisTIme2 = 0;
volatile int rpms = 0;
volatile bool synced = false;
volatile uint8_t tooth = 1;

#define MICROSECONDS_IN_ONE_MINUTE 60000000

int rpmFromMicroseconds(unsigned long microseconds) {
    return MICROSECONDS_IN_ONE_MINUTE / (microseconds * 60) * 2;
}

void falling19() {
    thisTIme2 = micros();
    pwm_value2 = thisTIme2 - prev_time2;
    prev_time2 = thisTIme2;

    if (previous_pwm_value2 != 0 && pwm_value2 > previous_pwm_value2 * 2) {
        // first tooth after gap
        tooth = 1;
        synced = true;
    }

    if (tooth++ > 1) {
        rpms = rpmFromMicroseconds(pwm_value2);
    }
    previous_pwm_value2 = pwm_value2;
}

void TachometerInput60Minus2::initialize() {
    pinMode(INPUT_PIN, FALLING);
    attachInterrupt(digitalPinToInterrupt(INPUT_PIN), falling19, FALLING);
}

int TachometerInput60Minus2::getCurrentRpm() {
//    Serial.println("sending rpms: " + (String)rpms);
    return rpms;
}

#endif
