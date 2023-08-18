#include "./sdCard.h"
#include <SD.h>
#include <SPI.h>
#include <ArduinoJson.h>

sdCard::sdCard(/* args */)
{
    if (!SD.begin(BUILTIN_SDCARD))
    {
        Serial.println("SdCard failed or not present!");
        return;
    }

    File sdCardDataFile = SD.open("data.json");

    if (sdCardDataFile)
    {
        while (sdCardDataFile.available())
        {
            sdCard::sdCardData += (char)sdCardDataFile.read();
        }
        sdCardDataFile.close();
        deserializeJson(sdCard::inputDoc, sdCard::sdCardData);
    }
}

sdCard::~sdCard()
{
}

AppData sdCard::loadData()
{
    File sdCardDataFile = SD.open("data.json");
    DeserializationError error = deserializeJson(sdCard::inputDoc, sdCardDataFile);
    if (error)
    {
        Serial.println(F("Failed to read config file, using default configuration"));
    }
    sdCardDataFile.close();

    AppData currentData;

    currentData.odometer = sdCard::inputDoc["odometers"]["vehicle"] | 0;
    currentData.tripA = sdCard::inputDoc["odometers"]["tripA"] | 0;
    currentData.tripB = sdCard::inputDoc["odometers"]["tripB"] | 0;
    currentData.oilChange = sdCard::inputDoc["odometers"]["oilChange"] | 0;
    currentData.transmissionFluidChange = sdCard::inputDoc["odometers"]["transmissionFluidChange"] | 0;
    currentData.transferCaseFluidChange = sdCard::inputDoc["odometers"]["transferCaseFluidChange"] | 0;
    currentData.frontDifferentialFluidChange = sdCard::inputDoc["odometers"]["frontDifferentialFluidChange"] | 0;
    currentData.rearDifferentialFluidChange = sdCard::inputDoc["odometers"]["rearDifferentialFluidChange"] | 0;
    currentData.fuelFilterChange = sdCard::inputDoc["odometers"]["fuelFilterChange"] | 0;
    currentData.tireRotation = sdCard::inputDoc["odometers"]["tireRotation"] | 0;

    currentData.odometerSaveCount = sdCard::inputDoc["saveCount"] | 0;

    return currentData;
}

void sdCard::saveData(AppData *currentData)
{
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