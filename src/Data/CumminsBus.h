//
// Created by jlaustill on 8/21/21.
//
#ifdef CUMMINS_BUS_INPUT
#ifndef NPCM_CUMMINSBUS_H
#define NPCM_CUMMINSBUS_H
#include <FlexCAN_T4.h>

class CumminsBus {
public:
    static void initialize();
    static void updateRpms();
    static int getCurrentRpms();
    static int getCurrentWaterTemp();
    static byte getCurrentOilPressure();
    static int getCurrentFuelTemp();
    static int getCurrentBoostInPsi();
    static int getCurrentLoad();
    static void updateThrottlePercentage();
    static int getCurrentThrottlePercentage();
    static int getCurrentAMT();
    static float getCurrentFuelPercentage();
    static void updateTiming();
    static float getCurrentTiming();
    static void updateTiming(CAN_message_t &msg);
};


#endif //NPCM_CUMMINSBUS_H
#endif
