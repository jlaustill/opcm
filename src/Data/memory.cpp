#include <Arduino.h>
#include "memory.h"
#include <EEPROM.h>

struct addresses opcmMemoryAddresses;

double memory::getOdometer() {
    double result = 0;
    EEPROM.get(opcmMemoryAddresses.odometer, result);

    if (isnan(result)) {
        result = 0;
    }

    return result;
}

void memory::setOdometer(double newReading) {
    EEPROM.put(opcmMemoryAddresses.odometer, newReading);
}

double memory::getTripA() {
    double result = 0;
    EEPROM.get(opcmMemoryAddresses.tripA, result);

    if (isnan(result)) {
        result = 0;
    }

    return result;
}

void memory::setTripA(double newReading) {
    EEPROM.put(opcmMemoryAddresses.tripA, newReading);
}

double memory::getTripB() {
    double result = 0;
    EEPROM.get(opcmMemoryAddresses.tripB, result);

    if (isnan(result)) {
        result = 0;
    }

    return result;
}

void memory::setTripB(double newReading) {
    EEPROM.put(opcmMemoryAddresses.tripB, newReading);
}

double memory::getSaveCount() {
    double result = 0;
    EEPROM.get(opcmMemoryAddresses.count, result);

    if (isnan(result)) {
        result = 0;
    }

    return result;
}

void memory::setSaveCount(unsigned long newReading) {
    EEPROM.put(opcmMemoryAddresses.count, newReading);
}