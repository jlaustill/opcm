#define thirdByte(w) ((uint8_t)((w) >> 16))
#define fourthByte(w) ((uint8_t)((w) >> 24))

#include "opcm.h"

#include <AppData.h>
#include <Arduino.h>

#include "Configuration.h"
#ifdef SDCARD
#include "Data/sdCard.h"
#endif
#include "setup.h"

#ifdef BLINK_OUTPUT
#include "Display/BlinkOutput.h"
#endif

#ifdef FRAM
#include "Data/fram.h"
#endif

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
unsigned long opcm::lastOdometerUpdate = 0;
float opcm::thisMileage = 0;

long opcm::sweep = 0;
long opcm::maxSweep = 50;
int opcm::up = 1;

#ifdef SDCARD
sdCard *SdCard;
#endif
long loopCountLastMillis = 0;

void opcm::setup() {
  Serial.begin(115200);
  count = 0;
  lastMillis = millis();
  thisMillis = millis();
  lastOdometerUpdate = 0;
  thisDuration = 0;
  thisMileage = 0;

  Serial.println("Starting up...");
#ifdef SDCARD
  SdCard = new sdCard();
#endif

#ifdef BLINK_OUTPUT
  BlinkOutput::initialize();
#endif

#ifdef FRAM
  OPCM_Fram::initialize();
#endif

#ifdef TRANSMISSION_PRESSURE_INPUT
  TransPresSensor = PressureSensor(TRANSMISSION_PRESSURE_INPUT_PSI_MAX,
                                   TRANSMISSION_PRESSURE_INPUT_RAW_ZERO,
                                   TRANSMISSION_PRESSURE_INPUT_RAW_MAX,
                                   TRANSMISSION_PRESSURE_INPUT_DEVICE_ID,
                                   TRANSMISSION_PRESSURE_INPUT_PIN_NUMBER);
#endif

#ifdef TRANSMISSION_TEMPERATURE_INPUT
  TransTempSensor =
      TempSensor(TRANSMISSION_TEMPERATURE_INPUT_DIVIDER,  // KnownResistorValue
                 TRANSMISSION_TEMPERATURE_INPUT_A,        // A
                 TRANSMISSION_TEMPERATURE_INPUT_B,        // B
                 TRANSMISSION_TEMPERATURE_INPUT_C,        // C
                 TRANSMISSION_TEMPERATURE_INPUT_DEVICE_ID,
                 TRANSMISSION_TEMPERATURE_INPUT_PIN_NUMBER,
                 TRANSMISSION_TEMPERATURE_INPUT_WIRING_RESISTANCE);
#endif

#ifdef OIL_TEMPERATURE_INPUT
  OilTempSensor = TempSensor(
      OIL_TEMPERATURE_INPUT_DIVIDER,  // KnownResistorValue
      OIL_TEMPERATURE_INPUT_A,        // A
      OIL_TEMPERATURE_INPUT_B,        // B
      OIL_TEMPERATURE_INPUT_C,        // C
      OIL_TEMPERATURE_INPUT_DEVICE_ID, OIL_TEMPERATURE_INPUT_PIN_NUMBER,
      OIL_TEMPERATURE_INPUT_WIRING_RESISTANCE);
#endif

#ifdef WATER_TEMPERATURE_INPUT
  WaterTempSensor = TempSensor(
      WATER_TEMPERATURE_INPUT_DIVIDER,  // KnownResistorValue
      WATER_TEMPERATURE_INPUT_A,        // A
      WATER_TEMPERATURE_INPUT_B,        // B
      WATER_TEMPERATURE_INPUT_C,        // C
      WATER_TEMPERATURE_INPUT_DEVICE_ID, WATER_TEMPERATURE_INPUT_PIN_NUMBER,
      WATER_TEMPERATURE_INPUT_WIRING_RESISTANCE);
#endif

#ifdef CUMMINS_BUS_INPUT
  Serial.println("CUMMINS_BUS_INPUT defined");
  CumminsBus::initialize(&currentData);
#endif
#ifdef SPEEDOMETER_INPUT
  SpeedometerInput::initialize();
#endif
#ifdef TACHOMETER_INPUT_60_MINUS_2
  TachometerInput60Minus2::initialize();
#endif

#ifdef FRAM
  currentData = OPCM_Fram::loadData();
//   Serial.print("odometer: ");
//   Serial.print(currentData.odometer);
//   Serial.print(" TripA: ");
//   Serial.print(currentData.tripA);
//   Serial.print(" TripB: ");
//   Serial.println(currentData.tripB);
#endif
#ifdef SDCARD
  currentData = SdCard->loadData();
#endif

  currentData.rpm = 0;
  currentData.coolantTemp = 0;
  currentData.speedInMph = 0;
  currentData.transmissionPressure = 0;
  currentData.oilPressureInPsi = 0;
  currentData.fuelTempF = 0;
  currentData.boost = 0;
  currentData.manifoldTempC = 0;

  currentData.timing = 0;
  currentData.fuelPercentage = 0;
  currentData.amt = 0;
  currentData.throttlePercentage = 0;
  currentData.load = 0;
  currentData.fuelPressure = 0.0;
  
  // // Temp remove after first flash
  // currentData.milesOnEngine = 1100;
  // currentData.milesOnTransferCase = 2000;
  // currentData.milesOnTransmission = 80000;

#ifdef TRANSMISSION_TEMPERATURE_INPUT
  currentData.transmissionTempC = 0;
#endif
#ifdef OIL_TEMPERATURE_INPUT
  currentData.oilTempC = 0;
#endif
#ifdef WATER_TEMPERATURE_INPUT
  currentData.coolantTemp2 = 0;
#endif
#ifdef TACHOMETER_OUTPUT
  tachometer.initialize();
#endif
#ifdef SPEEDOMETER_OUTPUT
  speedometer.initialize();
#endif

#ifdef ODB2
  OBD2::initialize(&currentData);
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

#ifdef BLINK_OUTPUT
  BlinkOutput::blink();
#endif

  // Debugging
  // Serial.print(count);
  // Serial.print(" ");
  // Serial.println(count);
  // delay(1000);
#ifdef DEBUG
  currentData.speedInMph = sweep;
  currentData.rpm = sweep * 100;
  currentData.coolantTemp = sweep * 2;
  currentData.load = sweep * 2;
  currentData.throttlePercentage = sweep * 2;
  currentData.oilTempC = sweep * 2 - 23;
  currentData.timing = sweep / 3;
  currentData.boost = sweep + sweep / 10;
  currentData.transmissionTempC = sweep;
  currentData.transmissionPressure = sweep;
  currentData.fuelTempF = sweep;
  currentData.fuelPercentage = sweep;
  currentData.amt = sweep;
  currentData.odometer = sweep * 100;
  currentData.tripA = sweep * 100;
  currentData.tripB = sweep * 100;
  currentData.oilChange = sweep * 100;
  currentData.transmissionFluidChange = sweep * 100;
  currentData.transferCaseFluidChange = sweep * 100;
  currentData.frontDifferentialFluidChange = sweep * 100;
  currentData.rearDifferentialFluidChange = sweep * 100;
  currentData.fuelFilterChange = sweep * 100;
  currentData.tireRotation = sweep * 100;
  currentData.odometerSaveCount = sweep * 100;
  currentData.egt = sweep * 100;
  currentData.currentGear = sweep;
  currentData.selectedGear = sweep;
  currentData.requestedRange = sweep;
  currentData.transmissionTempC = sweep;
  currentData.transmissionPressure = sweep;
  currentData.oilPressureInPsi = sweep;
  currentData.fuelTempF = sweep;
  currentData.boost = sweep;
  currentData.timing = sweep;
  currentData.coolantTemp2 = sweep;

  uint8_t req = sweep % 7;
  switch (req) {
    case 1:
      currentData.requestedRange = 'P';
      break;
    case 2:
      currentData.requestedRange = 'R';
      break;
    case 3:
      currentData.requestedRange = 'N';
      break;
    case 4:
      currentData.requestedRange = 'D';
      break;
    case 5:
      currentData.requestedRange = '5';
      break;
    case 6:
      currentData.requestedRange = '4';
      break;
    case 7:
      currentData.requestedRange = '3';
      break;

    default:
      break;
  }
#endif

#ifdef CUMMINS_BUS_INPUT
  CumminsBus::loop();
  currentData.rpm = CumminsBus::getCurrentRpms();
  currentData.coolantTemp = CumminsBus::getCurrentWaterTemp();
  currentData.oilPressureInPsi = CumminsBus::getCurrentOilPressure();
  currentData.fuelTempF = CumminsBus::getCurrentFuelTemp();
  currentData.boost = CumminsBus::getCurrentBoostInPsi();
  currentData.manifoldTempC = CumminsBus::getCurrentBoostTemp();
  // Serial.print("boost ");
  // Serial.print(currentData.boost);
  // Serial.print(" boost temp ");
  // Serial.println(currentData.manifoldTempC);

  currentData.timing = CumminsBus::getCurrentTiming();
  currentData.fuelPercentage = CumminsBus::getCurrentFuelPercentage();
  currentData.amt = CumminsBus::getCurrentAMT();
  currentData.throttlePercentage = CumminsBus::getCurrentThrottlePercentage();
  currentData.load = CumminsBus::getCurrentLoad();
  currentData.transmissionTempC = CumminsBus::getTransmissionTempC();
  currentData.speedInMph = CumminsBus::getVehicleSpeed();
  currentData.requestedRange = CumminsBus::getRequestedRange();
  currentData.currentGear = CumminsBus::getCurrentGear();
  currentData.selectedGear = CumminsBus::getSelectedGear();
  currentData.egt = CumminsBus::getCurrentEgtTemp();
  currentData.fuelPressure = CumminsBus::getCurrentFuelPressurePsi();

#endif
#ifdef TACHOMETER_INPUT_60_MINUS_2
  currentData.rpm = TachometerInput60Minus2::getCurrentRpm();
#endif

#ifdef SPEEDOMETER_INPUT
  currentData.speedInMph = SpeedometerInput::getCurrentSpeedInMph();

#endif

  thisMileage += (static_cast<float>(currentData.speedInMph) / 3600000.0f *
                  static_cast<float>(thisDuration));
  //    Serial.println("thisMileage? " + (String)thisMileage + " "  +
  //    (thisMillis) + " "  + (lastOdometerUpdate) + " "  + (thisMillis -
  //    lastOdometerUpdate));

  if (thisMileage >= 0.1 || (thisMillis - lastOdometerUpdate) > 10) {
    lastOdometerUpdate = thisMillis;
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
    currentData.milesOnEngine += thisMileage;
    currentData.milesOnTransmission += thisMileage;
    currentData.milesOnTransferCase += thisMileage;
    currentData.odometerSaveCount++;

    //        Serial.println("odometer: " + (String)currentData.odometer + "
    //        tripA: " + (String)currentData.tripA + " tripB: " +
    //        (String)currentData.tripB);
    thisMileage = 0;

    // TEMP to adjust starting state
    // currentData.tripB = 312.1;
    // currentData.oilChange = 312.1;

#ifdef FRAM
    // Thanks to the speed of Fram, we can save on every loop!
    OPCM_Fram::saveData(&currentData);
#endif
#ifdef SDCARD
    // We only want to save if data has changed and we have come to a stop
    if (currentData.speedInMph <= 0.5) SdCard->saveData(&currentData);
#endif
  }

  // Serial.println("Transfer Case Fluid? " +
  // (String)currentData.transferCaseFluidChange);

#ifdef TRANSMISSION_PRESSURE_INPUT
  currentData.transmissionPressure = TransPresSensor.getPressureInPsi();
  // Serial.println("trans pressure? " +
  // (String)currentData.transmissionPressure);
#endif

#ifdef THERMOCOUPLE
  currentData.egt = thermocouple.readFahrenheit();
  // Serial.println("EGT: " + (String)currentData.egt);
#endif

#ifdef TRANSMISSION_TEMPERATURE_INPUT
  currentData.transmissionTempC = TransTempSensor.getTempCelsius();
//    Serial.println("Trans temp in C? " +
//    (String)currentData.transmissionTempC);
#endif

#ifdef OIL_TEMPERATURE_INPUT
  currentData.oilTempC = OilTempSensor.getTempCelsius();
//    Serial.println("Oil temp in C? " + (String)currentData.oilTempC);
#endif

#ifdef WATER_TEMPERATURE_INPUT
  currentData.coolantTemp2 = WaterTempSensor.getTempCelsius();
//    Serial.println("Water2 temp in C? " + (String)currentData.coolantTemp2);
#endif

#ifdef TACHOMETER_OUTPUT
  tachometer.SetRpms(currentData.rpm);
#endif
#ifdef OBD2
  OBD2::sendCumminsObd2Speed(currentData.speedInMph);
#endif
#ifdef SPEEDOMETER_OUTPUT
  speedometer.SetMph(currentData.speedInMph);
#endif

#ifdef NEXTION
  Nextion::updateDisplayData(&currentData);
  // Serial.println("why the rebooting?" + (String)thisMillis);
#endif

  lastMillis = thisMillis;

  if (thisMillis - loopCountLastMillis > 1000) {
    loopCountLastMillis = thisMillis;
    Serial.println("Loop Count/Sec: " + (String)count + " Miles On Engine: " + (String)currentData.milesOnEngine + " RPM: " + (String)currentData.rpm);
    count = 0;
  }

  while (Serial.available()) {
    int newData = Serial.read();
    if (newData == ';') {
      Serial.println("Execute!" + serialBuffer);
      if (serialBuffer == "resetTripA") {
        Serial.println("reset trip A!");
        currentData.tripA = 0;
      } else if (serialBuffer.indexOf("setOdometer") > 0) {
        double newOdometerReading =
            atof(serialBuffer
                     .substring(serialBuffer.indexOf("=") + 1,
                                serialBuffer.indexOf(";"))
                     .c_str());
        currentData.odometer = newOdometerReading;
        Serial.print("Set Odometer = ");
        Serial.println(currentData.odometer);
      }
      serialBuffer = "";
    } else {
      serialBuffer += static_cast<char>(newData);
    }
  }
}
