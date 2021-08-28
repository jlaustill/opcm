//
// Created by jlaustill on 8/21/21.
//
#include "Configuration.h"
#ifdef CAN_BUS
#include <Arduino.h>
// mcp2515 uses SPI, so include the SPI library
#include <SPI.h>

#include "CanBus.h"

#define CanBusSPI_BEGIN()        SPIClass::beginTransaction(SPISettings(4000000, MSBFIRST, SPI_MODE0))
#define CanBusSPI_END()          SPIClass::endTransaction()

byte mphToKph2(int mph) {
    double kph = mph * 1.60934;
    return (byte)round(kph);
}
byte dataCB[13] = {0,0,0,0,0,0,0,0,0,0,0,0,0};
unsigned long canid = 0x7E8;
//volatile byte dataCB[13];
volatile byte messageSupportedPids[13] = {(byte)(canid >> 3), (byte)((canid & 0x07) << 5), 0, 0, 8, 0x06,  0x41,  0x00,    B10001000, B00011000, B00000000, B00000000, 0x00 };
volatile byte messageSupportedPids32[13] = {(byte)(canid >> 3), (byte)((canid & 0x07) << 5), 0, 0, 8, 0x06,  0x41,  0x20,  B00000000, B00010000, B00000000, B00000001, 0x00 };
volatile byte messageSupportedPids64[13] = {(byte)(canid >> 3), (byte)((canid & 0x07) << 5), 0, 0, 8, 0x06,  0x41,  0x40,  B00000000, B00000000, B00000000, B00000001, 0x00 };
volatile byte messageSupportedPids96[13] = {(byte)(canid >> 3), (byte)((canid & 0x07) << 5), 0, 0, 8, 0x06,  0x41,  0x60,  B00000000, B00001000, B00000000, B00000000, 0x00 };
volatile byte messageBaroResponse[13] = {(byte)(canid >> 3), (byte)((canid & 0x07) << 5), 0, 0, 8, 4,  51, 5, 0, 0, 0, 0, 0 };
volatile byte messageCat1Response[13] = {(byte)(canid >> 3), (byte)((canid & 0x07) << 5), 0, 0, 8, 4,  62, 5, 5, 0, 0, 0, 0 };
volatile byte messageMilCleared[13] =   {(byte)(canid >> 3), (byte)((canid & 0x07) << 5), 0, 0, 8, 4, 65, 63, 34, 224, 185, 147};
volatile byte messageWaterTempResponse[13] = {(byte)(canid >> 3), (byte)((canid & 0x07) << 5), 0, 0, 8, 4, 65, 5, 0, 0, 0, 0 };
volatile byte messageRpmResponse[13] = {(byte)(canid >> 3), (byte)((canid & 0x07) << 5), 0, 0, 8, 4, 65, 12, 0, 0, 0, 0 };
volatile byte messageMphResponse[13] = {(byte)(canid >> 3), (byte)((canid & 0x07) << 5), 0, 0, 8, 4, 65, 13, 0, 0, 0, 0, 0 };
volatile byte messageOilPressureResponse[13] = {(byte)(canid >> 3), (byte)((canid & 0x07) << 5), 0, 0, 8, 4, 65, 200, 0, 0, 0, 0, 0 };

void writeRegister2(int CSpin, byte thisRegister, byte thisValue) {
    CanBusSPI_BEGIN();
    // take the chip select low to select the device:
    digitalWrite(CSpin, LOW);
    SPIClass::transfer(0x02);         //write register command (02)
    SPIClass::transfer(thisRegister); //Send register address
    SPIClass::transfer(thisValue);  //Send value to record into register
    // take the chip select high to de-select:
    digitalWrite(CSpin, HIGH);
    CanBusSPI_END();
}

byte readRegister2(int CSpin, byte thisRegister) {
    byte result = 0;
    CanBusSPI_BEGIN();
    digitalWrite(CSpin, LOW);
    SPIClass::transfer(0x03);                 //read register command (03)
    SPIClass::transfer(thisRegister);
    result = SPIClass::transfer(0x00);        //shift out dummy byte (00) so "thisRegister" gets clocked into result
    digitalWrite(CSpin, HIGH);
    CanBusSPI_END();
    return result;
}

