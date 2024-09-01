//
// Created by jlaustill on 8/21/21.
//

#ifndef J1939_CPP
#define J1939_CPP

#define thirdByte(w) ((uint8_t)((w) >> 16))
#define fourthByte(w) ((uint8_t)((w) >> 24))
#include "Configuration.h"
#ifdef CUMMINS_BUS_INPUT
#include <Arduino.h>
#include <FlexCAN_T4.h>
FlexCAN_T4<CAN1, RX_SIZE_256, TX_SIZE_16> Can1;
#include <J1939.h>

#include <SeaDash.hpp>
#include <cstdint>
#include <unordered_set>

#include "CumminsBus.h"

J1939 message;
std::unordered_set<uint8_t> sourceAddresses;

#define DM1_DTCS_PGN 65226
#define ENGINE_TEMP_1_PGN 65262
#define CRUISE_CONTROL_PGN 65265
#define DM2_DTCS_PGN 65227

// Track update timings
uint32_t lastJ1939Request = 0;

// volatile byte data[16];
boolean warmedUp = false;

// J1939 message structure
struct J1939Message {
  std::uint32_t id;
  std::uint8_t data[8];
};

volatile CanMessage pgn65262{
    0x67, 0x8, {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}, 0};
volatile CanMessage pgn65263{};
volatile CanMessage pgn65272{};
volatile CanMessage pgn61445{};
volatile CanMessage CumminsBus::pgn61442{};
volatile CanMessage message256{};
volatile CanMessage message274{};
volatile CanMessage x67984Message{
    0x67, 0x8, {0xFE, 0x7D, 0x7D, 0xC0, 0x1, 0xFF, 0xFF, 0xFF}, 0};
volatile CanMessage pgn61443{
    0x67, 0x8, {0xF1, 0x0, 0x0, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}, 0};
volatile CanMessage pgn65270{};
CAN_message_t msg;

// Our data, how exciting!
volatile unsigned long RPM, FuelLongM, FuelLongR, TimingLongM, TimingLongR;
volatile double fuelTemp;              // +
volatile double oilPressure = 0;       // +
volatile double waterTemp = 0;         // +
volatile byte load = 0;                // +
volatile byte throttlePercentage = 0;  // +
volatile float Timing;                 // +
volatile float FuelPercentage;         // +

// tuning data
volatile float maxTiming = 15.0f;
volatile int maxOfThrottleAndLoad = 0;
volatile float newTiming = 15.0f;
volatile unsigned short shortTimingValue = 0;

volatile int ids[8] = {0, 0, 0, 0, 0, 0, 0, 0};
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

byte* getIdBytes(uint32_t id) {
  byte* idBytes = new byte[4];

  idBytes[0] = id >> 24;
  idBytes[1] = id >> 16;
  idBytes[2] = id >> 8;
  idBytes[3] = id;

  return idBytes;
}

void requestPgn(uint32_t pgn) {
  J1939 tempMessage = J1939();
  tempMessage.setPgn(pgn);
  tempMessage.setSourceAddress(50);
  tempMessage.setPriority(6);
  tempMessage.setPduSpecific(255);

  // pgn to request water temp pgn :)
  msg.flags.extended = 1;

  // msg.id = 2364145912;
  msg.id = tempMessage.canId;
  msg.len = 3;
  msg.buf[0] = lowByte(pgn);
  msg.buf[1] = highByte(pgn);
  msg.buf[2] = thirdByte(pgn);
  Can1.write(msg);  // 59640 sa 0
}

