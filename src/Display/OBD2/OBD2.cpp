//
// Created by jlaustill on 7/18/21.
// Recreated by jlaustill on 9/22/23.
//
#include "Configuration.h"
#ifdef ODB2

#define ODD_CHECKSUM_A 0x00
#define ODD_CHECKSUM_B 0x03
#define EVEN_CHECKSUM_A 0x01
#define EVEN_CHECKSUM_B 0x02

#include <FlexCAN_T4.h>

#include "../../../include/AppData.h"
#include "./responses.h"
#include "OBD2.h"
#include <SeaDash.hpp>
FlexCAN_T4<CAN2, RX_SIZE_256, TX_SIZE_16> Can2;
unsigned long runtime;
double boost;
double oilPres;
double oilTemp;
double manifoldTemp;
double manifoldPres;
double transTemp;
double transPres;
long odometer;
long tripA;
long tripB;

byte service;
bool requestIsFromScannerRange = false;


byte mphToKph(int mph) {
    double kph = mph * 1.60934;
    return (byte)round(kph);
}

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

void OBD2::sendCumminsObd2Speed(uint8_t speedInMph) {
        uint8_t speedInKph = mphToKph(speedInMph);
        uint16_t multipliedSpeed = speedInKph * 128;
        bool parity = SeaDash::Bits::parity(multipliedSpeed);
        byte checksum = 0;
        
        if (parity == 0) {
          checksum = checksum == EVEN_CHECKSUM_A ? EVEN_CHECKSUM_B : EVEN_CHECKSUM_A;
        } else {
          checksum = checksum == ODD_CHECKSUM_A ? ODD_CHECKSUM_B : ODD_CHECKSUM_A;
        }
        cumminsSpeedResponse.buf[0] = highByte(multipliedSpeed);
        cumminsSpeedResponse.buf[1] = lowByte(multipliedSpeed);
        cumminsSpeedResponse.buf[6] = checksum;
        Can2.write(cumminsSpeedResponse);
}

