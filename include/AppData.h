//
// Created by jlaustill on 7/18/21.
//

#ifndef OPCM_APPDATA_H
#define OPCM_APPDATA_H

#include <Arduino.h>

struct AppData {
    byte coolantTemp;
    byte coolantTemp2;
    int rpm;
    byte speedInMph;
    float oilPressureInPsi;
    float oilTempC;
    int transmissionPressure;
    byte transmissionTempC;
    int fuelTempF;
    float boost;
    float manifoldTempC;
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
    double oilChange;
    double transmissionFluidChange;
    double transferCaseFluidChange;
    double frontDifferentialFluidChange;
    double rearDifferentialFluidChange;
    double fuelFilterChange;
    double tireRotation;
};

#endif //OPCM_APPDATA_H
