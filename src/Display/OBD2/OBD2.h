//
// Created by jlaustill on 7/18/21.
//

#ifndef OPCM_OBD2_H
#define OPCM_OBD2_H
#include <FlexCAN_T4.h>
#include "../../../include/AppData.h"

class OBD2 {
public:
    static void initialize();
    static void sendData(const CAN_message_t &msg);
};


#endif //OPCM_OBD2_H
