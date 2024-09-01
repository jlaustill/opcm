
#ifndef J1939_GET_VEHICLE_SPEED_CPP
#define J1939_GET_VEHICLE_SPEED_CPP
#include <Arduino.h>

#include <SeaDash.hpp>

#include "../../Configuration.h"
#include "../CumminsBus.h"

SeaDash::Uint32::IncrementalCircularAverage speedAverage(10);

byte CumminsBus::getVehicleSpeed() {
  // Compute Vehicle Speed
  uint32_t speedRaw =
      (CumminsBus::ElectronicTransmissionController1Pgn.data[2] << 8) |
      CumminsBus::ElectronicTransmissionController1Pgn.data[1];
  uint32_t outputShaftRpm = speedRaw * .125;
  float wheelRpm = outputShaftRpm / 3.73;
  float inchesPerMinute = wheelRpm * 100.11;
  float milesPerHour = inchesPerMinute * 60 / 63360;

  speedAverage.addValue(static_cast<byte>(milesPerHour));

  return speedAverage.getAverage();
}

#endif  // J1939_GET_VEHICLE_SPEED_CPP
