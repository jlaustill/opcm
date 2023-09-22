//
// Created by jlaustill on 7/18/21.
// Recreated by jlaustill on 9/22/23.
//
#include "Configuration.h"
#ifdef ODB2
#include <FlexCAN_T4.h>

#include "../../../include/AppData.h"
#include "./responses.h"
#include "OBD2.h"
FlexCAN_T4<CAN2, RX_SIZE_256, TX_SIZE_16> Can2;
unsigned long runtime;
unsigned long boost;

// byte mphToKph(int mph) {
//     double kph = mph * 1.60934;
//     return (byte)round(kph);
// }

AppData *OBD2::appData;

void OBD2::initialize(AppData *currentData) {
  Serial.println("ODB2 initializing");

  appData = currentData;

  Can2.begin();
  Can2.setBaudRate(500 * 1000);
  Can2.setMaxMB(16);
  Can2.enableFIFO();
  Can2.enableFIFOInterrupt();
  Can2.onReceive(sendData);
  Can2.mailboxStatus();
}

void OBD2::sendData(const CAN_message_t &msg) {
  if (msg.id == 2015 && msg.buf[1] == 1) {
    switch (msg.buf[2]) {
      case 0:
        Serial.println("ODB2 pids 1-40 requested from " + (String)msg.id);
        Can2.write(supportedPidsOneToThirtyTwoResponse);
        break;
      case 1:
        Can2.write(monitorStatusResponse);
        break;
      case 4:
        engineLoadTempResponse.buf[3] = OBD2::appData->load * 2.55;
        Can2.write(engineLoadTempResponse);
        break;
      case 5:
        waterTempResponse.buf[3] = OBD2::appData->coolantTemp;
        Can2.write(waterTempResponse);
        break;
      case 12:
        rpmResponse.buf[3] = highByte(OBD2::appData->rpm << 2);
        rpmResponse.buf[4] = lowByte(OBD2::appData->rpm << 2);
        Can2.write(rpmResponse);
        break;
      case 13:
        speedResponse.buf[3] = OBD2::appData->speedInMph;
        Can2.write(speedResponse);
        break;
      case 14:
        timingResponse.buf[3] = (OBD2::appData->timing + 64) * 2;
        Can2.write(timingResponse);
        break;
      case 17:
        throttleResponse.buf[3] = OBD2::appData->throttlePercentage * 2.55;
        Can2.write(throttleResponse);
        break;
      case 28:
        Serial.println("ODB2 standard requested");
        Can2.write(obdStandardResponse);
        break;
      case 31:
        // I don't have code to determine engine start time yet, so just using
        // the time the app started
        runtime = millis() / 1000;
        runtimeResponse.buf[3] = highByte(runtime);
        runtimeResponse.buf[4] = lowByte(runtime);
        Can2.write(runtimeResponse);
        break;
      case 32:
        Serial.println("ODB2 pids 33-64 requested");
        Can2.write(supportedPidsThirtyThreeToSixtyFourResponse);
        break;
      case 64:
        Serial.println("ODB2 pids 65-96 requested");
        Can2.write(supportedPidsSixtyFiveToNinetySixResponse);
        break;
      case 81:
        Serial.println("ODB2 fuel type requested");
        Can2.write(fuelTypeResponse);
        break;
      case 92:
        oilTempResponse.buf[3] = OBD2::appData->oilTempC + 40;
        Can2.write(oilTempResponse);
        break;
      case 96:
        Serial.println("ODB2 pids 97-128 requested");
        Can2.write(supportedPidsNinetySevenToOneHundredTwentyEightResponse);
        break;
      case 112:
        boost = OBD2::appData->boost;
        boost *= 220;
        boost -= 10;
        boostResponse.buf[6] = highByte(boost);
        boostResponse.buf[7] = lowByte(boost);
        Can2.write(boostResponse);
        break;
      default:
        Serial.println("ODB2 unknown request" + (String)msg.buf[2]);
        break;
    }
  }
}
#endif
