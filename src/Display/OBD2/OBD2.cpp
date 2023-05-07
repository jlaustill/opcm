//
// Created by jlaustill on 7/18/21.
//
#include "Configuration.h"
#ifdef ODB2
#include "OBD2.h"

#include <FlexCAN_T4.h>
FlexCAN_T4<CAN2, RX_SIZE_256, TX_SIZE_16> Can2;

// void sendCanMessage (int dataSize, const byte *dataToSend) {
// //    int dataSizeBits = CHAR_BIT * dataSize;
// //    Serial.println("sending data size: " + (String)dataSize);
// //    Serial.println("data size in bits: " + (String)dataSizeBits);
//     CAN.sendMsgBuf(CAN_ID_PID, 0, dataSize, dataToSend);
// }

// byte mphToKph(int mph) {
//     double kph = mph * 1.60934;
//     return (byte)round(kph);
// }

// unsigned long canId = 0x000;

// unsigned char len = 0;
// unsigned char buf[8];

// String BuildMessage="";
// String SupportedPIDsRequested = "2,1,0,0,0,0,0,0"; // 00
// String SupportedPIDs21to40Requested = "2,1,32,0,0,0,0,0"; // 32
// String SupportedPIDs41to60Requested = "2,1,64,0,0,0,0,0"; // 64
// String SupportedPIDs61to80Requested = "2,1,96,0,0,0,0,0"; // 96

// String CheckEngineLightClearRequested = "2,1,1,0,0,0,0,0"; // 01
// String CoolantTempRequested = "2,1,5,0,0,0,0,0"; // 05
// String RpmRequested = "2,1,12,0,0,0,0,0"; // 0C
// String SpeedoRequested = "2,1,13,0,0,0,0,0"; // 0D
// String FuelPressureControlSystemRequested = "2,1,109,0,0,0,0,0";
// //String TransmissionTemperatureRequested = "3,34,22,116,0,0,0,0";
// String TransmissionTemperatureRequested = "3,34,17,189,0,0,0,0";

// //GENERAL ROUTINE
// byte SupportedPID[8] =                {0x06,  0x41,  0x00,  B10001000, B00011000, B00000000, B00000001, 0x00};
// //                                                          33-40      41-48      49-56      57-64
// byte SupportedPID21to40[8] =          {0x06,  0x41,  0x20,  B00000000, B00010000, B00000000, B00000001, 0x00};
// //                                                          65-72      73-80      81-88      89-96
// byte SupportedPID41to60[8] =          {0x06,  0x41,  0x40,  B00000000, B00000000, B00000000, B00000001, 0x00};
// //                                                          97-104     105-112    113-120    121-128
// byte SupportedPID61to80[8] =          {0x06,  0x41,  0x60,  B00000000, B00001000, B00000000, B00000000, 0x00};

// unsigned char MilCleared[7] =         {4, 65, 63, 34, 224, 185, 147};

// unsigned char rpmMessage[7] = {4, 65, 12, 0, 0, 0, 0 };
// unsigned char vspeedMessage[7] = {4, 65, 13, 0, 224, 185, 147};

void OBD2::initialize() {
    Serial.println("ODB2 initializing");

    Can2.begin();
    Can2.setBaudRate(500 * 1000);
    Can2.setMaxMB(16);
    Can2.enableFIFO();
    Can2.enableFIFOInterrupt();
    Can2.onReceive(sendData);
    Can2.mailboxStatus();
}


