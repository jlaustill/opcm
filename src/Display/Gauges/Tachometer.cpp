//
// Created by jlaustill on 7/4/21.
//
#include "../../Configuration.h"

#ifdef TACHOMETER_OUTPUT
#include <Arduino.h>
#include "Tachometer.h"
IntervalTimer tachTimer;
int tachState = LOW;

void tachSignal() {
    if (tachState == LOW) {
        tachState = HIGH;
    } else {
        tachState = LOW;
    }
    digitalWrite(4, tachState);
}

Tachometer::Tachometer(int _pinNumber) {
    this->numberOfCylinders = TACHOMETER_OUTPUT_CYLINDER_COUNT;
    this->rpms = TACHOMETER_OUTPUT_INITIAL_RPMS;
    this->pinNumber = _pinNumber;
}

void Tachometer::initialize() {
    // analogWrite(this->pinNumber, 255);
    // analogWriteResolution(15);
    pinMode(this->pinNumber, OUTPUT);
    digitalWrite(this->pinNumber, HIGH);
    tachTimer.begin(tachSignal, 150000);
}

void Tachometer::SetRpms(int _rpms) {
    this->rpms = _rpms;
    // long hertz = Tachometer::RpmToHertz(this->rpms);
    // Serial.print("RPMs: "); Serial.print(this->rpms); Serial.print(" pn: "); Serial.print(this->pinNumber); Serial.print(" hertz: "); Serial.println(hertz);
    // tone(4, 100);
    long microseconds = Tachometer::RpmToMicroseconds(this->rpms);
    tachTimer.update(microseconds / 2);
}

long Tachometer::RpmToHertz(int32_t _rpms) {
    double hertz = (double)_rpms * this->numberOfCylinders / 120;
    return hertz;
}

long Tachometer::RpmToMicroseconds(int32_t _rpms) const {
    double hertz = (double)_rpms * this->numberOfCylinders / 120;
//    Serial.println("hertz: " + (String)hertz);
    long microseconds = Tachometer::HertzToMicroseconds(hertz);
    return microseconds;
}

long Tachometer::HertzToMicroseconds(double _hz) {
    double duration;
    duration = 1.000000000 / _hz;
    return (long)(duration * 1000000.0);
}

#endif
