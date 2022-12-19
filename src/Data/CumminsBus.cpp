//
// Created by jlaustill on 8/21/21.
//
#include "Configuration.h"
#ifdef CUMMINS_BUS_INPUT
#include <Arduino.h>
#include <FlexCAN_T4.h>
FlexCAN_T4<CAN1, RX_SIZE_256, TX_SIZE_16> Can2;
#include "CumminsBus.h"

volatile byte data[16];

struct CanMessage {
    byte id;
    byte length;
    byte data[8];
    unsigned count;
};


volatile CanMessage message419360256{0x67, 0x8, {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}, 0};
volatile CanMessage message419360512{};
volatile CanMessage message256{};
volatile CanMessage message274{};
volatile CanMessage x67984Message{0x67, 0x8, {0xFE, 0x7D, 0x7D, 0xC0, 0x1, 0xFF, 0xFF, 0xFF}, 0};
volatile CanMessage message217056000{0x67, 0x8, {0xF1, 0x0, 0x0, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, 0};
volatile CanMessage message419362304{};

// Our data, how exciting!
volatile unsigned long RPM, FuelLongM, FuelLongR, TimingLongM, TimingLongR;
volatile double fuelTemp; // +
volatile double oilPressure = 0; // +
volatile double waterTemp = 0; // +
volatile double load = 0; // +
volatile unsigned long throttlePercentage = 0; // +
volatile float Timing; // +
volatile float FuelPCT; // +

volatile int ids[8] = {0,0,0,0,0,0,0,0};
volatile int idsP = 0;

void updateMessage(volatile CanMessage* _messageToUpdate, CAN_message_t msg) {
    _messageToUpdate->id = msg.id;
    _messageToUpdate->length = msg.len;
    _messageToUpdate->data[0] = msg.buf[0];
    _messageToUpdate->data[1] = msg.buf[1];
    _messageToUpdate->data[2] = msg.buf[2];
    _messageToUpdate->data[3] = msg.buf[3];
    _messageToUpdate->data[4] = msg.buf[4];
    _messageToUpdate->data[5] = msg.buf[5];
    _messageToUpdate->data[6] = msg.buf[6];
    _messageToUpdate->data[7] = msg.buf[7];
    _messageToUpdate->count++;
}

void CumminsBusSniff(const CAN_message_t &msg) {
    for(uint8_t i = 0; i < msg.len; i++){
        data[i] = msg.buf[i];
    }
    
    if (msg.id == 419360256) {
        updateMessage(&message419360256, msg);
    }
    else if (msg.id == 419360512) {
        updateMessage(&message419360512, msg);
    }
    else if (msg.id == 419362304) {
        updateMessage(&message419362304, msg);
    }
    else if (msg.id == 217056000) {
        updateMessage(&message217056000, msg);

       // computer APP/throttle percentage
       throttlePercentage = message217056000.data[1] * 100 / 255;
    }
//  else if (data[0] == 0x67 && data[1] == 0x98 && data[2] == 0x4) {
//         updateMessage(&x67984Message, msg.id);

// //        // Compute Load %
// //        load = x67984Message.data[2];
// //        load -= 125;
// //        load = load * 4 / 5;
//     }
 else if (msg.id == 274) {
        updateMessage(&message274, msg);
    } 
    else if (msg.id == 256) {
        updateMessage(&message256, msg);

       //Fuel compute
       FuelPCT = ((message256.data[1] << 8) | message256.data[0]);
       FuelPCT = (float)(FuelPCT*100) / 4096;    // 4095 is max fuel allowed by pump

       // compute timing advance
       Timing = (message256.data[5] << 8) | message256.data[4]; //convert from little endian. 128 bits per degree.
       Timing = (float)((Timing) / 128);
    } else {
        ids[idsP++] = msg.id;
        if (idsP >= 8) idsP = 0;
    }
}// ISR done

int CumminsBus::getCurrentRpms() {
//    Serial.println("x20 id? " + (String)x20Message.computedId);
    //compute RPM from ID 20xx bytes 7 and 6
    RPM = (message256.data[7] << 8) | message256.data[6];     //convert from little endian
    RPM /= 4 ;
    return (int)RPM;
}

int CumminsBus::getCurrentBoostInPsi() {
    double kpa = message419362304.data[1] * 2;
    double psi = kpa * 0.1450377377 - 10; 
    return psi;
}

int CumminsBus::getCurrentWaterTemp() {
    // Compute Water Temperature
    // waterTemp = ((message419360256.data[1] >> 8) | message419360256.data[0]) - 40; // celcius water temp!!! // My guess
//    waterTemp = waterTemp * 9 / 5 + 32; // F
//Serial.println("water temp? " + (String)waterTemp + " " + (String)C7FAEEMessage.count);
//    for (int a = 0; a < 8; a++) {
//        Serial.print(ids[a]);
//        Serial.print(" ");
//    }
//    Serial.println();

    Serial.print("IAT temp? ");
    Serial.println(message419360256.data[1] - 40);

    waterTemp = message419360256.data[0] - 40;
    return (int)waterTemp;
}

byte CumminsBus::getCurrentOilPressure() {
    // Compute Oil Pressure
    oilPressure = message419360512.data[3] * 4 / 6.895;
    return (byte)oilPressure;
}

int CumminsBus::getCurrentFuelTemp() {
    // Compute Fuel Temperature
    fuelTemp = (message274.data[7] << 8) | message274.data[6]; // Raw
    fuelTemp = fuelTemp / 16; // Kelvin
    fuelTemp = fuelTemp - 273.15; // Celsius
    fuelTemp = ((fuelTemp * 9) / 5) + 32; // Fahrenheit

    return fuelTemp;
}

void CumminsBus::initialize() {
    Serial.println("Cummins Bus initializing");

    Can2.begin();
    Can2.setBaudRate(250000);
    Can2.setMaxMB(16);
    Can2.enableFIFO();
    Can2.enableFIFOInterrupt();
    Can2.onReceive(CumminsBusSniff);
    Can2.mailboxStatus();
}

#endif
