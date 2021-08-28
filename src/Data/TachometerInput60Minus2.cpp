//
// Created by jlaustill on 8/2/21.
//
#include <Arduino.h>
//#include <CircularList.h>
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

template <typename T>
class CircularList {
private:
    T* data;
    uint8_t size;
    uint8_t index = 0;

public:
    explicit CircularList(uint8_t _size);
    void next(T _next);
    T average();
};
template<typename T>
CircularList<T>::CircularList(uint8_t _size) {
    this->data = new T[_size];
    this->size = _size;
}

template<typename T>
void CircularList<T>::next(T _next) {
    this->data[this->index] = _next;

    if (++this->index >= this->size) {
        this->index = 0;
    }
}

template<typename T>
T CircularList<T>::average() {
    unsigned long sum = 0;

    for (uint8_t i = 0; i < size; i++) {
        sum += this->data[i];
    }

    return sum / size;
}

CircularList<int> smoothingBuffer(6);

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
    smoothingBuffer.next(rpms);
    int smoothedRpm = smoothingBuffer.average();
//    Serial.println("smoothed RPM: " + (String)smoothedRpm);
    return smoothedRpm;
//return rpms;
}

#endif
