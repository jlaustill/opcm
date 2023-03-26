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
    int boost;
    double odometer;
    unsigned long odometerSaveCount;
    double tripA;
    double tripB;
    double egt;
    boolean leftBlinker;
    boolean rightBlinker;
    boolean highBeams;
    boolean waitToStart;
    boolean fourByFour;
    boolean seatBeltWarning;
    boolean doorAjarWarning;
    boolean brakeLightWarning;
    float timing;
    float fuelPercentage;
    float amt;
    int throttlePercentage;
    int load;
    int oilChange;
    int transmissionFluidChange;
    int transferCaseFluidChange;
    int frontDifferentialFluidChange;
    int rearDifferentialFluidChange;
    int fuelFilterChange;
    int tireRotation;
};

#endif //OPCM_APPDATA_H
