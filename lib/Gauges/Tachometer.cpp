//
// Created by jlaustill on 7/4/21.
//

#include <Arduino.h>
#include <TimerFive.h>

#include "Tachometer.h"

Tachometer::Tachometer(int _numberOfCylinders, int _initialRpms, int _pinNumber) {
    this->numberOfCylinders = _numberOfCylinders;
    this->rpms = _initialRpms;
    this->pinNumber = _pinNumber;
}

void Tachometer::initialize() {
    Timer5.initialize(75);
    Timer5.pwm((char)this->pinNumber, 1023);
}

void Tachometer::SetRpms(int _rpms) {
    this->rpms = _rpms;
    long microseconds = Tachometer::RpmToMicroseconds(this->rpms);
    Serial.println("rpm: " + (String)this->rpms + " microseconds: " + (String)microseconds);
    Serial.println();
    Timer5.setPeriod(microseconds);
}

long Tachometer::RpmToMicroseconds(int32_t _rpms) const {
    double hertz = (double)_rpms * this->numberOfCylinders / 120;
    Serial.println("hertz: " + (String)hertz);
    long microseconds = Tachometer::HertzToMicroseconds(hertz);
    return microseconds;
}

long Tachometer::HertzToMicroseconds(double _hz) {
    double duration;
    duration = 1.000000000 / _hz;
    return (long)(duration * 1000000.0);
}
