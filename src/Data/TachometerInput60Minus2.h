//
// Created by jlaustill on 8/2/21.
//
#include "../Configuration.h"

#ifdef TACHOMETER_INPUT_60_MINUS_2
#ifndef OPCM_TACHOMETER_INPUT_60_MINUS_2_H
#define OPCM_TACHOMETER_INPUT_60_MINUS_2_H


class TachometerInput60Minus2 {
public:
    static void initialize();
    static int getCurrentRpm();
};


#endif //OPCM_TACHOMETER_INPUT_60_MINUS_2_H
#endif
