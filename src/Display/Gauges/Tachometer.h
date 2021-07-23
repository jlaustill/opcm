//
// Created by jlaustill on 7/4/21.
//
#include <TimerFive.h>
#include "../../Configuration.h"

#ifdef TACHOMETER_OUTPUT

class Tachometer {
public:
    explicit Tachometer(int pinNumber = TIMER5_C_PIN); // pin 44
    void initialize();
    void SetRpms(int _rpms);
    int rpms;

private:
    long RpmToMicroseconds(int32_t _rpms) const;
    static long HertzToMicroseconds(double _hz);
    int numberOfCylinders;
    int pinNumber;
};

#endif //TACHOMETER_OUTPUT
