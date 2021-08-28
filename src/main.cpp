#include <Arduino.h>
#include "Configuration.h"

#ifdef CAN_BUS
    #include "Display/CanBus.h"
#endif
#ifdef CUMMINS_BUS_INPUT
    #include "Data/CumminsBus.h"
#endif
#ifdef SPEEDOMETER_INPUT
    #include "Data/SpeedometerInput.h"
#endif
#ifdef TACHOMETER_INPUT_60_MINUS_2
    #include "Data/TachometerInput60Minus2.h"
#endif
#ifdef TRANSMISSION_PRESSURE_INPUT
    #include "Data/Sensors/TransmissionPressureSensor.h"
#endif
#ifdef TRANSMISSION_TEMPERATURE_INPUT
    #include "Data/Sensors/TransmissionTemperatureSensor.h"
#endif

#ifdef TACHOMETER_OUTPUT
    #include "Display/Gauges/Tachometer.h"
#endif
#ifdef SPEEDOMETER_OUTPUT
    #include "Display/Gauges/Speedometer.h"
#endif

#ifdef ODB2
    #include "Display/OBD2/OBD2.h"
#endif

#ifdef NEXTION
    #include "Display/Nextion.h"
#endif

#include <AppData.h>

AppData currentData;
unsigned long count;
unsigned long lastMillis;
unsigned long thisMillis;

#ifdef TACHOMETER_OUTPUT
    Tachometer tachometer = Tachometer();
#endif
#ifdef SPEEDOMETER_OUTPUT
    Speedometer speedometer = Speedometer();
#endif

void setup() {
    Serial.begin(115200);
    count = 0;
    lastMillis = millis();
    thisMillis = millis();

#ifdef CAN_BUS
    CanBus::initialize();
#endif

#ifdef CUMMINS_BUS_INPUT
    Serial.println("CUMMINS_BUS_INPUT defined");
    CumminsBus::initialize();
#endif
#ifdef SPEEDOMETER_INPUT
    SpeedometerInput::initialize();
#endif
#ifdef TACHOMETER_INPUT_60_MINUS_2
    TachometerInput60Minus2::initialize();
#endif

    currentData.rpm = 0;
    currentData.coolantTemp = 0;
    currentData.speedInMph = 0;
    currentData.transmissionPressure = 0;
    currentData.oilPressureInPsi = 0;
#ifdef TRANSMISSION_TEMPERATURE_INPUT
    currentData.transmissionTempC = 0;
#endif
#ifdef TACHOMETER_OUTPUT
    tachometer.initialize();
#endif
#ifdef SPEEDOMETER_OUTPUT
    speedometer.initialize();
#endif

#ifdef ODB2
    OBD2::initialize();
#endif

#ifdef NEXTION
    Nextion::initialize();
#endif
}

long sweep = 0;
int up = 1;
long maxSweep = 100000;
void newSweepValue() {
    if (up == 1 && sweep < maxSweep) {
        sweep++;
    } else if (up == 1 && sweep >= maxSweep) {
        up = 0;
    } else if (sweep > 0) {
        sweep--;
    } else {
        up = 1;
    }
}

__attribute__((unused)) void loop()
{
    thisMillis = millis();
    count++;
    newSweepValue();
#ifdef CUMMINS_BUS_INPUT
    currentData.rpm = CumminsBus::getCurrentRpms();
    currentData.coolantTemp = CumminsBus::getCurrentWaterTemp();
    currentData.oilPressureInPsi = CumminsBus::getCurrentOilPressure();
//    Serial.println("RPM: " + (String)currentData.rpm);
#endif
#ifdef TACHOMETER_INPUT_60_MINUS_2
    currentData.rpm = TachometerInput60Minus2::getCurrentRpm();
#endif

#ifdef SPEEDOMETER_INPUT
    currentData.speedInMph = SpeedometerInput::getCurrentSpeedInMph(); // map(sweep, 0, maxSweep, 0, 200); // random(0,255);
//    Serial.println("speedInMph? " + (String)currentData.speedInMph);
#endif

#ifdef TRANSMISSION_PRESSURE_INPUT
    currentData.transmissionPressure = TransmissionPressureSensor::getTransmissionPressureInPsi();
#endif

#ifdef TRANSMISSION_TEMPERATURE_INPUT
    currentData.transmissionTempC = TransmissionTemperatureSensor::getTransmissionTemperatureInCelcius();
#endif

#ifdef TACHOMETER_OUTPUT
    tachometer.SetRpms(currentData.rpm);
#endif
#ifdef SPEEDOMETER_OUTPUT
    speedometer.SetMph(currentData.speedInMph);
#endif

#ifdef ODB2
//    OBD2::sendData(currentData);
#endif
#ifdef CAN_BUS
//    if (count % 100 == 0)
//    CanBus::sendRpms(currentData.rpm);
    CanBus::setRpms(currentData.rpm);
    CanBus::setMph(currentData.speedInMph);
    CanBus::setCoolantTemp(currentData.coolantTemp);
    if (thisMillis - lastMillis > 50) {
        // send unpolled data
        CanBus::sendOilPressure(currentData.oilPressureInPsi);
//        Serial.println("Sending Oil Pressure: " + (String)currentData.oilPressureInPsi);
        lastMillis = thisMillis;
    }
#endif

#ifdef NEXTION
    Nextion::updateDisplayData(currentData);
#endif

    if (count > 50000 && count % 50000 == 0) Serial.println("Average Microseconds Per Loop: " + (String)(micros() / count));
};