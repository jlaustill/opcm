#include <Arduino.h>
#include <SPI.h>
#include <limits.h>
using namespace std;

#include "../lib/Display/Gauges/Tachometer.h"
#include "../lib/Display/Gauges/Speedometer.h"
#include "../lib/Display/OBD2/OBD2.h"
#include <AppData.h>

AppData currentData;

Tachometer tachometer = Tachometer(6, 0);
Speedometer speedometer = Speedometer();
OBD2 odb2 = OBD2();

void setup() {
    currentData.rpm = 0;
    currentData.coolantTemp = 0;
    currentData.speedInMph = 0;
    Serial.begin(115200);
    tachometer.initialize();
    speedometer.initialize();
    odb2.initialize();
}

long sweep = 0;
int up = 1;
long maxSweep = 100000;
void newSweepValue() {
    if (up == 1 && sweep < maxSweep) {
        sweep++;
    } else if (up == 1 && sweep >= maxSweep) {
        up = 0;
    } else if (sweep > 0) {
        sweep--;
    } else {
        up = 1;
    }
}

__attribute__((unused)) void loop()
{
    newSweepValue();
    currentData.coolantTemp = map(sweep, 0, maxSweep, 1, 200); // random(1,200);
    currentData.rpm = map(sweep, 0, maxSweep, 0, 4000);
    currentData.speedInMph = map(sweep, 0, maxSweep, 0, 200); // random(0,255);

    Serial.println("sweep: " + (String)sweep);
    Serial.println("rpm: " + (String)currentData.rpm);
    tachometer.SetRpms(currentData.rpm);
    speedometer.SetMph(currentData.speedInMph);

    odb2.sendData(currentData);
}