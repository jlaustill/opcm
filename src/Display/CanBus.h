//
// Created by jlaustill on 8/21/21.
//

#ifndef NPCM_CANBUS_H
#define NPCM_CANBUS_H


class CanBus {
public:
    static void initialize();
    static void setRpms(int _rpms);
    static void setMph(int _mph);
    static void setCoolantTemp(int _temp);
    static void sendOilPressure(byte _oilPressure);
};

#endif //NPCM_CANBUS_H
