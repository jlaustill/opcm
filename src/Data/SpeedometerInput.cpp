//
// Created by jlaustill on 7/20/21.
//

#include "SpeedometerInput.h"
#include <Arduino.h>
#include "../Configuration.h"
#include "Display/Gauges/Speedometer.h"
#include <math.h>

#ifdef SPEEDOMETER_INPUT
#define INPUT_PIN 18
#define SPEEDOMETER_INPUT_SMOOTHING_SAMPLES 10

volatile unsigned long prev_time = 0;
volatile unsigned long thisTime = 0;
unsigned long oneMilePerHour = 0;
unsigned long thisRequestTime = 0;
int mphSamples[SPEEDOMETER_INPUT_SMOOTHING_SAMPLES] = {};
uint16_t mphSampleIndex = 0;
int mph = 0;

void falling() {
    prev_time = thisTime;
    thisTime = micros();
}

void SpeedometerInput::initialize() {
    pinMode(INPUT_PIN, FALLING);
    oneMilePerHour = Speedometer::MphToMicroseconds(1);
    attachInterrupt(digitalPinToInterrupt(INPUT_PIN), falling, FALLING);
}

double MicrosecondsToHertz(unsigned long _microseconds) {
    double hertz;
    hertz = (double)_microseconds * .000001;
    return (double)(1.0 / hertz);
}

int MicrosecondsToMph(unsigned long _microseconds) {
    double hertz = MicrosecondsToHertz(_microseconds);
    hertz = (hertz * 60 * 60) / SPEEDOMETER_INPUT_CLICKS_PER_MILE;
    return (int)(hertz > 1 ? hertz : 0);
}

int SpeedometerInput::getCurrentSpeedInMph() {
    thisRequestTime = micros();

//    Serial.println("thisRequestTime : thisTime : oneMilePerHour " + (String)thisRequestTime + " : " + (String)thisTime + " : " + (String)oneMilePerHour);
    if ((thisRequestTime - thisTime) > oneMilePerHour) {
//        Serial.println("WHYYYYYYYYY");
        mph = 0;
    } else {
        mph = MicrosecondsToMph((thisTime - prev_time));
    }



    mphSamples[mphSampleIndex++] = mph;
    if (mphSampleIndex >= SPEEDOMETER_INPUT_SMOOTHING_SAMPLES) {
        mphSampleIndex = 0; // circular structure reset
    }
    double average = 0;
    int min = INT16_MAX;
    int max = 0;
    for (int i = 0; i < SPEEDOMETER_INPUT_SMOOTHING_SAMPLES; i++) {
        int thisSample = mphSamples[i];
        if (thisSample < min) {
            min = thisSample;
        }
        if (thisSample > max) {
            max = thisSample;
        }
        average += (double)thisSample;
//        Serial.print(thisSample);
//        Serial.print(" ");
    }
//    Serial.print(average);
//    Serial.print(" ");
    average -= (double)min; // don't include the min
//    Serial.print(min);
//    Serial.print(" ");
    average -= (double)max; // don't include the max
//    Serial.print(max);
//    Serial.print(" ");
    average /= (SPEEDOMETER_INPUT_SMOOTHING_SAMPLES - 2);
//    Serial.println("average mph? " + (String)average);
    return (int)lround(average);
}

#endif
