//
// Created by jlaustill on 7/4/21.
//
#include <TimerFive.h>

#ifndef GAUGES_TACHOMETER_H
#define GAUGES_TACHOMETER_H

class Tachometer {
public:
    explicit Tachometer(int _numberOfCylinders, int initialRpms = 1, int pinNumber = TIMER5_C_PIN); // pin 44
    void initialize();
    void SetRpms(int _rpms);
    int rpms;

private:
    long RpmToMicroseconds(int32_t _rpms) const;
    static long HertzToMicroseconds(double _hz);
    int numberOfCylinders;
    int pinNumber;
};

#endif //GAUGES_TACHOMETER_H
