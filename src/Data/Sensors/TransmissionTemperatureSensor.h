//
// Initially copied from https://github.com/tyler92dodge/Arduino-Coolant-Sensor-Thermistor/blob/master/Coolant_Temp_Sensor_v1.ino
//

#ifndef OPCM_TRANSMISSIONTEMPERATURESENSOR_H
#define OPCM_TRANSMISSIONTEMPERATURESENSOR_H

/* This example code shows how to display temperature
 *  from a standard 1 or 2 wire automotive coolant temperature
 *  sensor. These types of sensors are a simple thermistor in
 *  an industrial casing. Thermistor's change resistance based
 *  on temperature. A voltage divider circuit must be used to
 *  read the voltage to a microcontroller. Contact tylerovens@me.com
 *  if you have any questions.
 */

#include <AppData.h>

#define NUMSAMPLES 5                //defines the number of samples to be taken for a smooth average

class TransmissionTemperatureSensor {
public:
    static byte getTransmissionTemperatureInCelcius();

};


#endif //OPCM_TRANSMISSIONTEMPERATURESENSOR_H
