//
// Created by jlaustill on 7/22/21.
//

// We need a way to opt in and out of functionality
// This file will do that in a central place to hopefully make using this app easier

/*
 * Inputs configuration
 *
 * You only need each input from a single source,
 * this section will allow you to configure where each input comes from, or if you want it at all
 *
 * */
#include <Arduino.h>
#include <TempSensor.h>

//#define ODB2
#ifdef ODB2
    #define ODB2_INT_PIN 2
    #define ODB2_CS_PIN 5
#endif
//#define CAN_BUS
#ifdef CAN_BUS
    #define CAN_BUS_INT_PIN 2
    #define CAN_BUS_CS_PIN 5
#endif
// The Cummins bus provides rpms, so don't enable both at the same time
// #define CUMMINS_BUS_INPUT
#ifdef CUMMINS_BUS_INPUT
    #define CUMMINS_BUS_INPUT_INT_PIN 3
    #define CUMMINS_BUS_INPUT_CS_PIN 6
#endif
// This is the setup on a 24 valve cummins using a camshaft sensor, so it assumes the gear is running at 1/2 the speed of the crankshaft
//#define TACHOMETER_INPUT_60_MINUS_2
//#define TACHOMETER_INPUT_2

#define SPEEDOMETER_INPUT
#ifdef SPEEDOMETER_INPUT
    #define SPEEDOMETER_INPUT_CLICKS_PER_MILE 8000
#endif

//#define TRANSMISSION_PRESSURE_INPUT
#ifdef TRANSMISSION_PRESSURE_INPUT
    #define TRANSMISSION_PRESSURE_INPUT_PIN A1
    #define TRANSMISSION_PRESSURE_INPUT_RAW_ZERO 105.0
    #define TRANSMISSION_PRESSURE_INPUT_RAW_MAX 921.6
    #define TRANSMISSION_PRESSURE_INPUT_PSI_MAX 500.0
#endif

#define TRANSMISSION_TEMPERATURE_INPUT
#ifdef TRANSMISSION_TEMPERATURE_INPUT
    #define TRANSMISSION_TEMPERATURE_INPUT_DEVICE_ID B1001000
    #define TRANSMISSION_TEMPERATURE_INPUT_PIN_NUMBER 0
    #define TRANSMISSION_TEMPERATURE_INPUT_DIVIDER 10000 //defines the analog pin of the input voltage from the voltage divider
    #define TRANSMISSION_TEMPERATURE_INPUT_A 1.511129458e-03
    #define TRANSMISSION_TEMPERATURE_INPUT_B 2.497079116e-04
    #define TRANSMISSION_TEMPERATURE_INPUT_C 0.03967365606e-07
#endif

#define LEFT_BLINKER_PIN 24
#define RIGHT_BLINKER_PIN 25
#define HIGH_BEAMS_PIN 26
#define WAIT_TO_START_PIN 27
#define FOUR_BY_FOUR_PIN 28
#define SEAT_BELT_WARNING_PIN 29
#define DOOR_AJAR_WARNING_PIN 30
#define BRAKE_LIGHT_PIN 31

/*
 * Outputs configuration
 *
 * */
// enable if you want a tachometer output on pin 44
//#define TACHOMETER_OUTPUT
#ifdef TACHOMETER_OUTPUT
    #define TACHOMETER_OUTPUT_INITIAL_RPMS 1
    #define TACHOMETER_OUTPUT_CYLINDER_COUNT 6
#endif

// enable if you want a speedometer out on pin 8
#define SPEEDOMETER_OUTPUT
#ifdef SPEEDOMETER_OUTPUT
    #define SPEEDOMETER_OUTPUT_CLICKS_PER_MILE 8000
#endif

#define NEXTION
#ifdef NEXTION
    #define nexSer Serial2 // Controlling the Nextion HMI using Serial1 (pin18 of the Arduino Mega) to prevent interfering with code upload
//    #define DEBUG          // Comment this out if you don't need to see what happens in the Serial Monitor
#endif

// enable if you want to monitor EGT's
//#define THERMOCOUPLE
#ifdef THERMOCOUPLE
    #define MAXDO   12
    #define MAXCS   13
    #define MAXCLK  11
#endif