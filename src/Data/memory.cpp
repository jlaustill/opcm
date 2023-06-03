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

void memory::setSaveCount(double newReading) {
    EEPROM.put(opcmMemoryAddresses.count, newReading);
}

double memory::getOilChange() {
    double result = 0;
    EEPROM.get(opcmMemoryAddresses.oilChange, result);

    if (isnan(result)) {
        result = 0;
    }

    return result;
}

void memory::setOilChange(double newReading) {
    EEPROM.put(opcmMemoryAddresses.oilChange, newReading);
}

double memory::getTransmissionFluidChange() {
    double result = 0;
    EEPROM.get(opcmMemoryAddresses.transmissionFluidChange, result);

    if (isnan(result)) {
        result = 0;
    }

    return result;
}

void memory::setTransmissionFluidChange(double newReading) {
    EEPROM.put(opcmMemoryAddresses.transmissionFluidChange, newReading);
}

double memory::getTransferCaseFluidChange() {
    double result = 0;
    EEPROM.get(opcmMemoryAddresses.transferCaseFluidChange, result);

    if (isnan(result)) {
        result = 0;
    }

    return result;
}

void memory::setTransferCaseFluidChange(double newReading) {
    EEPROM.put(opcmMemoryAddresses.transferCaseFluidChange, newReading);
}

double memory::getFrontDifferentialFluidChange() {
    double result = 0;
    EEPROM.get(opcmMemoryAddresses.frontDifferentialFluidChange, result);

    if (isnan(result)) {
        result = 0;
    }

    return result;
}

void memory::setFrontDifferentialFluidChange(double newReading) {
    EEPROM.put(opcmMemoryAddresses.frontDifferentialFluidChange, newReading);
}

double memory::getRearDifferentialFluidChange() {
    double result = 0;
    EEPROM.get(opcmMemoryAddresses.rearDifferentialFluidChange, result);

    if (isnan(result)) {
        result = 0;
    }

    return result;
}

void memory::setRearDifferentialFluidChange(double newReading) {
    EEPROM.put(opcmMemoryAddresses.rearDifferentialFluidChange, newReading);
}

double memory::getFuelFilterChange() {
    double result = 0;
    EEPROM.get(opcmMemoryAddresses.fuelFilterChange, result);

    if (isnan(result)) {
        result = 0;
    }

    return result;
}

void memory::setFuelFilterChange(double newReading) {
    EEPROM.put(opcmMemoryAddresses.fuelFilterChange, newReading);
}

double memory::getTireRotation() {
    double result = 0;
    EEPROM.get(opcmMemoryAddresses.tireRotation, result);

    if (isnan(result)) {
        result = 0;
    }

    return result;
}

void memory::setTireRotation(double newReading) {
    EEPROM.put(opcmMemoryAddresses.tireRotation, newReading);
}