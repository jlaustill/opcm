#include <Arduino.h>
#include <SPI.h>
#include <limits.h>
using namespace std;

#include <Tachometer.h>
#include <Speedometer.h>

Tachometer tachometer = Tachometer(6, 0);
Speedometer speedometer = Speedometer();

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
String SupportedPIDs21to40Requested = "2,1,32,0,0,0,0,0"; // 32
String SupportedPIDs41to60Requested = "2,1,64,0,0,0,0,0"; // 64
String SupportedPIDs61to80Requested = "2,1,96,0,0,0,0,0"; // 96

String CheckEngineLightClearRequested = "2,1,1,0,0,0,0,0"; // 01
String CoolantTempRequested = "2,1,5,0,0,0,0,0"; // 05
String RpmRequested = "2,1,12,0,0,0,0,0"; // 0C
String SpeedoRequested = "2,1,13,0,0,0,0,0"; // 0D
String FuelPressureControlSystemRequested = "2,1,109,0,0,0,0,0";

struct {
    int rpm;
    byte coolantTemp;
    byte speedInMph;
} currentData;

void setup() {
    currentData.rpm = 0;
    currentData.coolantTemp = 0;
    currentData.speedInMph = 0;
    Serial.begin(115200);
    tachometer.initialize();
    speedometer.initialize();

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

void newRpm() {
    int rpmChange = random(-10, 10);
    if (currentData.rpm + rpmChange < 0) {
        currentData.rpm = 0;
    } else if (currentData.rpm + rpmChange > 4000) {
        currentData.rpm = 4000;
    } else {
        currentData.rpm += rpmChange;
    }
}

__attribute__((unused)) void loop()
{
    currentData.coolantTemp = random(1,200);
    newRpm();
    currentData.speedInMph = random(0,255);

    tachometer.SetRpms(currentData.rpm);
    speedometer.SetMph(currentData.speedInMph);

    //GENERAL ROUTINE
    byte SupportedPID[8] =                {0x06,  0x41,  0x00,  B10001000, B00011000, B00000000, B00000001, 0x00};
    //                                                          33-40      41-48      49-56      57-64
    byte SupportedPID21to40[8] =          {0x06,  0x41,  0x20,  B00000000, B00010000, B00000000, B00000001, 0x00};
    //                                                          65-72      73-80      81-88      89-96
    byte SupportedPID41to60[8] =          {0x06,  0x41,  0x40,  B00000000, B00000000, B00000000, B00000001, 0x00};
    //                                                          97-104     105-112    113-120    121-128
    byte SupportedPID61to80[8] =          {0x06,  0x41,  0x60,  B00000000, B00001000, B00000000, B00000000, 0x00};

    unsigned char MilCleared[7] =         {4, 65, 63, 34, 224, 185, 147};

    //SENSORS
    unsigned char CoolantTemp[7] =                  {4, 65, 5,  currentData.coolantTemp, 0, 185, 147};
    unsigned char rpms[7] =                          {4, 65, 12, highByte(currentData.rpm << 2), lowByte(currentData.rpm << 2), 0, 0 }; //224, 185, 147};
    unsigned char vspeed[7] =                       {4, 65, 13, currentData.speedInMph, 224, 185, 147};
    unsigned char fuelPressureControlSystem[7] =    {4, 65, 109, 10, 20, 0, 0};

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
//            int response =
                    sendCanMessage(sizeof(SupportedPID), SupportedPID);
//            Serial.println("response " + (String)response + " : " + (String)CAN_FAILTX);
        }

        if(BuildMessage == SupportedPIDs21to40Requested) {
            Serial.println("requested the 21 to 40!!!!!");
            sendCanMessage(sizeof(SupportedPID21to40), SupportedPID21to40);
        }

        if(BuildMessage == SupportedPIDs41to60Requested) {
            Serial.println("requested the 41 to 60!!!!!");
            sendCanMessage(sizeof(SupportedPID41to60), SupportedPID41to60);
        }

        if(BuildMessage == SupportedPIDs61to80Requested) {
            Serial.println("requested the 61 to 80!!!!!");
            sendCanMessage(sizeof(SupportedPID61to80), SupportedPID61to80);
        }

        if(BuildMessage == CheckEngineLightClearRequested) {
            Serial.println("sending CIL cleared");
            sendCanMessage(sizeof(MilCleared), MilCleared);
        }

        //SEND SENSOR STATUSES
        if(BuildMessage == CoolantTempRequested) {
            Serial.println("sending coolant temp" + (String)currentData.coolantTemp);
            sendCanMessage(sizeof(CoolantTemp), CoolantTemp);
        }

        if(BuildMessage == RpmRequested){
            Serial.println("sending rpm" + (String)currentData.rpm);
            sendCanMessage(sizeof(rpms), rpms);
        }

        if(BuildMessage == SpeedoRequested){
            Serial.println("sending speedo" + (String)currentData.speedInMph);
            sendCanMessage(sizeof(vspeed), vspeed);
        }
        if(BuildMessage == FuelPressureControlSystemRequested) {
            Serial.println("sending FuelPressureControlSystemRequested 50");
            sendCanMessage(sizeof(fuelPressureControlSystem), fuelPressureControlSystem);
            sendCanMessage(sizeof(fuelPressureControlSystem), fuelPressureControlSystem);
            sendCanMessage(sizeof(fuelPressureControlSystem), fuelPressureControlSystem);
        }
        BuildMessage="";
    }
    if(CAN.checkError() != 0){
        Serial.print("CAN.checkError != 0");
    }
}