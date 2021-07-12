#include <Arduino.h>
#include <SPI.h>
#include <limits.h>
using namespace std;

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


#ifdef CAN_2518FD
#include "mcp2518fd_can.h"
mcp2518fd CAN(SPI_CS_PIN); // Set CS pin
#endif

#ifdef CAN_2515
#include "mcp2515_can.h"
mcp2515_can CAN(SPI_CS_PIN); // Set CS pin
#endif                                    // CAN_2515

#define CAN_ID_PID          0x7E8

unsigned long canId = 0x000;

unsigned char len = 0;
unsigned char buf[8];

String BuildMessage="";
String SupportedPIDsRequested = "2,1,0,0,0,0,0,0"; // 00
String CheckEngineLightClearRequested = "2,1,1,0,0,0,0,0"; // 01
String CoolantTempRequested = "2,1,5,0,0,0,0,0"; // 05
String RpmRequested = "2,1,12,0,0,0,0,0"; // 0C
String SpeedoRequested = "2,1,13,0,0,0,0,0"; // 0D

void setup() {
    Serial.begin(115200);

    while (CAN_OK != CAN.begin(CAN_500KBPS)) {
        Serial.println("CAN BUS Shield init fail");
        Serial.println("Init CAN BUS Shield again");
        delay(100);
    }

    Serial.println("CAN BUS Shield init ok!");
}

int count = 0;
int sendCanMessage (int dataSize, const byte *dataToSend) {
    int dataSizeBits = CHAR_BIT * dataSize;
//    Serial.println("sending data size: " + (String)dataSize);
//    Serial.println("data size in bits: " + (String)dataSizeBits);
    return CAN.sendMsgBuf(CAN_ID_PID, 0, dataSize, dataToSend);
}

void loop()
{
    unsigned char rndCoolantTemp=random(1,200);
    unsigned char rndRPM=random(1,55);
    unsigned char rndSpeed=random(0,255);

    //GENERAL ROUTINE
    byte SupportedPID[7] =                {0x06,  0x41,  0x00,  B10001000, B00011000, B00000000, B00000000};
    unsigned char MilCleared[7] =         {4, 65, 63, 34, 224, 185, 147};

    //SENSORS
    unsigned char CoolantTemp[7] =        {4, 65, 5,  rndCoolantTemp, 0, 185, 147};
    unsigned char rpm[7] =                {4, 65, 12, rndRPM, 224, 185, 147};
    unsigned char vspeed[7] =             {4, 65, 13, rndSpeed, 224, 185, 147};

    while(CAN_MSGAVAIL == CAN.checkReceive())
    {

        CAN.readMsgBuf(&len, buf);
        canId = CAN.getCanId();
        Serial.print("<");Serial.print(canId);Serial.print(">{");

        for(int i = 0; i<len; i++)
        {
            BuildMessage = BuildMessage + buf[i] + (i == len - 1 ? "" : ",");
        }
        Serial.println(BuildMessage + "}");

        //Check which message was received.
        if(BuildMessage == SupportedPIDsRequested) {
//            Serial.println("sending supported pids!" + (String)(1 + count++) + " times.");
            int response = sendCanMessage(sizeof(SupportedPID), SupportedPID);
//            Serial.println("response " + (String)response + " : " + (String)CAN_FAILTX);
        }

        if(BuildMessage == CheckEngineLightClearRequested) {
            Serial.println("sending CIL cleared");
            sendCanMessage(sizeof(MilCleared), MilCleared);
        }

        //SEND SENSOR STATUSES
        if(BuildMessage == CoolantTempRequested) {
            Serial.println("sending coolant temp" + (String)rndCoolantTemp);
            sendCanMessage(sizeof(CoolantTemp), CoolantTemp);
        }

        if(BuildMessage == RpmRequested){
            Serial.println("sending rpm" + (String)rndRPM);
            sendCanMessage(sizeof(rpm), rpm);
        }

        if(BuildMessage == SpeedoRequested){
            Serial.println("sending speedo" + (String)rndSpeed);
            sendCanMessage(sizeof(vspeed), vspeed);
        }
        BuildMessage="";
    }
    if(CAN.checkError() != 0){
        Serial.print("CAN.checkError != 0");
    }
}