void CumminsBusSniff(const CAN_message_t& msg) {
  message = J1939();
  message.setCanId(msg.id);
  message.setData(msg.buf);

  sourceAddresses.insert(message.sourceAddress);

  if (message.canId == 256) {
    // PGN: 1 ????
    updateMessage(&message256, msg);
    CAN_message_t newMsg = msg;
    CumminsBus::updateTiming(newMsg);
  } else if (message.canId == 274) {
    // Must come from the VP44, no messages when bench testing
    updateMessage(&message274, msg);
  } else if (message.pgn == 60415) {
    // what's this?
  } else if (message.pgn == 60671) {
    // what's this?
  } else if (message.pgn == 61440) {
    // Electronic Retarder Controller 1 - ERC1 -
  } else if (message.pgn == 61443) {
    // PGN: 61443
    updateMessage(&pgn61443, msg);
  } else if (message.pgn == 61444) {
    // Electronic Engine Controller 1 - EEC1 -
  } else if (message.pgn == 65247) {
    // Electronic Engine Controller 3 - EEC3 -
  } else if (message.pgn == 65252) {
    // Shutdown - SHUTDOWN -
    // wait to start lamp spn 1081 (doesn't seem to work bench testing)
  } else if (message.pgn == ENGINE_TEMP_1_PGN) {
    // PGN: 65262
    // Engine Temperature 1 - ET1
    // Engine coolant temperature 1 spn 110
    // fuel temperate 2 spn 174
    // engine oil temperature 3-4 spn 175
    // turbo oil temperature 5-6 spn 176
    // enginer intercooler temperature 7 spn 52
    // engine intercooler thermostat 8 spn 1134
    updateMessage(&pgn65262, msg);
  } else if (message.pgn == 65263) {
    // PGN: 65263
    // Engine Fluid Level/Pressure 1 - EFL/P1 -
    updateMessage(&pgn65263, msg);
  } else if (message.pgn == 65264) {
    // Power Takeoff Information - PTO -
  } else if (message.pgn == CRUISE_CONTROL_PGN) {
    // don't appear to be getting anything useful at all blah
    // coming from ECU, interesting
    // PN 65265, SA:0 243 0 0 16 0 0 31 255
    // 31 means cruise control not available, sad panda
    // Serial.println("PN 65265, SA:" + (String)message.sourceAddress + " " +
    //                message.data[0] + " " + message.data[1] + " " +
    //                message.data[2] + " " + message.data[3] + " " +
    //                message.data[4] + " " + message.data[5] + " " +
    //                message.data[6] + " " + message.data[7]);

    // Cruise Control/Vehicle Speed - CCVS -
  } else if (message.pgn == 65266) {
    // Fuel Economy (Liquid) - LFE -
  } else if (message.pgn == 65269) {
    // Ambient Conditions - AMB -
    // AIT air inlent temp spn 172
  } else if (message.pgn == 65270) {
    // PGN: 65270
    // Inlet/Exhaust Conditions 1 - IC1 -
    updateMessage(&pgn65270, msg);
  } else if (message.pgn == 65271) {
    // Vehicle Electrical Power - VEP -
  } else if (message.pgn == 65504) {
    // what's this?
  } else if (message.pgn == 65272) {
    // Transmission Fluids, not sure how much of this data is useful except temp
    updateMessage(&pgn65272, msg);
    // Serial.println(
    //     "PGN: " + (String)message.pgn + " Data: " + (String)message.data[0] +
    //     " " + (String)message.data[1] + " " + (String)message.data[2] + " " +
    //     (String)message.data[3] + " " + (String)message.data[4] + " " +
    //     (String)message.data[5] + " " + (String)message.data[6] + " " +
    //     (String)message.data[7]);
    // std::uint16_t tempRaw = (message.data[5] << 8) | message.data[4];
    // float coolantTemp = (float)tempRaw * 0.03125f - 273.15f;
    // Serial.println("Coolant Temp: " + (String)coolantTemp);
    uint16_t clutchPressure = message.data[0] * 16;
    uint16_t transmissionFilterDifferentialPressure = message.data[2] * 2;
    uint16_t transmissionOilPressure = message.data[3] * 16;
    // Serial.println("Clutch Pressure: " + (String)clutchPressure +
    //                "kPa Transmission Filter Differential Pressure: " +
    //                (String)transmissionFilterDifferentialPressure +
    //                "kPa Transmission Oil Pressure: " +
    //                (String)transmissionOilPressure + "kPa");

  } else if (message.pgn == 61442) {
    // Electronic Tranmission COntroller 1 - ETC1 -
    // 1.1 2 bits driveline engaged spn 560
    // 1.3 2 bits converter lockeup engaged spn 573
    // 1.5 2 bits shift in progress spn 574
    // 2-3 output shaft speed spn 191
    // 4 percent clutch slip spn 522
    // 5.1 2 bits momentary engine overspeed enable spn 606
    // 5.3 2 bits progressive shift disable spn 607
    // 6-7 input shaft speed spn 160
    // 8 source address of controlling device for transmission control spn 1482
    updateMessage(&CumminsBus::pgn61442, msg);
  } else if (message.pgn == 61452) {
    // no idea what this one is, and neither does AI lol
  } else if (message.pgn == 65098) {
    // information about transmission modes, not sure we need this?
  } else if (message.pgn == 59640) {
  } else if (message.pgn == 61445) {
    updateMessage(&pgn61445, msg);
    // All about our gears, we want this data!
    // uint8_t selectedGear = message.data[0] - 125;  // spn 524
    // float actualGearRatio =
    //     ((message.data[2] << 8) | message.data[1]) * 0.001;  // spn 526
    // uint8_t currentGear = message.data[3] - 125;             // spn 523
    // uint8_t requestedRange1 = message.data[4];
    // uint8_t requestedRange2 = message.data[5];  // spn 162
    // uint8_t currentRange1 = message.data[6];    // spn 163
    // uint8_t currentRange2 = message.data[7];    // spn 163
    // Serial.println("Selected Gear: " + (String)selectedGear +
    //                " Actual Gear Ratio: " + (String)actualGearRatio +
    //                " Current Gear: " + (String)currentGear +
    //                " Requested Range: " + (char)requestedRange1 + " " +
    //                (char)requestedRange2 + " Current Range: " +
    //                (char)currentRange1 + " " + (char)currentRange2);
  } else if (message.pgn == 5) {
  } else if (message.pgn == 65099) {
    // Transmission configuration 2
    // Transmit torque limit, so we can ignore
  } else if (message.pgn == 60159) {
    // No idea what this is
  } else if (message.pgn == DM1_DTCS_PGN) {
    // This is a DTC message
    uint8_t mil = SeaDash::Bits::getNBits(message.data[0], 6, 2);
    uint8_t rsl = SeaDash::Bits::getNBits(message.data[0], 4, 2);
    uint8_t awl = SeaDash::Bits::getNBits(message.data[0], 2, 2);
    uint8_t pls = SeaDash::Bits::getNBits(message.data[0], 0, 2);
    uint8_t milBlink = SeaDash::Bits::getNBits(message.data[1], 6, 2);
    uint8_t rslBlink = SeaDash::Bits::getNBits(message.data[1], 4, 2);
    uint8_t awlBlink = SeaDash::Bits::getNBits(message.data[1], 2, 2);
    uint8_t plsBlink = SeaDash::Bits::getNBits(message.data[1], 0, 2);
    uint32_t spn = 0;
    spn = SeaDash::Bits::setNBitsAt<uint32_t>(spn, message.data[2], 11, 8);
    spn = SeaDash::Bits::setNBitsAt<uint32_t>(spn, message.data[3], 7, 0);
    uint8_t spnLeastSignificantBits =
        SeaDash::Bits::getNBits<uint8_t>(message.data[4], 5, 3);
    spn =
        SeaDash::Bits::setNBitsAt<uint32_t>(spn, spnLeastSignificantBits, 0, 3);
    uint8_t fmi = SeaDash::Bits::getNBits(message.data[4], 0, 5);
    uint8_t oc = SeaDash::Bits::getNBits(message.data[5], 0, 7);
    Serial.println(
        "DM1 DTC: SPN: " + (String)spn +
        " Failure Mode Indicator: " + (String)fmi +
        " Occurence Count: " + (String)oc + " " + " MIL " + (String)mil + " " +
        " RSL " + (String)rsl + " " + " AWL " + (String)awl + " " + " PLS " +
        (String)pls + " MIL Blink " + (String)milBlink + " " + " RSL Blink" +
        (String)rslBlink + " " + " AWL Blink" + (String)awlBlink + " " +
        " PLS Blink" + (String)plsBlink + " Data: " + (String)message.data[0] +
        " " + (String)message.data[1] + " " + (String)message.data[2] + " " +
        (String)message.data[3] + " " + (String)message.data[4] + " " +
        (String)message.data[5] + " " + (String)message.data[6] + " " +
        (String)message.data[7]);
  } else {
    Serial.println("PGN: " + (String)message.pgn +
                   " From source address: " + (String)message.sourceAddress +
                   (String)message.data[0] + " " + (String)message.data[1] +
                   " " + (String)message.data[2] + " " +
                   (String)message.data[3] + " " + (String)message.data[4] +
                   " " + (String)message.data[5] + " " +
                   (String)message.data[6] + " " + (String)message.data[7]);
    ids[idsP++] = msg.id;
    if (idsP >= 8) idsP = 0;
  }
}  // ISR done

