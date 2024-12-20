#ifndef OPCM_SETUP_H
#define OPCM_SETUP_H

#include <AppData.h>
#include <PressureSensor.h>
#include <TempSensor.h>

#include "Configuration.h"
// #include <Wire.h>

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
PressureSensor TransPresSensor;
#endif

#ifdef TRANSMISSION_TEMPERATURE_INPUT
TempSensor TransTempSensor;
#endif

#ifdef OIL_TEMPERATURE_INPUT
TempSensor OilTempSensor;
#endif

#ifdef WATER_TEMPERATURE_INPUT
TempSensor WaterTempSensor;
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

#ifdef THERMOCOUPLE
#include <Adafruit_MAX31855.h>
#endif

AppData currentData;

String serialBuffer;

float roundToTwo(float var) {
  float value = (int)(var * 100 + .5);
  return (float)value / 100.0;
}

#ifdef TACHOMETER_OUTPUT
Tachometer tachometer = Tachometer();
#endif
#ifdef SPEEDOMETER_OUTPUT
Speedometer speedometer = Speedometer();
#endif

#ifdef THERMOCOUPLE
// initialize the Thermocouple
Adafruit_MAX31855 thermocouple(MAXCS);
#endif

#endif  // OPCM_SETUP_H