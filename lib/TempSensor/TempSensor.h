#ifndef TEMP_SENSOR_H
#define TEMP_SENSOR_H

#include <Adafruit_ADS1X15.h>

class TempSensor {
public:
    // Constructor that takes an instance of Adafruit_ADS1115 as a reference
    TempSensor(float knownResistorValue, float a, float b, float c, uint8_t deviceId, uint8_t channelId);

    // Functions to get temperature in Kelvin, Celsius, and Fahrenheit
    float getTempInKelvin();
    float getTempInCelsius();
    float getTempInFahrenheit();

private:
    // Private member variables
    float knownResistorValue_;
    float a_;
    float b_;
    float c_;
    uint8_t channelId_;
    uint8_t deviceId_;
    float readVoltage_;
    float rawResistorValue_;
    float kelvin_;
    Adafruit_ADS1115 ads_;

    // Private member functions
    void updateSensor();
    void computeKelvin();
    void computeResistorValue();
};

#endif // TEMP_SENSOR_H
