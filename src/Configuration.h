//
// Created by jlaustill on 7/22/21.
//

// We need a way to opt in and out of functionality
// This file will do that in a central place to hopefully make using this app
// easier

/*
 * Inputs configuration
 *
 * You only need each input from a single source,
 * this section will allow you to configure where each input comes from, or if
 * you want it at all
 *
 * */

#ifndef OPCM_CONFIGURATION_H
#define OPCM_CONFIGURATION_H

#include <Arduino.h>
#include <TempSensor.h>

#define AEM_TEMP_SENSOR_A 1.485995686e-03
#define AEM_TEMP_SENSOR_B 2.279654266e-04
#define AEM_TEMP_SENSOR_C 1.197578033e-07

// #define ODB2
// #define NEXTION_DEBUG

#define BLINK_OUTPUT

#define FRAM

// The Cummins bus provides rpms, so don't enable both at the same time
// #define CUMMINS_BUS_INPUT

// This is the setup on a 24 valve cummins using a camshaft sensor, so it
// assumes the gear is running at 1/2 the speed of the crankshaft #define
// TACHOMETER_INPUT_60_MINUS_2 #define TACHOMETER_INPUT_2

// #define SPEEDOMETER_INPUT
#ifdef SPEEDOMETER_INPUT
#define SPEEDOMETER_INPUT_CLICKS_PER_MILE 8000
#endif

// #define TRANSMISSION_PRESSURE_INPUT
#ifdef TRANSMISSION_PRESSURE_INPUT
#define TRANSMISSION_PRESSURE_INPUT_DEVICE_ID B1001000  // addr to gnd
#define TRANSMISSION_PRESSURE_INPUT_PIN_NUMBER 0
#define TRANSMISSION_PRESSURE_INPUT_RAW_ZERO 2000.0f
#define TRANSMISSION_PRESSURE_INPUT_RAW_MAX 59000.0f
#define TRANSMISSION_PRESSURE_INPUT_PSI_MAX 500.0
#endif

// #define TRANSMISSION_TEMPERATURE_INPUT
#ifdef TRANSMISSION_TEMPERATURE_INPUT
#define TRANSMISSION_TEMPERATURE_INPUT_DEVICE_ID B1001000  // addr to gnd
#define TRANSMISSION_TEMPERATURE_INPUT_PIN_NUMBER 1
#define TRANSMISSION_TEMPERATURE_INPUT_DIVIDER \
  2 * 1000  // defines the analog pin of the input voltage from the voltage
            // divider
#define TRANSMISSION_TEMPERATURE_INPUT_WIRING_RESISTANCE 111.7
#define TRANSMISSION_TEMPERATURE_INPUT_A 1.511129458e-03
#define TRANSMISSION_TEMPERATURE_INPUT_B 2.497079116e-04
#define TRANSMISSION_TEMPERATURE_INPUT_C -0.03967365606e-07
#endif

// #define OIL_TEMPERATURE_INPUT
#ifdef OIL_TEMPERATURE_INPUT
#define OIL_TEMPERATURE_INPUT_DEVICE_ID B1001000  // addr to gnd
#define OIL_TEMPERATURE_INPUT_PIN_NUMBER 2
#define OIL_TEMPERATURE_INPUT_DIVIDER \
  2 * 1000  // defines the analog pin of the input voltage from the voltage
            // divider
#define OIL_TEMPERATURE_INPUT_WIRING_RESISTANCE 100.0
#define OIL_TEMPERATURE_INPUT_A AEM_TEMP_SENSOR_A
#define OIL_TEMPERATURE_INPUT_B AEM_TEMP_SENSOR_B
#define OIL_TEMPERATURE_INPUT_C AEM_TEMP_SENSOR_C
#endif

// #define WATER_TEMPERATURE_INPUT
#ifdef WATER_TEMPERATURE_INPUT
#define WATER_TEMPERATURE_INPUT_DEVICE_ID B1001000  // addr to gnd
#define WATER_TEMPERATURE_INPUT_PIN_NUMBER 3
#define WATER_TEMPERATURE_INPUT_DIVIDER \
  2 * 1000  // defines the analog pin of the input voltage from the voltage
            // divider
#define WATER_TEMPERATURE_INPUT_WIRING_RESISTANCE 63.0
#define WATER_TEMPERATURE_INPUT_A 1.511129458e-03
#define WATER_TEMPERATURE_INPUT_B 2.497079116e-04
#define WATER_TEMPERATURE_INPUT_C -0.03967365606e-07
#endif

/*
 * Outputs configuration
 *
 * */
// enable if you want a tachometer output on pin 44
// #define TACHOMETER_OUTPUT
#ifdef TACHOMETER_OUTPUT
#define TACHOMETER_OUTPUT_INITIAL_RPMS 1
#define TACHOMETER_OUTPUT_CYLINDER_COUNT 6
#endif

// enable if you want a speedometer out on pin 8
// #define SPEEDOMETER_OUTPUT
#ifdef SPEEDOMETER_OUTPUT
#define SPEEDOMETER_OUTPUT_CLICKS_PER_MILE 8000
#endif

#define NEXTION
#ifdef NEXTION
#define nexSer \
  Serial2  // Controlling the Nextion HMI using Serial1 (pin18 of the Arduino
           // Mega) to prevent interfering with code upload
//    #define DEBUG          // Comment this out if you don't need to see what
//    happens in the Serial Monitor
#endif

// enable if you want to monitor EGT's
// #define THERMOCOUPLE
#ifdef THERMOCOUPLE
#define MAXCS 40
#endif

#endif  // OPCM_CONFIGURATION_H