float mapf(float x, float in_min, float in_max, float out_min, float out_max) {
  if (in_min == in_max) {
    // Handle division by zero error or invalid input range
    // You can choose to return a default value or throw an exception
    return 0.0f;  // Default value
  }

  // Compute the scaling factor
  float scale = (out_max - out_min) / (in_max - in_min);

  // Map the value to the output range
  return (x - in_min) * scale + out_min;
}

void updateMaxTiming() {
  // 1200 => 16
  // 2000 => 20
  // 2500 => 25
  // 3000+ => 30
  if (RPM < 1200) {
    maxTiming = Timing;
  } else if (RPM > 3000) {
    maxTiming = 30.0f;
  }

  maxTiming = mapf((float)RPM, 1200.0f, 3000.0f, 16.0f, 30.0f);
}

void CumminsBus::updateTiming(CAN_message_t& msg) {
  // check coolant temp > 150
  if (!warmedUp) {
    // Serial.println("Engine not warmed up yet, not adjusting timing " +
    // (String)waterTemp);
    return;
  }

  // get max timing for rpm
  CumminsBus::updateRpms();
  CumminsBus::updateTiming();
  updateMaxTiming();

  // Serial.println("current timing -> " + (String)currentTiming + " max timing
  // " + (String)maxTiming);

  // get max of throttle and load
  CumminsBus::updateThrottlePercentage();
  CumminsBus::updateLoad();
  maxOfThrottleAndLoad = throttlePercentage > load ? throttlePercentage : load;
  // map between current timing and max timing based on max of throttle and load
  newTiming =
      mapf((float)maxOfThrottleAndLoad, 0.0f, 100.0f, Timing, maxTiming);

  // Serial.println(
  //     "current timing -> " + (String)Timing
  //     + " max timing " + (String)maxTiming
  //     + " calculated timing " + (String)newTiming
  //     + " Timing Advanced " + (String)(newTiming - Timing)
  // );

  // Serial.println("Fuel % " + (String)FuelPercentage
  //     + " Throttle % " + (String)throttlePercentage
  //     + " Load % " + (String)load
  // );

  newTiming *=
      128.0;  // Multiply by 128 to convert timing to 128 bits per degree
  shortTimingValue = static_cast<unsigned short>(
      newTiming);  // Convert timing to unsigned short

  msg.buf[4] = shortTimingValue & 0xFF;
  msg.buf[5] = (shortTimingValue >> 8) & 0xFF;

  // send new timing message
  Can1.write(msg);
}

