//
// Created by jlaustill on 4/6/22.
//

#include "TempSensor.h"
#include <Arduino.h>

void TempSensor::computeKelvin() {
    this->kelvin = (1.0f /
            (this->A +
             this->B * log(TempSensor::computedResistorValue) +
             this->C * pow(log(TempSensor::computedResistorValue), 3)
            ));
}

void TempSensor::updateSensor() {
    this->readVoltage = analogRead(this->SensorPin);
    this->computeResistorValue();
    this->computeKelvin();
}

float TempSensor::getTempKelvin() {
    this->updateSensor();
    return this->kelvin;
}

float TempSensor::getTempCelsius() {
    this->updateSensor();
    return this->kelvin - 273.15f;
}

float TempSensor::getTempFahrenheit() {
    this->updateSensor();
    return ((this->kelvin - 273.15f) * 9.0f) / 5.0f + 32.0f;
}

void TempSensor::computeResistorValue() {
    this->computedResistorValue =
            (TempSensor::KnownResistorValue * readVoltage) /
            (1024 - readVoltage);
}
