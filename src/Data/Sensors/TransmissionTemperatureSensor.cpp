//
// Created by jlaustill on 7/19/21.
//

#include "TransmissionTemperatureSensor.h"

int samples[NUMSAMPLES] = {1,1,1,1,1};
uint16_t sampleIndex = 0;

byte TransmissionTemperatureSensor::readNextValue() {
    uint8_t i;                                          //integer for loop
    float average;                                      //decimal for average

    samples[sampleIndex++] = analogRead(coolantsensorPin);  // each loop we take a new sample and update the circular array
    if (sampleIndex >= NUMSAMPLES) {
        sampleIndex = 0; // circular structure reset
    }

    average = 0;
    Serial.print("Samples: ");
    for (i=0; i< NUMSAMPLES; i++) {
        Serial.print((String)samples[i] + " ");
        average += samples[i];                            //adds all number of samples together
    }
    Serial.println();
    average /= NUMSAMPLES;                              //divides by number of samples to output the average

    Serial.print("Average Analog Coolant Reading = ");
    Serial.println(average);                                     //analog value at analog pin into arduino
    average = (coolantsensorDivider*average)/(1023-average);        //conversion equation to read resistance from voltage divider
    Serial.print("Coolant Sensor Resistance = ");
    Serial.println(average);

    float steinhart;                              //steinhart equation to estimate temperature value at any resistance from curve of thermistor sensor
    steinhart = log(average);                     //lnR
    steinhart = pow(steinhart,3);                 //(lnR)^3
    steinhart *= steinconstC;                     //C*((lnR)^3)
    steinhart += (steinconstB*(log(average)));    //B*(lnR) + C*((lnR)^3)
    steinhart += steinconstA;                     //Complete equation, 1/T=A+BlnR+C(lnR)^3
    steinhart = 1.0/steinhart;                    //Inverse to isolate for T
    steinhart -= 273.15;                          //Conversion from kelvin to celcius

    Serial.print("Temperature = ");
    Serial.print(steinhart);                      //prints final temp in celcius
    Serial.println(" *C");

    return (byte)steinhart; // does odb2 use F|C?
}
