//
// Created by jlaustill on 8/19/21.
//

#ifndef TRANSMISSION_PRESSURE_INPUT_H
#define TRANSMISSION_PRESSURE_INPUT_H


class TransmissionPressureSensor {
public:
    static int getTransmissionPressureInPsi();

private:
    static float currentRawValue;
};


#endif //TRANSMISSION_PRESSURE_INPUT_H
