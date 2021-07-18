//
// Created by jlaustill on 7/18/21.
//

#ifndef OPCM_OBD2_H
#define OPCM_OBD2_H

#include "../../../include/AppData.h"

//#define _MCP_FAKE_MODE_
#define CAN_2515
// #define CAN_2518FD

// Set SPI CS Pin according to your hardware

#if defined(SEEED_WIO_TERMINAL) && defined(CAN_2518FD)
// For Wio Terminal w/ MCP2518FD RPi Hat：
// Channel 0 SPI_CS Pin: BCM 8
// Channel 1 SPI_CS Pin: BCM 7
// Interupt Pin: BCM25
const int SPI_CS_PIN  = BCM8;
const int CAN_INT_PIN = BCM25;
#else

// For Arduino MCP2515 Hat:
// the cs pin of the version after v1.1 is default to D9
// v0.9b and v1.0 is default D10
const int SPI_CS_PIN = 9;
const int CAN_INT_PIN = 2;
#endif





class OBD2 {
public:
    static void initialize();
    static void sendData(AppData currentData);
};


#endif //OPCM_OBD2_H