void CumminsBus::updateTiming() {
  // compute timing advance
  Timing =
      (message256.data[5] << 8) |
      message256.data[4];  // convert from little endian. 128 bits per degree.
  Timing = (float)((Timing) / 128.0f);
}

float CumminsBus::getCurrentTiming() {
  CumminsBus::updateTiming();
  return Timing;
}

byte CumminsBus::getTransmissionTempC() {
  std::uint16_t tempRaw = (pgn65272.data[5] << 8) | pgn65272.data[4];
  float transmissionTemp = static_cast<float>(tempRaw) * 0.03125f - 273.15f;
  return static_cast<byte>(transmissionTemp);
}

float CumminsBus::getCurrentFuelPercentage() {
  // Fuel compute
  FuelPercentage = ((message256.data[1] << 8) | message256.data[0]);
  FuelPercentage = (float)(FuelPercentage * 100.0f) /
                   4096.0f;  // 4095 is max fuel allowed by pump
  return FuelPercentage;
}

void CumminsBus::updateThrottlePercentage() {
  throttlePercentage = pgn61443.data[1] * .4f;  // looking good!
}

int CumminsBus::getCurrentThrottlePercentage() {
  CumminsBus::updateThrottlePercentage();
  return throttlePercentage;
}

void CumminsBus::updateLoad() {
  load = static_cast<float>(pgn61443.data[2]) * 0.8f;  // looking good!
}

int CumminsBus::getCurrentLoad() {
  CumminsBus::updateLoad();
  return load;
}

void CumminsBus::updateRpms() {
  RPM = (message256.data[7] << 8) |
        message256.data[6];  // convert from little endian
  RPM /= 4;
}

int CumminsBus::getCurrentRpms() {
  CumminsBus::updateRpms();
  return (int)RPM;
}

int CumminsBus::getCurrentAMT() { return pgn65270.data[2] - 40; }

int CumminsBus::getCurrentBoostInPsi() {
  double kpa = pgn65270.data[1] * 2;
  double psi = kpa / 6.895 - 10;
  return psi;
}

