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
    int fuelTempF;
    double odometer;
    unsigned long odometerSaveCount;
    double tripA;
    double tripB;
    boolean leftBlinker;
    boolean rightBlinker;
    boolean highBeams;
    boolean waitToStart;
    boolean fourByFour;
    boolean seatBeltWarning;
    boolean doorAjarWarning;
    boolean brakeLightWarning;
};

#endif //OPCM_APPDATA_H
