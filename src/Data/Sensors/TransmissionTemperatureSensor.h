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

#define coolantsensorDivider 2670   //defines the resistor value that is in series in the voltage divider
#define coolantsensorPin A0         //defines the analog pin of the input voltage from the voltage divider
#define NUMSAMPLES 5                //defines the number of samples to be taken for a smooth average


const float steinconstA = 0.00132774106461327;        //steinhart equation constant A, determined from wikipedia equations
const float steinconstB = 0.000254470874104285;       //steinhart equation constant B, determined from wikipedia equations
const float steinconstC = 0.000000101216538378909;    //steinhart equation constant C, determined from wikipedia equations

class TransmissionTemperatureSensor {
public:
    static byte readNextValue();
};


#endif //OPCM_TRANSMISSIONTEMPERATURESENSOR_H
