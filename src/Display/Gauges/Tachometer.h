//
// Created by jlaustill on 7/4/21.
//
#include "../../Configuration.h"

#ifdef TACHOMETER_OUTPUT

class Tachometer {
public:
    explicit Tachometer(int pinNumber = 4);
    void initialize();
    void SetRpms(int _rpms);
    int rpms;

private:
    long RpmToMicroseconds(int32_t _rpms) const;
    static long HertzToMicroseconds(double _hz);
    long RpmToHertz(int32_t _rpms);
    int numberOfCylinders;
    int pinNumber;
};

#endif //TACHOMETER_OUTPUT
