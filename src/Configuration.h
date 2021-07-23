//
// Created by jlaustill on 7/22/21.
//

// We need a way to opt in and out of functionality
// This file will do that in a central place to hopefully make using this app easier

// enable if you want a tach output on pin 44
#define TACHOMETER_OUTPUT
#ifdef TACHOMETER_OUTPUT
    #define TACHOMETER_OUTPUT_INITIAL_RPMS 1
    #define TACHOMETER_OUTPUT_CYLINDER_COUNT 6
#endif

#ifndef NPCM_CONFIGURATION_H
#define NPCM_CONFIGURATION_H

#endif //NPCM_CONFIGURATION_H
