#include <Arduino.h>
#include "Configuration.h"

#ifdef TACHOMETER_OUTPUT
#include "Display/Gauges/Tachometer.h"
#endif
#include "Display/Gauges/Speedometer.h"
#include "Display/OBD2/OBD2.h"
#include "Data/Sensors/TransmissionTemperatureSensor.h"
#include "Data/SpeedometerInput.h"
#include <AppData.h>

AppData currentData;

#ifdef TACHOMETER_OUTPUT
Tachometer tachometer = Tachometer();
#endif
Speedometer speedometer = Speedometer();
SpeedometerInput speedometerInput = SpeedometerInput();

void setup() {
    SpeedometerInput::initialize();

    currentData.rpm = 0;
    currentData.coolantTemp = 0;
    currentData.speedInMph = 0;
    Serial.begin(115200);
#ifdef TACHOMETER_OUTPUT
    tachometer.initialize();
#endif
    speedometer.initialize();
    OBD2::initialize();
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
#ifdef TACHOMETER_OUTPUT
    currentData.rpm = (int)map(sweep, 0, maxSweep, 0, 4000);
#endif
    currentData.speedInMph = SpeedometerInput::getCurrentSpeedInMph(); // map(sweep, 0, maxSweep, 0, 200); // random(0,255);
    currentData.transmissionTempC = 40; //TransmissionTemperatureSensor::readNextValue();

//    Serial.println("sweep: " + (String)sweep);
//    Serial.println("rpm: " + (String)currentData.rpm);
#ifdef TACHOMETER_OUTPUT
    tachometer.SetRpms(currentData.rpm);
#endif
//    speedometer.SetMph((int)map(sweep, 0, maxSweep, 0, 115)); // random(0,255));
speedometer.SetMph(25);
//    Serial.println("Mph: " + (String)currentData.speedInMph);

    OBD2::sendData(currentData);
}