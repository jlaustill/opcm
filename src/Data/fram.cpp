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
    uint32_t restarts = 0;
    OPCM_Fram::fram.read(0x0, (uint8_t *)&restarts, sizeof(uint32_t));
    Serial.print("Restarted ");
    Serial.print(restarts++);
    Serial.println(" times");

    // restarts write ++
    OPCM_Fram::fram.writeEnable(true);
    OPCM_Fram::fram.write(0x0, (uint8_t *)&restarts, sizeof(uint32_t));
    OPCM_Fram::fram.writeEnable(false);

    // uint8_t value;
    // for (uint32_t a = 0; a < 256; a++) {
    //   value = fram.read8(a);
    //   if ((a % 32) == 0) {
    //     Serial.print("\n 0x");
    //     Serial.print(a, HEX);
    //     Serial.print(": ");
    //   }
    //   Serial.print("0x");
    //   if (value < 0x1) Serial.print('0');
    //   Serial.print(value, HEX);
    //   Serial.print(" ");
    // }
  } else {
    Serial.println("No SPI FRAM found ... check your connections\r\n");
  }
}

void OPCM_Fram::saveData(AppData *currentData) {
    OPCM_Fram::fram.writeEnable(true);

    OPCM_Fram::fram.write(4, (uint8_t *)&currentData->odometer, sizeof(currentData->odometer));
    OPCM_Fram::fram.write(12, (uint8_t *)&currentData->tripA, sizeof(currentData->tripA));
    OPCM_Fram::fram.write(20, (uint8_t *)&currentData->tripB, sizeof(currentData->tripB));
    OPCM_Fram::fram.write(28, (uint8_t *)&currentData->oilChange, sizeof(currentData->oilChange));
    OPCM_Fram::fram.write(36, (uint8_t *)&currentData->transmissionFluidChange, sizeof(currentData->transmissionFluidChange));
    OPCM_Fram::fram.write(44, (uint8_t *)&currentData->transferCaseFluidChange, sizeof(currentData->transferCaseFluidChange));
    OPCM_Fram::fram.write(52, (uint8_t *)&currentData->frontDifferentialFluidChange, sizeof(currentData->frontDifferentialFluidChange));
    OPCM_Fram::fram.write(60, (uint8_t *)&currentData->rearDifferentialFluidChange, sizeof(currentData->rearDifferentialFluidChange));
    OPCM_Fram::fram.write(68, (uint8_t *)&currentData->fuelFilterChange, sizeof(currentData->fuelFilterChange));
    OPCM_Fram::fram.write(76, (uint8_t *)&currentData->tireRotation, sizeof(currentData->tireRotation));
    OPCM_Fram::fram.write(84, (uint8_t *)&currentData->odometerSaveCount, sizeof(currentData->odometerSaveCount));

    OPCM_Fram::fram.writeEnable(false);
}

AppData OPCM_Fram::loadData() {
    AppData currentData;

    OPCM_Fram::fram.read(4, (uint8_t *)&currentData.odometer, sizeof(currentData.odometer));
    OPCM_Fram::fram.read(12, (uint8_t *)&currentData.tripA, sizeof(currentData.tripA));
    OPCM_Fram::fram.read(20, (uint8_t *)&currentData.tripB, sizeof(currentData.tripB));
    OPCM_Fram::fram.read(28, (uint8_t *)&currentData.oilChange, sizeof(currentData.oilChange));
    OPCM_Fram::fram.read(36, (uint8_t *)&currentData.transmissionFluidChange, sizeof(currentData.transmissionFluidChange));
    OPCM_Fram::fram.read(44, (uint8_t *)&currentData.transferCaseFluidChange, sizeof(currentData.transferCaseFluidChange));
    OPCM_Fram::fram.read(52, (uint8_t *)&currentData.frontDifferentialFluidChange, sizeof(currentData.frontDifferentialFluidChange));
    OPCM_Fram::fram.read(60, (uint8_t *)&currentData.rearDifferentialFluidChange, sizeof(currentData.rearDifferentialFluidChange));
    OPCM_Fram::fram.read(68, (uint8_t *)&currentData.fuelFilterChange, sizeof(currentData.fuelFilterChange));
    OPCM_Fram::fram.read(76, (uint8_t *)&currentData.tireRotation, sizeof(currentData.tireRotation));
    OPCM_Fram::fram.read(84, (uint8_t *)&currentData.odometerSaveCount, sizeof(currentData.odometerSaveCount));

    return currentData;
}

#endif

#endif  // OPCM_DATA_FRAM_CPP