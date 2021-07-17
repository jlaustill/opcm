//
// Created by jlaustill on 7/6/21.
//

#include <Arduino.h>
#include <TimerFour.h>
#include "Speedometer.h"

Speedometer::Speedometer(int _clicksPerMile) {
    this->clicksPerMile = _clicksPerMile;
    this->mph = 0;
}

void Speedometer::initialize() const {
    Timer4.initialize();
    Timer4.pwm(TIMER4_C_PIN, 255); // pin 8
}

void Speedometer::SetMph(int _mph) {
    this->mph = _mph;
    long microseconds = Speedometer::MphToMicroseconds(this->mph);
//    Serial.println("mph: " + (String)this->mph);
//    Serial.println("microseconds: " + (String)microseconds);
//    Serial.println();
    Timer4.setPeriod(microseconds);
}

long Speedometer::MphToMicroseconds(int _mph) const {
    double hertz = (double)_mph * this->clicksPerMile / 60 / 60;
//    Serial.pri¿ntln("hertz: " + (String)hertz);
    long microseconds = Speedometer::HertzToMicroseconds(hertz);
    return microseconds;
}

long Speedometer::HertzToMicroseconds(double _hz) {
    double duration;
    duration = 1.000000000 / _hz;
    return (long)(duration * 1000000.0);
}
