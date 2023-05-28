//
// Created by jlaustill on 5/27/23.
//

#ifndef OPCM_PRESSURE_SENSOR_H
#define OPCM_PRESSURE_SENSOR_H
#include <Adafruit_ADS1X15.h>


class PressureSensor {
public:
    PressureSensor(float _psiMax, float _rawZero, float _rawMax, uint8_t deviceId, uint8_t channelId) {
        Serial.println("Starting to initialize ADS. devicedId: " + (String)deviceId);

        DeviceId = deviceId;
        ChannelId = channelId;

        rawValue = 0.0f;
        rawZero = _rawZero;
        rawMax = _rawMax;
        PsiMax = _psiMax;

        if (!ads.begin(DeviceId)) {
            Serial.println("Failed to initialize ADS.");
        } else {
            Serial.println("Succesfuly initialized ADS.");
        }
    }

    PressureSensor() {

    }

    float getPressureInPsi();

private:
    uint8_t DeviceId;
    uint8_t ChannelId;

    float rawValue;
    float rawZero;
    float rawMax;
    float PsiMax;

    void updateSensor();
    Adafruit_ADS1115 ads;
};


#endif //OPCM_PRESSURE_SENSOR_H