#include <Arduino.h>
#include "Configuration.h"

#ifdef SPEEDOMETER_INPUT
    #include "Data/SpeedometerInput.h"
#endif

#ifdef TACHOMETER_OUTPUT
    #include "Display/Gauges/Tachometer.h"
#endif
#ifdef SPEEDOMETER_OUTPUT
    #include "Display/Gauges/Speedometer.h"
#endif


#include "Display/OBD2/OBD2.h"
#include "Data/Sensors/TransmissionTemperatureSensor.h"
#include <AppData.h>

AppData currentData;

#ifdef TACHOMETER_OUTPUT
    Tachometer tachometer = Tachometer();
#endif
#ifdef SPEEDOMETER_OUTPUT
    Speedometer speedometer = Speedometer();
#endif

void setup() {
#ifdef SPEEDOMETER_INPUT
    SpeedometerInput::initialize();
#endif

    currentData.rpm = 0;
    currentData.coolantTemp = 0;
    currentData.speedInMph = 0;
    Serial.begin(115200);
#ifdef TACHOMETER_OUTPUT
    tachometer.initialize();
#endif
#ifdef SPEEDOMETER_OUTPUT
    speedometer.initialize();
#endif
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

#ifdef SPEEDOMETER_INPUT
    currentData.speedInMph = SpeedometerInput::getCurrentSpeedInMph(); // map(sweep, 0, maxSweep, 0, 200); // random(0,255);
    Serial.println("speedInMph? " + (String)currentData.speedInMph);
#endif

    currentData.transmissionTempC = 40; //TransmissionTemperatureSensor::readNextValue();

#ifdef TACHOMETER_OUTPUT
    tachometer.SetRpms(currentData.rpm);
#endif
#ifdef SPEEDOMETER_OUTPUT
    speedometer.SetMph(currentData.speedInMph);
#endif

    OBD2::sendData(currentData);
}