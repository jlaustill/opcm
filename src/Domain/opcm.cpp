#include <Arduino.h>
#include "Configuration.h"
#include "setup.h"

#include "opcm.h"
#include "Data/memory.h"

#include <AppData.h>

void opcm::newSweepValue() {
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

unsigned long opcm::count = 0;
unsigned long opcm::lastMillis = 0;
unsigned long opcm::thisMillis = 0;
unsigned long opcm::thisDuration = 0;
float opcm::thisMileage = 0;

long opcm::sweep = 0;
long opcm::maxSweep = 50;
int opcm::up = 1;

void opcm::setup() {
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

    Serial.println("Starting up...");

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
    currentData.boost = 0;
    
    currentData.timing = 0;
    currentData.fuelPercentage = 0;
    currentData.amt = 0;
    currentData.throttlePercentage = 0;
    currentData.load = 0;

    currentData.odometer = memory::getOdometer();
    currentData.tripA = memory::getTripA();
    currentData.tripB = memory::getTripB();
    currentData.odometerSaveCount = memory::getSaveCount();
    currentData.oilChange = memory::getOilChange();
    currentData.transmissionFluidChange = memory::getTransmissionFluidChange();
    currentData.transferCaseFluidChange = memory::getTransferCaseFluidChange();
    currentData.frontDifferentialFluidChange = memory::getFrontDifferentialFluidChange();
    currentData.rearDifferentialFluidChange = memory::getRearDifferentialFluidChange();
    currentData.fuelFilterChange = memory::getFuelFilterChange();
    currentData.tireRotation = memory::getTireRotation();

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

void opcm::loop() {
    thisMillis = millis();
    thisDuration = thisMillis - lastMillis;
    count++;
    newSweepValue();
    // Serial.print(sweep);
    // Serial.print(" ");
    // Serial.println(count);
    // delay(1000);

    // currentData.speedInMph = sweep;
    // currentData.rpm = sweep * 100;
// Serial.print("time to debug "); Serial.println(digitalRead(LEFT_BLINKER_PIN));
    currentData.leftBlinker = digitalRead(LEFT_BLINKER_PIN) == LOW;
    currentData.rightBlinker = digitalRead(RIGHT_BLINKER_PIN) == LOW;
    currentData.highBeams = digitalRead(HIGH_BEAMS_PIN) == LOW;
    currentData.waitToStart = digitalRead(WAIT_TO_START_PIN) == LOW;
    // currentData.fourByFour = digitalRead(FOUR_BY_FOUR_PIN) == LOW;
    // currentData.seatBeltWarning = digitalRead(SEAT_BELT_WARNING_PIN) == LOW;
    // currentData.doorAjarWarning = digitalRead(DOOR_AJAR_WARNING_PIN) == LOW;
    // currentData.brakeLightWarning = digitalRead(BRAKE_LIGHT_PIN) == LOW;

#ifdef CUMMINS_BUS_INPUT
    currentData.rpm = CumminsBus::getCurrentRpms();
    currentData.coolantTemp = CumminsBus::getCurrentWaterTemp();
    currentData.oilPressureInPsi = CumminsBus::getCurrentOilPressure();
    currentData.fuelTempF = CumminsBus::getCurrentFuelTemp();
    currentData.boost = CumminsBus::getCurrentBoostInPsi();

    currentData.timing = CumminsBus::getCurrentTiming();
    currentData.fuelPercentage = CumminsBus::getCurrentFuelPercentage();
    currentData.amt = CumminsBus::getCurrentAMT();
    currentData.throttlePercentage = CumminsBus::getCurrentThrottlePercentage();
    currentData.load = CumminsBus::getCurrentLoad();
#endif
#ifdef TACHOMETER_INPUT_60_MINUS_2
    currentData.rpm = TachometerInput60Minus2::getCurrentRpm();
#endif

#ifdef SPEEDOMETER_INPUT
    currentData.speedInMph = SpeedometerInput::getCurrentSpeedInMph(); // map(sweep, 0, maxSweep, 0, 200); // random(0,255);
    thisMileage += ((float)currentData.speedInMph / 3600000.0f * (float)thisDuration);
//    Serial.println("thisMileage? " + (String)thisMileage);

    if (thisMileage >= 0.1 || currentData.speedInMph <= 0) {
        currentData.odometer += thisMileage;
        currentData.tripA += thisMileage;
        currentData.tripB += thisMileage;
        currentData.oilChange += thisMileage;
        currentData.transmissionFluidChange += thisMileage;
        currentData.transferCaseFluidChange += thisMileage;
        currentData.frontDifferentialFluidChange += thisMileage;
        currentData.rearDifferentialFluidChange += thisMileage;
        currentData.fuelFilterChange += thisMileage;
        currentData.tireRotation += thisMileage;

//        Serial.println("odometer: " + (String)currentData.odometer + " tripA: " + (String)currentData.tripA + " tripB: " + (String)currentData.tripB);
        thisMileage = 0;
    }
    // We only want to save if data has changed and we have come to a stop
    if (currentData.speedInMph <= 0) {
        memory::setOdometer(currentData.odometer);
        memory::setTripA(currentData.tripA);
        memory::setTripB(currentData.tripB);
        memory::setSaveCount(++currentData.odometerSaveCount);
        memory::setOilChange(currentData.oilChange);
        memory::setTransmissionFluidChange(currentData.transmissionFluidChange);
        memory::setTransferCaseFluidChange(currentData.transferCaseFluidChange);
        memory::setFrontDifferentialFluidChange(currentData.frontDifferentialFluidChange);
        memory::setRearDifferentialFluidChange(currentData.rearDifferentialFluidChange);
        memory::setFuelFilterChange(currentData.fuelFilterChange);
        memory::setTireRotation(currentData.tireRotation);
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
                memory::setTripA(0);
                currentData.tripA = 0;
            } else if (serialBuffer.indexOf("setOdometer") > 0) {
                double newOdometerReading = atof(
                    serialBuffer.substring(
                        serialBuffer.indexOf("=") + 1,
                        serialBuffer.indexOf(";")
                        ).c_str()
                        );
                memory::setOdometer(newOdometerReading);
                currentData.odometer = newOdometerReading;
                Serial.print("Set Odometer = "); Serial.println(currentData.odometer);
            }
            serialBuffer = "";
        } else {
            serialBuffer += (char)newData;
        }
    }
}