#include <ArduinoJson.h>
#include <Arduino.h>
#include "AppData.h"

class sdCard
{
private:
    String sdCardData;
    StaticJsonDocument<200> inputDoc;
    StaticJsonDocument<200> outputDoc;
public:
    sdCard(/* args */);
    ~sdCard();
    void saveData(AppData* currentData);
};
