//
// Created by jlaustill on 5/27/23.
//

#include "PressureSensor.h"
#include <Arduino.h>

void PressureSensor::updateSensor() {
    this->rawValue = ads.readADC_SingleEnded(ChannelId);
}

float PressureSensor::getPressureInPsi() {
    this->updateSensor();
    // Serial.println("raw value? " + (String)this->rawValue);
    return (int)((this->rawValue - this->rawZero) *
            this->PsiMax /
            (this->rawMax - this->rawZero));
}