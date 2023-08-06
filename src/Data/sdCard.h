#include <ArduinoJson.h>
#include <Arduino.h>
#include "AppData.h"

class sdCard
{
private:
    String sdCardData;
    StaticJsonDocument<400> inputDoc;
    StaticJsonDocument<400> outputDoc;
public:
    sdCard(/* args */);
    ~sdCard();
    void saveData(AppData* currentData);
};
