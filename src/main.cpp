#define _VERSION_MAJOR 0
#define _VERSION_MINOR 1
#define _VERSION_PATCH 1

#include <Arduino.h>
#include "Configuration.h"
#include <EEPROM.h>

#include "Domain/setup.h"

#include <AppData.h>

void setup() {
    Serial.begin(115200);
    count = 0;
    lastMillis = millis();
    thisMillis = millis();
    thisDuration = 0;
    thisMileage = 0;
    pinMode(LEFT_BLINKER_PIN, INPUT_PULLUP);
    pinMode(RIGHT_BLINKER_PIN, INPUT_PULLUP);
    pinMode(HIGH_BEAMS_PIN, INPUT_PULLUP);
    pinMode(WAIT_TO_START_PIN, INPUT_PULLUP);
    // pinMode(FOUR_BY_FOUR_PIN, INPUT_PULLUP);
    // pinMode(SEAT_BELT_WARNING_PIN, INPUT_PULLUP);
    // pinMode(DOOR_AJAR_WARNING_PIN, INPUT_PULLUP);
    // pinMode(BRAKE_LIGHT_PIN, INPUT_PULLUP);

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
    currentData.fuelTempF = 0;

//     currentData.odometer = 307000;
// currentData.tripA = 0;
// currentData.tripB = 0;

//             EEPROM.put(0, currentData.odometer);
//             EEPROM.put(8, currentData.tripA);
//             EEPROM.put(16, currentData.tripB);
//             EEPROM.put(24, 0);

   EEPROM.get(0, currentData.odometer);
   EEPROM.get(8, currentData.tripA);
   EEPROM.get(16, currentData.tripB);
   EEPROM.get(24, currentData.odometerSaveCount);
// currentData.odometer = 307000;
// currentData.tripA = 0;
// currentData.tripB = 0;

    





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

#ifdef THERMOCOUPLE
    if (!thermocouple.begin()) {
        Serial.println("ERROR INITING THERMOCOUPLE");
    } else {
        Serial.println("Thermocouple initialized");
    }
#endif
}

long sweep = 0;
int up = 1;
long maxSweep = 50;
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
    thisDuration = thisMillis - lastMillis;
    count++;
    newSweepValue();
    // Serial.print(sweep);
    // Serial.print(" ");
    // Serial.println(count);
    // delay(1000);

    currentData.speedInMph = sweep;
    currentData.rpm = sweep * 100;
// Serial.print("time to debug "); Serial.println(digitalRead(LEFT_BLINKER_PIN));
    currentData.leftBlinker = digitalRead(LEFT_BLINKER_PIN) == LOW;
    currentData.rightBlinker = digitalRead(RIGHT_BLINKER_PIN) == LOW;
    currentData.highBeams = digitalRead(HIGH_BEAMS_PIN) == LOW;
    currentData.waitToStart = digitalRead(WAIT_TO_START_PIN) == LOW;
    // currentData.fourByFour = digitalRead(FOUR_BY_FOUR_PIN) == LOW;
    // currentData.seatBeltWarning = digitalRead(SEAT_BELT_WARNING_PIN) == LOW;
    // currentData.doorAjarWarning = digitalRead(DOOR_AJAR_WARNING_PIN) == LOW;
    // currentData.brakeLightWarning = digitalRead(BRAKE_LIGHT_PIN) == LOW;
//    Serial.println("left blinker " + (String)(currentData.leftBlinker ? "on" : "off"));

#ifdef CUMMINS_BUS_INPUT
    currentData.rpm = CumminsBus::getCurrentRpms();
    currentData.coolantTemp = CumminsBus::getCurrentWaterTemp();
    currentData.oilPressureInPsi = CumminsBus::getCurrentOilPressure();
    currentData.fuelTempF = CumminsBus::getCurrentFuelTemp();
//    Serial.println("RPM: " + (String)currentData.rpm);
#endif
#ifdef TACHOMETER_INPUT_60_MINUS_2
    currentData.rpm = TachometerInput60Minus2::getCurrentRpm();
#endif

#ifdef SPEEDOMETER_INPUT
    currentData.speedInMph = sweep; // SpeedometerInput::getCurrentSpeedInMph(); // map(sweep, 0, maxSweep, 0, 200); // random(0,255);
    thisMileage += ((float)currentData.speedInMph / 3600000.0f * (float)thisDuration);
//    Serial.println("thisMileage? " + (String)thisMileage);

    if (thisMileage >= 0.1 || currentData.speedInMph <= 0) {
        currentData.odometer += thisMileage;
        currentData.tripA += thisMileage;
        currentData.tripB += thisMileage;
//        Serial.println("odometer: " + (String)currentData.odometer + " tripA: " + (String)currentData.tripA + " tripB: " + (String)currentData.tripB);
        thisMileage = 0;
    }
    // We only want to save if data has changed and we have come to a stop
    if (currentData.speedInMph <= 0) {
        EEPROM.put(0, currentData.odometer);
        EEPROM.put(8, currentData.tripA);
        EEPROM.put(16, currentData.tripB);
        EEPROM.put(24, ++currentData.odometerSaveCount);
//        Serial.println("odometer: " + (String)currentData.odometer + " tripA: " + (String)currentData.tripA + " tripB: " + (String)currentData.tripB + " saveCount: " + (String)currentData.odometerSaveCount);
    }
#endif

#ifdef TRANSMISSION_PRESSURE_INPUT
    currentData.transmissionPressure = TransmissionPressureSensor::getTransmissionPressureInPsi();
#endif

#ifdef THERMOCOUPLE
    currentData.egt = thermocouple.readFahrenheit();
#endif

#ifdef TRANSMISSION_TEMPERATURE_INPUT
    currentData.transmissionTempC = TransTempSensor.getTempCelsius();
//    Serial.println("Trans temp in C? " + (String)currentData.transmissionTempC);
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
    if (thisDuration > 50) {
        // send unpolled data
        CanBus::sendOilPressure(currentData.oilPressureInPsi);
//        Serial.println("Sending Oil Pressure: " + (String)currentData.oilPressureInPsi);
    }
#endif

#ifdef NEXTION
    Nextion::updateDisplayData(&currentData);
#endif

    lastMillis = thisMillis;
    if (count > 50000 && count % 50000 == 0) Serial.println("Average Microseconds Per Loop: " + (String)(micros() / count));

    while (Serial.available()) {
        int newData = Serial.read();
        if (newData == ';') {
            Serial.println("Execute!" + serialBuffer);
            if (serialBuffer == "resetTripA") {
                Serial.println("reset trip A!");
                double zero = 0;
                EEPROM.put(8, zero);
                currentData.tripA = zero;
            } else if (serialBuffer.indexOf("setOdometer") > 0) {
                double newOdometerReading = atof(
                    serialBuffer.substring(
                        serialBuffer.indexOf("=") + 1,
                        serialBuffer.indexOf(";")
                        ).c_str()
                        );
                EEPROM.put(0, newOdometerReading);
                currentData.odometer = newOdometerReading;
                Serial.print("Set Odometer = "); Serial.println(currentData.odometer);
            }
            serialBuffer = "";
        } else {
            serialBuffer += (char)newData;
        }
    }
}