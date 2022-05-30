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