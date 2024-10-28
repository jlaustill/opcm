#ifndef OPCM_DATA_FRAM_CPP
#define OPCM_DATA_FRAM_CPP

#include "../Configuration.h"

#ifdef FRAM

#include "./fram.h"

Adafruit_FRAM_SPI OPCM_Fram::fram = Adafruit_FRAM_SPI(10);  // use hardware SPI
bool OPCM_Fram::isInitialized = true;

void OPCM_Fram::initialize() {
  if (OPCM_Fram::fram.begin()) {
    OPCM_Fram::isInitialized = true;
    Serial.println("Found SPI FRAM");

    // Read the first byte
    uint32_t restarts = OPCM_Fram::fram.read8(0x0);
    Serial.print("Restarted ");
    Serial.print(restarts);
    Serial.println(" times");

    // restarts write ++
    OPCM_Fram::fram.writeEnable(true);
    OPCM_Fram::fram.write8(0x0, restarts + 1);
    OPCM_Fram::fram.writeEnable(false);
  } else {
    Serial.println("No SPI FRAM found ... check your connections\r\n");
  }
}

#endif

#endif  // OPCM_DATA_FRAM_CPP