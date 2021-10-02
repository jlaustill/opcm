//
// Created by jlaustill on 8/21/21.
//
#ifdef CUMMINS_BUS_INPUT
#ifndef NPCM_CUMMINSBUS_H
#define NPCM_CUMMINSBUS_H


class CumminsBus {
public:
    static void initialize();
    static int getCurrentRpms();
    static int getCurrentWaterTemp();
    static byte getCurrentOilPressure();
    static int getCurrentFuelTemp();
};


#endif //NPCM_CUMMINSBUS_H
#endif
