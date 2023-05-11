//
// Created by jlaustill on 4/6/22.
//

#include "TempSensor.h"
#include <Arduino.h>

TempSensor::TempSensor(float knownResistorValue, float a, float b, float c, uint8_t deviceId, uint8_t channelId)
    : knownResistorValue_(knownResistorValue),
      a_(a),
      b_(b),
      c_(c),
      channelId_(channelId),
      deviceId_(deviceId),
      readVoltage_(0),
      rawResistorValue_(0),
      kelvin_(0)
{
    if (!ads_.begin(deviceId_)) {
        // throw an exception or return an error code instead of printing to serial
        Serial.println("Failed to initialize ADS.");
    }
}

void TempSensor::computeKelvin() {
    this->kelvin_ = (1.0f /
            (this->a_ +
             this->b_ * log(TempSensor::rawResistorValue_) +
             this->c_ * pow(log(TempSensor::rawResistorValue_), 3)
            ));
}

void TempSensor::updateSensor() {
    const float voltage = ads_.readADC_SingleEnded(channelId_);
    TempSensor::rawResistorValue_ =
            (TempSensor::knownResistorValue_ * voltage) /
            (32767 - voltage);
    this->computeKelvin();
}

float TempSensor::getTempInKelvin() {
    this->updateSensor();
    return this->kelvin_;
}

float TempSensor::getTempInCelsius() {
    this->updateSensor();
    return this->kelvin_ - 273.15f;
}

float TempSensor::getTempInFahrenheit() {
    this->updateSensor();
    return ((this->kelvin_ - 273.15f) * 9.0f) / 5.0f + 32.0f;
}
