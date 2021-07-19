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
//    int transmissionPressure;
//    byte transmissionTempF;
};

#endif //OPCM_APPDATA_H
