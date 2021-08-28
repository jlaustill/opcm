//
// Created by jlaustill on 7/19/21.
//

#include "TransmissionTemperatureSensor.h"
#include "Configuration.h"

int samples[NUMSAMPLES] = {1,1,1,1,1};
uint16_t sampleIndex = 0;

byte TransmissionTemperatureSensor::getTransmissionTemperatureInCelcius() {
//    uint8_t i;                                          //integer for loop
    float average;                                      //decimal for average

    samples[sampleIndex++] = analogRead(TRANSMISSION_TEMPERATURE_INPUT_PIN);  // each loop we take a new sample and update the circular array
    if (sampleIndex >= NUMSAMPLES) {
        sampleIndex = 0; // circular structure reset
    }

    average = 0;
//    Serial.print("Samples: ");
//    for (i=0; i< NUMSAMPLES; i++) {
//        Serial.print((String)samples[i] + " ");
//        average += samples[i];                            //adds all number of samples together
//    }
//    Serial.println();
    average /= NUMSAMPLES;                              //divides by number of samples to output the average

//    Serial.print("Average Analog Coolant Reading = ");
//    Serial.println(average);                                     //analog value at analog pin into arduino
    average = (TRANSMISSION_TEMPERATURE_INPUT_DIVIDER*average)/(1023-average);        //conversion equation to read resistance from voltage divider
//    Serial.print("Coolant Sensor Resistance = ");
//    Serial.println(average);

    average = 4918.83 - 4663.82 * pow(average, 0.00672081);                       //Conversion from resistance to C
//
//    Serial.print("Temperature = ");
//    Serial.print(average);                      //prints final temp in celcius
//    Serial.println(" *C");

    return (byte)average; // does odb2 use F|C?
}
