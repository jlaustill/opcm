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

#define SPEEDOMETER_INPUT
#ifdef SPEEDOMETER_INPUT
    #define SPEEDOMETER_INPUT_CLICKS_PER_MILE 8000
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