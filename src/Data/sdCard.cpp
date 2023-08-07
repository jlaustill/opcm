#include "./sdCard.h"
#include <SD.h>
#include <SPI.h>
#include <ArduinoJson.h>

sdCard::sdCard(/* args */) {
    if (!SD.begin(BUILTIN_SDCARD)) {
        Serial.println("SdCard failed or not present!");
        return;
    }

    File sdCardDataFile = SD.open("data.json");

    if (sdCardDataFile) {
        while (sdCardDataFile.available()) {
            sdCard::sdCardData += (char)sdCardDataFile.read();
        }
        sdCardDataFile.close();
        deserializeJson(sdCard::inputDoc, sdCard::sdCardData);
    }
}

sdCard::~sdCard() {
}

void sdCard::saveData(AppData* currentData) {
    sdCard::outputDoc["odometers"]["vehicle"] = currentData->odometer;
    sdCard::outputDoc["odometers"]["tripA"] = currentData->tripA;
    sdCard::outputDoc["odometers"]["tripB"] = currentData->tripB;
    sdCard::outputDoc["odometers"]["oilChange"] = currentData->oilChange;
    sdCard::outputDoc["odometers"]["transmissionFluidChange"] = currentData->transmissionFluidChange;
    sdCard::outputDoc["odometers"]["transferCaseFluidChange"] = currentData->transferCaseFluidChange;
    sdCard::outputDoc["odometers"]["frontDifferentialFluidChange"] = currentData->frontDifferentialFluidChange;
    sdCard::outputDoc["odometers"]["rearDifferentialFluidChange"] = currentData->rearDifferentialFluidChange;
    sdCard::outputDoc["odometers"]["fuelFilterChange"] = currentData->fuelFilterChange;
    sdCard::outputDoc["odometers"]["tireRotation"] = currentData->tireRotation;

    sdCard::outputDoc["saveCount"] = currentData->odometerSaveCount;

    File sdCardDataFile = SD.open("data.json", FILE_WRITE_BEGIN);
    sdCardDataFile.truncate();
    serializeJsonPretty(sdCard::outputDoc, sdCardDataFile);
    sdCardDataFile.close();
    Serial.println("Saved data to sdcard!" + sdCard::outputDoc.as<String>());
}