// int only16 = 0;
void OBD2::sendData(const CAN_message_t &msg) {
    if (msg.buf[2] == 0) {
        CAN_message_t msg;
        msg.id = CAN2_ID;
        msg.buf[0] = 0x6;
        msg.buf[1] = 0x41;
        msg.buf[2] = B00000000;
        msg.buf[3] = B00000000;
        msg.buf[4] = B00000000;
        msg.buf[5] = B00000001;
        msg.buf[6] = 0x0;
        Can2.write(msg);
    }
// //    //SENSORS
// //    unsigned char fuelPressureControlSystem[8] =    {16, 65, 209, 225, 100, 25, 50, 50};
// //
//     while(CAN_MSGAVAIL == CAN.checkReceive() && only16++ < 16)
//     {
// ////        Serial.println("CAN_MSGAVAIL");
//         CAN.readMsgBuf(&len, buf);
//         canId = CAN.getCanId();
// //        if (canId != 2015) {
// //            Serial.print("<");
// //            Serial.print(canId);
// //            Serial.print(">{");
// //        }
// //
//         for(int i = 0; i<len; i++)
//         {
//             BuildMessage = BuildMessage + buf[i] + (i == len - 1 ? "" : ",");
//         }
// //        if (canId != 2015) {
// //            Serial.println(BuildMessage + "}");
// //        }
// //
// //        Serial.println(BuildMessage);
//         //Check which message was received.
//         if(BuildMessage == SupportedPIDsRequested) {
//             Serial.println("PIDs requested");
//             sendCanMessage(sizeof(SupportedPID), SupportedPID);
//         }
// //
//         if(BuildMessage == SupportedPIDs21to40Requested) {
// //            Serial.println("requested the 21 to 40!!!!!");
//             sendCanMessage(sizeof(SupportedPID21to40), SupportedPID21to40);
//         }
// //
//         if(BuildMessage == SupportedPIDs41to60Requested) {
// //            Serial.println("requested the 41 to 60!!!!!");
//             sendCanMessage(sizeof(SupportedPID41to60), SupportedPID41to60);
//         }
// //
//         if(BuildMessage == SupportedPIDs61to80Requested) {
//             Serial.println("requested the 61 to 80!!!!!");
//             sendCanMessage(sizeof(SupportedPID61to80), SupportedPID61to80);
//         }
// //
//         if(BuildMessage == CheckEngineLightClearRequested) {
//             Serial.println("sending CIL cleared");
//             sendCanMessage(sizeof(MilCleared), MilCleared);
//         }
// //
// //        //SEND SENSOR STATUSES
// //        if(BuildMessage == CoolantTempRequested) {
// //            unsigned char CoolantTemp[7] = {4, 65, 5,  currentData.coolantTemp, 0, 185, 147};
// ////            Serial.println("sending coolant temp " + (String)currentData.coolantTemp);
// ////            Serial.println("message received " + CoolantTempRequested);
// //            sendCanMessage(sizeof(CoolantTemp), CoolantTemp);
// //        }
// //
// //        if(BuildMessage == TransmissionTemperatureRequested) {
// ////            unsigned char TransmissionTemperatureData[7] = {5,98,22,116,5,5,0}; // doesn't work :(
// //            unsigned char TransmissionTemperatureData[7] = {5,98,17,189,5,5,0}; // doesn't work :(
// ////            Serial.println("Sending Transmission Temperature " + (String)currentData.transmissionTempC + " " + sizeof(TransmissionTemperatureData));
// //            sendCanMessage(7, TransmissionTemperatureData);
// //        }
// //
//         if(BuildMessage == RpmRequested){
//             Serial.println("RPM requested via ODB2");
//             rpmMessage[3] = highByte(currentData.rpm << 2);
//             rpmMessage[4] = lowByte(currentData.rpm << 2);
//             sendCanMessage(sizeof(rpmMessage), rpmMessage);
//         }
// //
//         if(BuildMessage == SpeedoRequested){
// //            Serial.println("sending speedo " + (String)currentData.speedInMph);

//             vspeedMessage[3] = mphToKph(currentData.speedInMph);
//             sendCanMessage(sizeof(vspeedMessage), vspeedMessage);
//         }
// //        if(BuildMessage == FuelPressureControlSystemRequested) {
// ////            Serial.println("sending FuelPressureControlSystemRequested 50");
// //            sendCanMessage(sizeof(fuelPressureControlSystem), fuelPressureControlSystem);
// ////            sendCanMessage(sizeof(fuelPressureControlSystem), fuelPressureControlSystem);
// ////            sendCanMessage(sizeof(fuelPressureControlSystem), fuelPressureControlSystem);
// //        }
//         BuildMessage="";
//     }
// //
// //    byte canCheckError = CAN.checkError();
// //    if(canCheckError != 0){
// //        Serial.println("CAN.checkError != 0" + (String)canCheckError);
// //    }
// only16 = 0;
}
#endif
