//
// Created by jlaustill on 8/19/21.
//

#include "TransmissionPressureSensor.h"
#include "Arduino.h"
#include "Configuration.h"

float TransmissionPressureSensor::currentRawValue = 0.0f;

int TransmissionPressureSensor::getTransmissionPressureInPsi() {
    TransmissionPressureSensor::currentRawValue = (float)analogRead(TRANSMISSION_PRESSURE_INPUT_PIN);
    return
            (int)((TransmissionPressureSensor::currentRawValue - TRANSMISSION_PRESSURE_INPUT_RAW_ZERO) *
            TRANSMISSION_PRESSURE_INPUT_PSI_MAX /
            (TRANSMISSION_PRESSURE_INPUT_RAW_MAX - TRANSMISSION_PRESSURE_INPUT_RAW_ZERO));
}
