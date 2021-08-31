//
// Created by jlaustill on 7/18/21.
//

#ifndef OPCM_APPDATA_H
#define OPCM_APPDATA_H

#include <Arduino.h>

struct AppData {
    byte coolantTemp;
    int rpm;
    byte speedInMph;
    byte oilPressureInPsi;
    int transmissionPressure;
    byte transmissionTempC;
    double odometer;
    unsigned long odometerSaveCount;
    double tripA;
    double tripB;
};

#endif //OPCM_APPDATA_H