void sendMessage(volatile byte _data[13]) {
    //Load Transmit buffer 0
//    while ((readRegister2(CAN_BUS_CS_PIN, 0x30) & 0x08)) {  //first wait for any pending tx, checks success or fail
//    }

    CanBusSPI_BEGIN();
    digitalWrite(CAN_BUS_CS_PIN, LOW);
    SPIClass::transfer(0x40);           //load tx buffer 0 CMD
    for (int i = 0; i < 13; i++) {
        SPIClass::transfer(_data[i]); //copy each byte that came in and substitute the modified bytes
    }
    digitalWrite(CAN_BUS_CS_PIN, HIGH);
    CanBusSPI_END();

    //send the transmit buffer, all bytes
    //RTS tx buffer 0
    CanBusSPI_BEGIN();
    digitalWrite(CAN_BUS_CS_PIN, LOW);
    SPIClass::transfer(0x81);   //RTS command
    digitalWrite(CAN_BUS_CS_PIN, HIGH);
    CanBusSPI_END();
}

void ISR_trig1() {
//    Serial.print("Message: ");
    CanBusSPI_BEGIN();
    digitalWrite(CAN_BUS_CS_PIN, LOW);

    SPIClass::transfer(0x90);   //read buffer 0 Command. also clears the CAN module interrupt flag
    for(int i = 0; i < 13; i++){
        dataCB[i] = SPIClass::transfer(0x00);
//        Serial.print(dataCB[i], HEX);
//        Serial.print(" ");
    }

    digitalWrite(CAN_BUS_CS_PIN, HIGH);
    CanBusSPI_END();

//    Serial.println();
    int id = (dataCB[0] << 3) + (dataCB[1] >> 5);
    if ((dataCB[1] & 0x08) ==  0x08) {
        /* extended id                  */
        id = (id << 2) + (dataCB[1] & 0x03);
        id = (id << 8) + dataCB[2];
        id = (id << 8) + dataCB[3];

    }

//    Serial.println("CANbus message received from: " + (String)id + " message: " + BuildMessage + " ? " + (String)dataCB[5] + " " + (String)dataCB[6]);

    if (dataCB[6] == 1 && dataCB[7] == 0) {
        sendMessage(messageSupportedPids);
        Serial.println("supported pids...");
    } else if (dataCB[6] == 1 && dataCB[7] == 32) {
        sendMessage(messageSupportedPids32);
    } else if (dataCB[6] == 1 && dataCB[7] == 64) {
        sendMessage(messageSupportedPids64);
    } else if (dataCB[6] == 1 && dataCB[7] == 96) {
        sendMessage(messageSupportedPids96);
    } else if (dataCB[6] == 1 && dataCB[7] == 1) {
        sendMessage(messageMilCleared);
    } else if (dataCB[6] == 1 && dataCB[7] == 5) {
        sendMessage(messageWaterTempResponse);
    } else if (dataCB[6] == 1 && dataCB[7] == 12) {
        sendMessage(messageRpmResponse);
    } else if (dataCB[6] == 1 && dataCB[7] == 13) {
        sendMessage(messageMphResponse);
    } else if (dataCB[6] == 1 && dataCB[7] == 51) {
        sendMessage(messageBaroResponse);
    } else if (dataCB[6] == 0x3E && dataCB[7] == 0x80) { // ID: 7DF8 2 3E 80 0 0 0 0 0
        sendMessage(messageCat1Response); // no idea what this is asking for
    } else {
        Serial.print("ID: ");
        Serial.print(id, 16);
        Serial.print(" ");
        for(int i = 4; i < 13; i++){
            Serial.print(i);
            Serial.print(": ");
            Serial.print(dataCB[i], 16);
            Serial.print(" ");
        }
        Serial.println();
    }
;
    for(int i = 0; i < 13; i++){
        dataCB[i] = 0;
    }
}

