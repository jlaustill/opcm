//
// Created by jlaustill on 5/27/23.
//

#include "PressureSensor.h"
#include <Arduino.h>

void PressureSensor::updateSensor() {
    this->rawValue = ads.computeVolts(ads.readADC_SingleEnded(ChannelId));
}

float PressureSensor::getPressureInPsi() {
    if (!initialized) {
        return 0.0f;
    }

    this->updateSensor();
    float zeroVoltage = .49f;
    float maxVoltage = 3.0f;
    float returnValue = map(this->rawValue, zeroVoltage, maxVoltage, 0, 500);
    return returnValue;
}