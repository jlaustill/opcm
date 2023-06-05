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
    this->readVoltage = ads.computeVolts(ads.readADC_SingleEnded(ChannelId));
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
    this->computedResistorValue = (this->readVoltage * TempSensor::KnownResistorValue) / (3.3f - this->readVoltage);
    // Serial.println("computed resistance value? " + (String)this->computedResistorValue + " ReadVoltage? " + (String)this->readVoltage);
}