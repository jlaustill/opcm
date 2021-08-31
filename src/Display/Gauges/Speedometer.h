//
// Created by jlaustill on 7/6/21.
//

#ifndef GAUGES_SPEEDOMETER_H
#define GAUGES_SPEEDOMETER_H

#include "../../Configuration.h"

#ifdef SPEEDOMETER_OUTPUT
class Speedometer {
public:
    explicit Speedometer(int _clicksPerMile = SPEEDOMETER_OUTPUT_CLICKS_PER_MILE);
    void initialize() const;
    void SetMph(int _mph);
    static long MphToMicroseconds(int _mph);

private:
    static long HertzToMicroseconds(double _hz);
    int mph;
};
#endif

#endif //GAUGES_SPEEDOMETER_H
