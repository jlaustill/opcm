//
// Created by jlaustill on 8/27/21.
//
#include "Configuration.h"

#ifdef NEXTION
#ifndef NPCM_NEXTION_H
#define NPCM_NEXTION_H
#include "Arduino.h"
#include "AppData.h"

class Nextion {
public:
    static void initialize();
    static void updateDisplayData(AppData currentData);
private:
    static void sendCmd(String cmd);
};


#endif //NPCM_NEXTION_H
#endif //NEXTION