//
// Created by jlaustill on 7/6/21.
//

#include "../../Configuration.h"

#ifdef SPEEDOMETER_OUTPUT

#include <Arduino.h>
// #include <TimerFour.h>
#include "Speedometer.h"

Speedometer::Speedometer(int _clicksPerMile) {
    this->mph = 0;
}

void Speedometer::initialize() const {
    // Timer4.initialize();
    // Timer4.pwm(TIMER4_C_PIN, 255); // pin 8
    analogWrite(2, 255);
    analogWriteResolution(15);
}

void Speedometer::SetMph(int _mph) {
    this->mph = _mph;
    long microseconds = Speedometer::MphToMicroseconds(this->mph);
    double hertz = (double)_mph * SPEEDOMETER_OUTPUT_CLICKS_PER_MILE / 60 / 60;
    tone(2, hertz);
//    Serial.println("mph: " + (String)this->mph);
//    Serial.println("microseconds: " + (String)microseconds);
//    Serial.println();
    // Timer4.setPeriod(microseconds);
}

long Speedometer::MphToMicroseconds(int _mph) {
    double hertz = (double)_mph * SPEEDOMETER_OUTPUT_CLICKS_PER_MILE / 60 / 60;
//    Serial.println("hertz: " + (String)hertz);
    long microseconds = Speedometer::HertzToMicroseconds(hertz);
//    Serial.println("microseconds: " + (String(microseconds)));
    return microseconds;
}

long Speedometer::HertzToMicroseconds(double _hz) {
    double duration;
    duration = 1.000000000 / _hz;
    return (long)(duration * 1000000.0);
}

#endif
