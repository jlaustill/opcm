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

#define ODB2
#ifdef ODB2
    #define ODB2_INT_PIN 2
    #define ODB2_CS_PIN 5
#endif
#define CAN_BUS
#ifdef CAN_BUS
    #define CAN_BUS_INT_PIN 2
    #define CAN_BUS_CS_PIN 5
#endif
// The Cummins bus provides rpms, so don't enable both at the same time
#define CUMMINS_BUS_INPUT
#ifdef CUMMINS_BUS_INPUT
    #define CUMMINS_BUS_INPUT_INT_PIN 3
    #define CUMMINS_BUS_INPUT_CS_PIN 6
#endif
// This is the setup on a 24 valve cummins using a camshaft sensor, so it assumes the gear is running at 1/2 the speed of the crankshaft
//#define TACHOMETER_INPUT_60_MINUS_2

#define SPEEDOMETER_INPUT
#ifdef SPEEDOMETER_INPUT
    #define SPEEDOMETER_INPUT_CLICKS_PER_MILE 8000
#endif

#define TRANSMISSION_PRESSURE_INPUT
#ifdef TRANSMISSION_PRESSURE_INPUT
    #define TRANSMISSION_PRESSURE_INPUT_PIN A1
    #define TRANSMISSION_PRESSURE_INPUT_RAW_ZERO 105.0
    #define TRANSMISSION_PRESSURE_INPUT_RAW_MAX 921.6
    #define TRANSMISSION_PRESSURE_INPUT_PSI_MAX 500.0
#endif

#define TRANSMISSION_TEMPERATURE_INPUT
#ifdef TRANSMISSION_TEMPERATURE_INPUT
    #define TRANSMISSION_TEMPERATURE_INPUT_PIN A0 //defines the resistor value that is in series in the voltage divider
    #define TRANSMISSION_TEMPERATURE_INPUT_DIVIDER 2150 //defines the analog pin of the input voltage from the voltage divider
#endif


/*
 * Outputs configuration
 *
 * */
// enable if you want a tachometer output on pin 44
#define TACHOMETER_OUTPUT
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
    #define nexSer Serial3 // Controlling the Nextion HMI using Serial1 (pin18 of the Arduino Mega) to prevent interfering with code upload
//    #define DEBUG          // Comment this out if you don't need to see what happens in the Serial Monitor
#endif