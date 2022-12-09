//
// Created by jlaustill on 8/19/21.
//

#include "Configuration.h"
#ifdef TRANSMISSION_PRESSURE_INPUT
#include "TransmissionPressureSensor.h"
#include "Arduino.h"


float TransmissionPressureSensor::currentRawValue = 0.0f;

int TransmissionPressureSensor::getTransmissionPressureInPsi() {
    TransmissionPressureSensor::currentRawValue = (float)analogRead(TRANSMISSION_PRESSURE_INPUT_PIN);
//    Serial.println(TransmissionPressureSensor::currentRawValue);
    return
            (int)((TransmissionPressureSensor::currentRawValue - TRANSMISSION_PRESSURE_INPUT_RAW_ZERO) *
            TRANSMISSION_PRESSURE_INPUT_PSI_MAX /
            (TRANSMISSION_PRESSURE_INPUT_RAW_MAX - TRANSMISSION_PRESSURE_INPUT_RAW_ZERO));
}
#endif
