//
// Created by jlaustill on 7/18/21.
//

#include "OBD2.h"

#ifdef CAN_2518FD
#include "mcp2518fd_can.h"
mcp2518fd CAN(SPI_CS_PIN); // Set CS pin
#endif

#ifdef CAN_2515
#include "mcp2515_can.h"
mcp2515_can CAN(SPI_CS_PIN); // Set CS pin
#endif                                    // CAN_2515

#define CAN_ID_PID          0x7E8

int sendCanMessage (int dataSize, const byte *dataToSend) {
//    int dataSizeBits = CHAR_BIT * dataSize;
//    Serial.println("sending data size: " + (String)dataSize);
//    Serial.println("data size in bits: " + (String)dataSizeBits);
    return CAN.sendMsgBuf(CAN_ID_PID, 0, dataSize, dataToSend);
}

byte mphToKph(int mph) {
    double kph = mph * 1.60934;
    return (byte)round(kph);
}

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
//String TransmissionTemperatureRequested = "3,34,22,116,0,0,0,0";
String TransmissionTemperatureRequested = "3,34,17,189,0,0,0,0";

void OBD2::initialize() {

    while (CAN_OK != CAN.begin(CAN_500KBPS)) {
        Serial.println("CAN BUS Shield init fail");
        Serial.println("Init CAN BUS Shield again");
        delay(100);
    }

    Serial.println("CAN BUS Shield init ok!");

}

void OBD2::sendData(AppData currentData) {

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
    unsigned char rpms[7] =                          {4, 65, 12, highByte(currentData.rpm << 2), lowByte(currentData.rpm << 2), 0, 0 }; //224, 185, 147};
    unsigned char vspeed[7] =                       {4, 65, 13, mphToKph(currentData.speedInMph), 224, 185, 147};
    unsigned char fuelPressureControlSystem[8] =    {16, 65, 209, 225, 100, 25, 50, 50};

    while(CAN_MSGAVAIL == CAN.checkReceive())
    {

        CAN.readMsgBuf(&len, buf);
        canId = CAN.getCanId();
        if (canId != 2015) {
            Serial.print("<");
            Serial.print(canId);
            Serial.print(">{");
        }

        for(int i = 0; i<len; i++)
        {
            BuildMessage = BuildMessage + buf[i] + (i == len - 1 ? "" : ",");
        }
        if (canId != 2015) {
            Serial.println(BuildMessage + "}");
        }

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
            unsigned char CoolantTemp[7] = {4, 65, 5,  currentData.coolantTemp, 0, 185, 147};
            Serial.println("sending coolant temp " + (String)currentData.coolantTemp);
            Serial.println("message received " + CoolantTempRequested);
            sendCanMessage(sizeof(CoolantTemp), CoolantTemp);
        }

        if(BuildMessage == TransmissionTemperatureRequested) {
//            unsigned char TransmissionTemperatureData[7] = {5,98,22,116,5,5,0}; // doesn't work :(
            unsigned char TransmissionTemperatureData[7] = {5,98,17,189,5,5,0}; // doesn't work :(
            Serial.println("Sending Transmission Temperature " + (String)currentData.transmissionTempC + " " + sizeof(TransmissionTemperatureData));
            sendCanMessage(7, TransmissionTemperatureData);
        }

        if(BuildMessage == RpmRequested){
//            Serial.println("sending rpm " + (String)currentData.rpm);
            sendCanMessage(sizeof(rpms), rpms);
        }

        if(BuildMessage == SpeedoRequested){
            Serial.println("sending speedo " + (String)currentData.speedInMph);
            sendCanMessage(sizeof(vspeed), vspeed);
        }
        if(BuildMessage == FuelPressureControlSystemRequested) {
            Serial.println("sending FuelPressureControlSystemRequested 50");
            sendCanMessage(sizeof(fuelPressureControlSystem), fuelPressureControlSystem);
//            sendCanMessage(sizeof(fuelPressureControlSystem), fuelPressureControlSystem);
//            sendCanMessage(sizeof(fuelPressureControlSystem), fuelPressureControlSystem);
        }
        BuildMessage="";
    }

    byte canCheckError = CAN.checkError();
    if(canCheckError != 0){
        Serial.println("CAN.checkError != 0" + (String)canCheckError);
    }

}
