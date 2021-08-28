//
// Created by jlaustill on 8/21/21.
//
#include "Configuration.h"
#ifdef CUMMINS_BUS_INPUT
#include <Arduino.h>
// mcp2515 uses SPI, so include the SPI library
#include <SPI.h>
#include "CumminsBus.h"

#define CumminsBusSPI_BEGIN()        SPIClass::beginTransaction(SPISettings(4000000, MSBFIRST, SPI_MODE0))
#define CumminsBusSPI_END()          SPIClass::endTransaction()

volatile byte data[16];

struct CanMessage {
    byte id;
    byte unknown;
    byte unknown2;
    int computedId;
    byte length;
    byte data[8];
    unsigned count;
};


volatile CanMessage C7FAEEMessage{};
volatile CanMessage C7FAEFMessage{};
volatile CanMessage x20Message{};
volatile CanMessage x22Message{};
volatile CanMessage x67984Message{0x67, 0x98, 0x4, 0, 0x8, {0xFE, 0x7D, 0x7D, 0xC0, 0x1, 0xFF, 0xFF, 0xFF}, 0};
volatile CanMessage x67983Message{0x67, 0x98, 0x3, 0, 0x8, {0xF1, 0x0, 0x0, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, 0};


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

void writeRegister(int CSpin, byte thisRegister, byte thisValue) {
    // take the chip select low to select the device:
    CumminsBusSPI_BEGIN();
    digitalWrite(CSpin, LOW);
    SPIClass::transfer(0x02);         //write register command (02)
    SPIClass::transfer(thisRegister); //Send register address
    SPIClass::transfer(thisValue);  //Send value to record into register
    // take the chip select high to de-select:
    digitalWrite(CSpin, HIGH);
    CumminsBusSPI_END();
}

byte readRegister(int CSpin, byte thisRegister) {
    byte result = 0;
    CumminsBusSPI_BEGIN();
    digitalWrite(CSpin, LOW);
    SPIClass::transfer(0x03);                 //read register command (03)
    SPIClass::transfer(thisRegister);
    result = SPIClass::transfer(0x00);        //shift out dummy byte (00) so "thisRegister" gets clocked into result
    digitalWrite(CSpin, HIGH);
    CumminsBusSPI_END();
    return result;
}

void updateMessage(volatile CanMessage* _messageToUpdate, int _id) {
    _messageToUpdate->computedId = _id;
    _messageToUpdate->id = data[0];
    _messageToUpdate->unknown = data[1];
    _messageToUpdate->unknown2 = data[2];
    _messageToUpdate->length = data[4];
    _messageToUpdate->data[0] = data[5];
    _messageToUpdate->data[1] = data[6];
    _messageToUpdate->data[2] = data[7];
    _messageToUpdate->data[3] = data[8];
    _messageToUpdate->data[4] = data[9];
    _messageToUpdate->data[5] = data[10];
    _messageToUpdate->data[6] = data[11];
    _messageToUpdate->data[7] = data[12];
    _messageToUpdate->count++;
}

void ISR_trig0(){
    CumminsBusSPI_BEGIN();
    digitalWrite(CUMMINS_BUS_INPUT_CS_PIN, LOW);
    SPIClass::transfer(0x90);   //read buffer 0 Command. also clears the CAN module interrupt flag
    for(int i = 0; i < 13; i++){
        data[i] = SPIClass::transfer(0x00);
    }
    digitalWrite(CUMMINS_BUS_INPUT_CS_PIN, HIGH);
    CumminsBusSPI_END();

    int id = (data[0] << 3) + (data[1] >> 5);
    if ((data[1] & 0x08) ==  0x08) {
        /* extended id                  */
        id = (id << 2) + (data[1] & 0x03);
        id = (id << 8) + data[2];
        id = (id << 8) + data[3];

    }
    if (data[0] == 0xC7 && data[1] == 0xFA && data[2] == 0xEE) {
        updateMessage(&C7FAEEMessage, id);
    } else if (id == -4352) {//data[0] == 0xC7 && data[1] == 0xFA && data[2] == 0xEF) { // id == -4608) {//
//        Serial.println("received water temp");
        updateMessage(&C7FAEFMessage, id);

//        // Compute Oil Pressure
//        oilPressure = C7FAEFMessage.data[3] * 4 / 6.895;
    } else if (data[0] == 0x67 && data[1] == 0x98 && data[2] == 0x3) {
        updateMessage(&x67983Message, id);

//        // computer APP/throttle percentage
//        throttlePercentage = x67983Message.data[1] * 100 / 255;
    }  else if (data[0] == 0x67 && data[1] == 0x98 && data[2] == 0x4) {
        updateMessage(&x67984Message, id);

//        // Compute Load %
//        load = x67984Message.data[2];
//        load -= 125;
//        load = load * 4 / 5;
    } else if (data[0] == 0x22) {
        updateMessage(&x22Message, id);

//        // Compute Fuel Temperature
//        fuelTemp = (x22Message.data[7] << 8) | x22Message.data[6]; // Raw
//        fuelTemp = fuelTemp / 16; // Kelvin
//        fuelTemp = fuelTemp - 273.15; // Celsius
//        fuelTemp = ((fuelTemp * 9) / 5) + 32; // Fahrenheit
    } else if (id == 256) {
        updateMessage(&x20Message, id);

//        //Fuel compute
//        FuelPCT = ((x20Message.data[1] << 8) | x20Message.data[0]);
//        FuelPCT = (float)(FuelPCT*100) / 4096;    // 4095 is max fuel allowed by pump
//
//        // compute timing advance
//        Timing = (x20Message.data[5] << 8) | x20Message.data[4]; //convert from little endian. 128 bits per degree.
//        Timing = (float)((Timing) / 128);
    } else {
        ids[idsP++] = id;
        if (idsP >= 8) idsP = 0;
    }
}// ISR done

int CumminsBus::getCurrentRpms() {
//    Serial.println("x20 id? " + (String)x20Message.computedId);
    //compute RPM from ID 20xx bytes 7 and 6
    RPM = (x20Message.data[7] << 8) | x20Message.data[6];     //convert from little endian
    RPM /= 4 ;
    return (int)RPM;
}

int CumminsBus::getCurrentWaterTemp() {
    // Compute Water Temperature
    waterTemp = ((C7FAEEMessage.data[1] >> 8) | C7FAEEMessage.data[0]) - 40; // celcius water temp!!!
//    waterTemp = waterTemp * 9 / 5 + 32; // F
//Serial.println("water temp? " + (String)waterTemp + " " + (String)C7FAEEMessage.count);
//    for (int a = 0; a < 8; a++) {
//        Serial.print(ids[a]);
//        Serial.print(" ");
//    }
//    Serial.println();
    return (int)waterTemp;
}

byte CumminsBus::getCurrentOilPressure() {
    // Compute Oil Pressure
    oilPressure = C7FAEFMessage.data[3] * 4 / 6.895;
    return (byte)oilPressure;
}

void CumminsBus::initialize() {
    Serial.println("Cummins Bus initializing");
    // start the SPI library:
    SPIClass::begin();
    //default settings, but shown here for learning example.
//    SPIClass::setBitOrder(MSBFIRST);
//    SPIClass::setDataMode(SPI_MODE0);
//    SPIClass::setClockDivider(SPI_CLOCK_DIV2);

    // initalize the  intterupts, ss, and chip select pins:

    pinMode(53, OUTPUT);    //Arduino is master
    pinMode(CUMMINS_BUS_INPUT_INT_PIN, INPUT);  //INT0
    pinMode(CUMMINS_BUS_INPUT_CS_PIN, OUTPUT);

    CumminsBusSPI_BEGIN();
    // take the chip select low to select the device
    digitalWrite(CUMMINS_BUS_INPUT_CS_PIN, LOW);
    SPIClass::transfer(0xC0); //reset CAN1 command & enter config mode
    // take the chip select high to de-select
    digitalWrite(CUMMINS_BUS_INPUT_CS_PIN, HIGH);
    CumminsBusSPI_END();

    // give the mcp2515 time to reset
    delay(10);

    //do this once for CAN1
    //set baud rate to 8MHZ osc 250Kb/s
    writeRegister(CUMMINS_BUS_INPUT_CS_PIN, 0x2A, 0x40);  //cnf1
    writeRegister(CUMMINS_BUS_INPUT_CS_PIN, 0x29, 0xF1);  //cnf2
    writeRegister(CUMMINS_BUS_INPUT_CS_PIN, 0x28, 0x85);  //cnf3

    //recieve buff 0 apply mask and filters SID or EXID
    writeRegister(CUMMINS_BUS_INPUT_CS_PIN, 0x60, 0x60);  // 60 RXB0CTRL rec all. ignore filters

    //SOF signal output to mcp2515 CLCKout pin 3 for O'scope trigger
    writeRegister(CUMMINS_BUS_INPUT_CS_PIN, 0x0F, 0x04);  //canctrl ******normal mode***** //one shot mode trans disabled

    if(readRegister(CUMMINS_BUS_INPUT_CS_PIN, 0x0E)== 0 && readRegister(CUMMINS_BUS_INPUT_CS_PIN, 0x0F) == 04){ //mode status to check if init happened correctly
        Serial.println("Cummins Bus CAN module init Normal");
    }
    else{
        Serial.println("Cummins Bus CAN module init FAILED!! ");
    }



//    byte i, a1, a2, a3;
//
//    a1 = 0x30;
//    a2 = 0x40;
//    a3 = 0x50;
//    for (i = 0; i < 14; i++) {                       // in-smoothingBuffer loop
//        writeRegister(CUMMINS_BUS_INPUT_CS_PIN, a1, 0);
//        writeRegister(CUMMINS_BUS_INPUT_CS_PIN, a2, 0);
//        writeRegister(CUMMINS_BUS_INPUT_CS_PIN, a3, 0);
//        a1++;
//        a2++;
//        a3++;
//    }
//    writeRegister(CUMMINS_BUS_INPUT_CS_PIN, 0x60, 0);
//    writeRegister(CUMMINS_BUS_INPUT_CS_PIN, 0x70, 0);


    //CAN 1 module enable REC buffer 0 interrupt
    writeRegister(CUMMINS_BUS_INPUT_CS_PIN, 0x2B, 0x01);



    SPIClass::usingInterrupt(digitalPinToInterrupt(CUMMINS_BUS_INPUT_INT_PIN));
    attachInterrupt(digitalPinToInterrupt(CUMMINS_BUS_INPUT_INT_PIN), ISR_trig0, FALLING);
}

#endif
