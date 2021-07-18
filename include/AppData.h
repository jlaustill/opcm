//
// Created by jlaustill on 7/18/21.
//

#ifndef OPCM_APPDATA_H
#define OPCM_APPDATA_H

#include <Arduino.h>

struct AppData {
    int rpm;
    byte coolantTemp;
    byte speedInMph;
};

#endif //OPCM_APPDATA_H