void CanBus::initialize() {
    Serial.println("Canbus initializing can bus");
    // start the SPI library:
    SPIClass::begin();
    //default settings, but shown here for learning example.
//    SPIClass::setBitOrder(MSBFIRST);
//    SPIClass::setDataMode(SPI_MODE0);
//    SPIClass::setClockDivider(SPI_CLOCK_DIV2);

    // initalize the  intterupts, ss, and chip select pins:

    pinMode(53, OUTPUT); // Arduino is master
    pinMode(CAN_BUS_INT_PIN, INPUT);  //INT0
    pinMode(CAN_BUS_CS_PIN, OUTPUT);

    CanBusSPI_BEGIN();
    // take the chip select low to select the device
    digitalWrite(CAN_BUS_CS_PIN, LOW);
    SPIClass::transfer(0xC0); //reset CAN1 command & enter config mode
    // take the chip select high to de-select
    digitalWrite(CAN_BUS_CS_PIN, HIGH);
    CanBusSPI_END();

    // give the mcp2515 time to reset
    delay(10);

    //do this once for CAN1
    //set baud rate to 8MHZ osc 250Kb/s
    writeRegister2(CAN_BUS_CS_PIN, 0x2A, 0x40);  //cnf1
    writeRegister2(CAN_BUS_CS_PIN, 0x29, 0xF1);  //cnf2
    writeRegister2(CAN_BUS_CS_PIN, 0x28, 0x85);  //cnf3

    //recieve buff 0 apply mask and filters SID or EXID
    writeRegister2(CAN_BUS_CS_PIN, 0x60, 0x60);  // 60 RXB0CTRL rec all. ignore filters

    //SOF signal output to mcp2515 CLCKout pin 3 for O'scope trigger
    writeRegister2(CAN_BUS_CS_PIN, 0x0F, 0x04);  //canctrl ******normal mode***** //one shot mode trans disabled

    if(readRegister2(CAN_BUS_CS_PIN, 0x0E)== 0 && readRegister2(CAN_BUS_CS_PIN, 0x0F) == 04){ //mode status to check if init happened correctly
        Serial.println("Canbus CAN module init Normal ");
    }
    else{
        Serial.println("Canbus CAN module init FAILED !! ");
    }

    byte i, a1, a2, a3;

    a1 = 0x30;
    a2 = 0x40;
    a3 = 0x50;
    for (i = 0; i < 14; i++) {                       // in-smoothingBuffer loop
        writeRegister2(CUMMINS_BUS_INPUT_CS_PIN, a1, 0);
        writeRegister2(CUMMINS_BUS_INPUT_CS_PIN, a2, 0);
        writeRegister2(CUMMINS_BUS_INPUT_CS_PIN, a3, 0);
        a1++;
        a2++;
        a3++;
    }
    writeRegister2(CUMMINS_BUS_INPUT_CS_PIN, 0x60, 0);
    writeRegister2(CUMMINS_BUS_INPUT_CS_PIN, 0x70, 0);

    //CAN 1 module enable REC buffer 0 interrupt
    writeRegister2(CAN_BUS_CS_PIN, 0x2B, 0x01);
    SPIClass::usingInterrupt(digitalPinToInterrupt(CAN_BUS_INT_PIN));
    attachInterrupt(digitalPinToInterrupt(CAN_BUS_INT_PIN), ISR_trig1, FALLING);

}

void CanBus::setRpms(int _rpms) {
    messageRpmResponse[8] = highByte(_rpms << 2);
    messageRpmResponse[9] = lowByte(_rpms << 2);
//    volatile byte temp[13] = {(byte)(1000 >> 3), (byte)((1000 & 0x07) << 5), 0, 0, 8, 0, messageRpmResponse[8], messageRpmResponse[9], 0, 0, 0, 0, 0};
//    sendMessage(temp);
}

void CanBus::setMph(int _mph) {
    messageMphResponse[8] = mphToKph2(_mph);
}

void CanBus::setCoolantTemp(int _temp) {
    messageWaterTempResponse[8] = _temp + 40;
}

void CanBus::sendOilPressure(byte _oilPressure) {
    messageOilPressureResponse[8] = _oilPressure;
    sendMessage(messageOilPressureResponse);
}

#endif