void OBD2::sendData(const CAN_message_t &msg) {
  service = msg.buf[1];
  requestIsFromScannerRange = msg.id >= 2015 && msg.id <= 2023;

  if (requestIsFromScannerRange && service == 1) {
    switch (msg.buf[2]) {
      case 0:
        Serial.println("ODB2 pids 1-32 requested from " + (String)msg.id);
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
        boost *= 1.003;
        boost *= 220;
        boost -= 10;
        boostResponse.buf[6] = boost < 0 ? 0 : highByte((long)boost);
        boostResponse.buf[7] = boost < 0 ? 0 : lowByte((long)boost);
        Can2.write(boostResponse);
        break;
      case 128:
        Serial.println("ODB2 pids 129-160 requested");
        Can2.write(
            supportedPidsOneHundredTwentyNineToOneHundredOneHundredFiftyResponse);
        break;
      case 166:
        odometer = OBD2::appData->odometer * 1.60934 * 10;
        // Serial.println("Odometer? " + (String)(odometer / 10));
        odometerResponse.buf[3] =
            odometer > 16777216 ? SeaDash::Bytes::getNthByte(odometer, 4) : 0;
        odometerResponse.buf[4] = odometer > 65535 ? SeaDash::Bytes::getNthByte(odometer, 3) : 0;
        odometerResponse.buf[5] = odometer > 255 ? highByte(odometer) : 0;
        odometerResponse.buf[6] = lowByte(odometer);
        Can2.write(odometerResponse);
        break;
      case 251:
        tripA = OBD2::appData->tripA * 1.60934 * 10;
        tripAResponse.buf[3] = tripA > 16777216 ? SeaDash::Bytes::getNthByte(tripA, 4) : 0;
        tripAResponse.buf[4] = tripA > 65535 ? SeaDash::Bytes::getNthByte(tripA, 3) : 0;
        tripAResponse.buf[5] = tripA > 255 ? highByte(tripA) : 0;
        tripAResponse.buf[6] = lowByte(tripA);
        Can2.write(tripAResponse);
        break;
      case 252:
        tripB = OBD2::appData->tripB * 1.60934 * 10;
        tripBResponse.buf[3] = tripB > 16777216 ? SeaDash::Bytes::getNthByte(tripB, 4) : 0;
        tripBResponse.buf[4] = tripB > 65535 ? SeaDash::Bytes::getNthByte(tripB, 3) : 0;
        tripBResponse.buf[5] = tripB > 255 ? highByte(tripB) : 0;
        tripBResponse.buf[6] = lowByte(tripB);
        Can2.write(tripBResponse);
        break;
      case 253:
        oilPres = OBD2::appData->oilPressureInPsi;
        oilPres *= 6.895;
        oilPres *= 10;
        oilResponse.buf[3] = highByte((long)oilPres);
        oilResponse.buf[4] = lowByte((long)oilPres);
        oilTemp = OBD2::appData->oilTempC;
        oilTemp *= 100;
        oilTemp += 32767;
        oilResponse.buf[5] = highByte((long)oilTemp);
        oilResponse.buf[6] = lowByte((long)oilTemp);
        Can2.write(oilResponse);
        break;
      case 254:
        // Serial.println("ODB2 254 requested");
        manifoldPres = OBD2::appData->boost;
        manifoldTemp = OBD2::appData->manifoldTempC;
        manifoldTemp *= 100;
        manifoldTemp += 32767;
        manifoldResponse.buf[3] = highByte((long)manifoldTemp);
        manifoldResponse.buf[4] = lowByte((long)manifoldTemp);
        manifoldPres *= 6.895;
        manifoldPres *= 10;
        manifoldResponse.buf[5] = highByte((long)manifoldPres);
        manifoldResponse.buf[6] = lowByte((long)manifoldPres);
        Can2.write(manifoldResponse);
        break;
      case 255:
        // Serial.println("ODB2 255 requested");
        transPres = OBD2::appData->transmissionPressure;
        transTemp = OBD2::appData->transmissionTempC;
        transPres *= 6.895;
        transPres *= 10;
        transResponse.buf[5] = highByte((long)transPres);
        transResponse.buf[6] = lowByte((long)transPres);
        transTemp *= 100;
        transTemp -= 32767;
        transResponse.buf[3] = highByte((long)transTemp);
        transResponse.buf[4] = lowByte((long)transTemp);
        Can2.write(transResponse);
        break;
      default:
        Serial.println("ODB2 unknown request ID: " + (String)msg.id + " " +
                       (String)msg.buf[0] + " " + (String)msg.buf[1] + " " +
                       (String)msg.buf[2] + " " + (String)msg.buf[3] + " " +
                       (String)msg.buf[4] + " " + (String)msg.buf[5] + " " +
                       (String)msg.buf[6] + " " + (String)msg.buf[7] + " ");
        break;
    }
  } else if (requestIsFromScannerRange && service == 255) {
    if (msg.buf[2] == 1) {
      // long biggy = 987654321;
      // Serial.println("lowByte? " + (String)lowByte(biggy) + " " +
      //                (String)highByte(biggy) + " " + (String)thirdByte(biggy) +
      //                " " + (String)fourthByte(biggy));
      OBD2::appData->tripA = 0;
      Serial.println("Reset Trip A");
    } else if (msg.buf[2] == 2) {
      OBD2::appData->tripB = 0;
      Serial.println("Reset Trip B");
    }

  } else {
    Serial.println("ODB2 unknown request ID: " + (String)msg.id + " " +
                   (String)msg.buf[0] + " " + (String)msg.buf[1] + " " +
                   (String)msg.buf[2] + " " + (String)msg.buf[3] + " " +
                   (String)msg.buf[4] + " " + (String)msg.buf[5] + " " +
                   (String)msg.buf[6] + " " + (String)msg.buf[7] + " ");
  }
}
#endif
