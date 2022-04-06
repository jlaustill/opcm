//
// Created by jlaustill on 4/6/22.
//

#ifndef OPCM_TEMPSENSOR_H
#define OPCM_TEMPSENSOR_H


class TempSensor {
public:
    TempSensor(float knownResistorValue, int sensorPin, float a, float b, float c) {
        KnownResistorValue = knownResistorValue;
        SensorPin = sensorPin;
        A = a;
        B = b;
        C = c;
    }

    float getTempKelvin();
    float getTempCelsius();
    float getTempFahrenheit();

private:
    float KnownResistorValue;
    int SensorPin;
    float A;
    float B;
    float C;


    float readVoltage;
    float computedResistorValue;
    float kelvin;
    void updateSensor();
    void computeKelvin();
    void computeResistorValue();

};


#endif //OPCM_TEMPSENSOR_H