// Parse "engine coolant temperature" parameter from J1939 message
bool parseEngineCoolantTemp(const J1939Message& msg, float& coolantTemp) {
  // Check if the message is an "engine coolant temperature" message
  if ((msg.id >> 18) != 0) {
    return false;
  }
  std::uint8_t pf = (msg.id >> 8) & 0xFF;  // PDU format
  std::uint8_t ps = msg.id & 0xFF;         // PDU specific
  // Check if the message has the PGN 65262 (hexadecimal 0xFEEE)
  if (pf != 240 || ps != 14) {
    return false;
  }
  // Extract "engine coolant temperature" parameter from data
  std::int16_t tempRaw = (msg.data[1] << 8) | msg.data[0];
  coolantTemp = (float)tempRaw * 0.03125f - 273.15f;
  return true;
}

int CumminsBus::getCurrentWaterTemp() {
  waterTemp = pgn65262.data[0] - 40;  // Confirmed!!!
  if (!warmedUp && waterTemp > 65) {
    warmedUp = true;
  }
  return static_cast<int>(waterTemp);
}

// Parse "current engine oil pressure" parameter from J1939 message and convert
// to PSI
bool parseCurrentEngineOilPressure(const J1939Message& msg,
                                   float& oilPressurePsi) {
  // Check if the message is an "engine oil pressure" message
  if ((msg.id >> 18) != 0) {
    return false;
  }
  std::uint8_t pf = (msg.id >> 8) & 0xFF;  // PDU format
  std::uint8_t ps = msg.id & 0xFF;         // PDU specific
  // Check if the message has the PGN 65265 (hexadecimal 0xFEF1)
  if (pf != 240 || ps != 17) {
    return false;
  }
  // Extract "current engine oil pressure" parameter from data
  std::uint16_t pressureRaw = (msg.data[1] << 8) | msg.data[0];
  float oilPressureKpa = (float)(pressureRaw - 10000) / 1000.0f;
  oilPressurePsi = oilPressureKpa * 0.145038f;
  return true;
}

byte CumminsBus::getCurrentOilPressure() {
  // Compute Oil Pressure
  oilPressure = pgn65263.data[3] * 4 / 6.895;  // Confirmed!!!
  return (byte)oilPressure;
}

int CumminsBus::getCurrentFuelTemp() {
  // Compute Fuel Temperature
  fuelTemp = (message274.data[7] << 8) | message274.data[6];  // Raw
  fuelTemp = fuelTemp / 16;                                   // Kelvin
  fuelTemp = fuelTemp - 273.15;                               // Celsius
  fuelTemp = ((fuelTemp * 9) / 5) + 32;                       // Fahrenheit

  return fuelTemp;
}

char CumminsBus::getRequestedRange() {
  return static_cast<char>(pgn61445.data[4]);
}

int8_t CumminsBus::getCurrentGear() {
  return static_cast<int8_t>(pgn61445.data[3] - 125);
}

int8_t CumminsBus::getSelectedGear() {
  return static_cast<int8_t>(pgn61445.data[0] - 125);
}

void CumminsBus::initialize() {
  Serial.println("Cummins Bus initializing");

  Can1.begin();
  Can1.setBaudRate(250 * 1000);
  Can1.setMaxMB(16);
  Can1.enableFIFO();
  Can1.enableFIFOInterrupt();
  Can1.onReceive(CumminsBusSniff);
  Can1.mailboxStatus();

  // pgn to request water temp pgn :)
  requestPgn(ENGINE_TEMP_1_PGN);
  requestPgn(DM1_DTCS_PGN);
}

void CumminsBus::loop() {  // request PGN's every 1 second
  uint32_t currentMillis = millis();
  if (currentMillis - lastJ1939Request >= 1000) {
    requestPgn(ENGINE_TEMP_1_PGN);
    requestPgn(DM1_DTCS_PGN);
    requestPgn(DM2_DTCS_PGN);
    lastJ1939Request = currentMillis;

    Serial.print("Seen SA's [");

    for (auto it = sourceAddresses.begin(); it != sourceAddresses.end(); ++it) {
      if (it != sourceAddresses.begin()) {
        Serial.print(",");  // Add a comma before each element except the first
      }
      Serial.print(*it);
    }

    Serial.println("]");  // Close the bracket and move to the next line
  }
}

#endif
#endif  // J1939_CPP
