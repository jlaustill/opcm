//
// Created by jlaustill on 7/6/21.
//

#ifndef GAUGES_SPEEDOMETER_H
#define GAUGES_SPEEDOMETER_H

class Speedometer {
public:
    explicit Speedometer(int _clicksPerMile = 8000);
    void initialize() const;
    void SetMph(int _mph);

private:
    long MphToMicroseconds(int _mph) const;
    static long HertzToMicroseconds(double _hz);
    int clicksPerMile;
    int mph;
    int pinNumber;
};

#endif //GAUGES_SPEEDOMETER_H
