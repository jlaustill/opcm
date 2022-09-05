#define _VERSION_MAJOR 0
#define _VERSION_MINOR 1
#define _VERSION_PATCH 1

#include <Arduino.h>

#include "Domain/opcm.h"

void setup() {
    opcm::setup();
}

__attribute__((unused)) void loop() {
    opcm::loop();
}