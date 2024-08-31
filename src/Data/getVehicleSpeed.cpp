
#include <Arduino.h>

#include "Configuration.h"
#include "CumminsBus.h"

byte CumminsBus::getVehicleSpeed() {
  // Compute Vehicle Speed
  uint32_t speedRaw = (pgn61442.data[2] << 8) | pgn61442.data[1];
  uint32_t outputShaftRpm = speedRaw * .125;
  float wheelRpm = outputShaftRpm / 3.73;
  float inchesPerMinute = wheelRpm * 100.11;
  float milesPerHour = inchesPerMinute * 60 / 63360;

  return static_cast<byte>(milesPerHour);
}