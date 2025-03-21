//
// Created by jlaustill on 8/21/21.
//
#ifndef NPCM_CUMMINSBUS_H
#define NPCM_CUMMINSBUS_H
#include <FlexCAN_T4.h>
#include <J1939Message.h>
#include <AppData.h>

struct CanMessage {
  byte id;
  byte length;
  byte data[8];
  unsigned count;
};

#ifdef CUMMINS_BUS_INPUT
class CumminsBus {
 public:
  static AppData *appData;
  static void initialize(AppData *appData);
  static void loop();
  static void updateRpms();
  static int getCurrentRpms();
  static int getCurrentWaterTemp();
  static byte getCurrentOilPressure();
  static int getCurrentFuelTemp();
  static int getCurrentBoostInPsi();
  static int getCurrentBoostTemp();
  static void updateLoad();
  static int getCurrentLoad();
  static void updateThrottlePercentage();
  static int getCurrentThrottlePercentage();
  static int getCurrentAMT();
  static float getCurrentFuelPercentage();
  static float getCurrentEgtTemp();
  static float getCurrentFuelPressurePsi();
  static void updateTiming();
  static float getCurrentTiming();
  static void updateTiming(CAN_message_t &msg);
  static byte getTransmissionTempC();
  static byte getVehicleSpeed();
  static char getRequestedRange();
  static int8_t getCurrentGear();
  static int8_t getSelectedGear();
  static J1939Message ElectronicTransmissionController1Pgn;
};

#endif  // NPCM_CUMMINSBUS_H
#endif
