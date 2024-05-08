//
// Created by jlaustill on 8/21/21.
//
#include "Configuration.h"
#ifdef CUMMINS_BUS_INPUT
#include <Arduino.h>
#include <FlexCAN_T4.h>
FlexCAN_T4<CAN1, RX_SIZE_256, TX_SIZE_16> Can1;
#include <J1939.h>
#include <bit-utils.h>

#include <cstdint>

#include "CumminsBus.h"

J1939 message;

// volatile byte data[16];
boolean warmedUp = false;

struct CanMessage {
  byte id;
  byte length;
  byte data[8];
  unsigned count;
};

// J1939 message structure
struct J1939Message {
  std::uint32_t id;
  std::uint8_t data[8];
};

uint32_t calculateJ1939PGN(uint8_t* canMsg) {
  // Extract PDU format (PF) and PDU specific (PS) bytes
  uint8_t pf = canMsg[0] >> 4;
  uint8_t ps = canMsg[1];

  // Calculate PGN
  uint32_t pgn = (pf << 16) | (ps << 8);

  // If PF is greater than or equal to 240, then PS contains the data page
  // number
  if (pf >= 0xF0) {
    uint8_t dp = canMsg[2];
    pgn |= (dp << 16);
  }

  return pgn;
}

volatile CanMessage pgn65262{
    0x67, 0x8, {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}, 0};
volatile CanMessage pgn65263{};
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

uint8_t calculateJ1939Source(uint8_t* canMsg) {
  // Extract source address (SA) from the message (bits 8-15 of byte 0)
  uint8_t sa = canMsg[0] & 0xFF;

  // If the message is an RTR message, add 0x80 to the SA to indicate that it's
  // a request message
  if ((canMsg[0] & 0x10) == 0x10) {
    sa += 0x80;
  }

  return sa;
}

uint8_t calculateJ1939Destination(uint8_t* canMsg) {
  // Extract destination address (DA) from the message (bits 0-7 of byte 0)
  uint8_t da = canMsg[0] >> 3;

  // If the message is an RTR message, set DA to the global broadcast address
  // (0xFF)
  if ((canMsg[0] & 0x10) == 0x10) {
    da = 0xFF;
  }

  return da;
}

void CumminsBusSniff(const CAN_message_t& msg) {
  // for (uint8_t i = 0; i < msg.len; i++) {
  //   data[i] = msg.buf[i];
  // }

  message = J1939();
  message.setCanId(msg.id);
  message.setData(msg.buf);

  // Serial.println("ID: " + (String)msg.id + " Data: " + (String)data[0] + " "
  // +
  //                (String)data[1] + " " + (String)data[2] + " " +
  //                (String)data[3] + " " + (String)data[4] + " " +
  //                (String)data[5] + " " + (String)data[6] + " " +
  //                (String)data[7] + " " + (String)msg.len + " " +
  //                (String)msg.flags.extended + " " +
  //                (String)msg.flags.remote);
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
  } else if (message.pgn == 65262) {
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
  } else if (message.pgn == 65265) {
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
  } else {
    Serial.println("PGN: " + (String)message.pgn);
    // byte* idBytes = getIdBytes(msg.id);
    // int pgn = calculateJ1939PGN(idBytes);
    // uint8_t sourceAddress = calculateJ1939Source(idBytes);
    // uint8_t destinationAddress = calculateJ1939Destination(idBytes);
    // Serial.println("unknow PGN: " + (String)pgn
    //     + " Source Address: " + (String)sourceAddress
    //     + " Destination Address: " + (String)destinationAddress
    // );
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

float parseEngineLoad(uint8_t* canMsg) {
  // Check that the PGN is 0xFEF1 (Engine Fluids - 1) and the SPN is 92 (Engine
  // Load)
  uint32_t pgn = calculateJ1939PGN(canMsg);
  uint16_t spn = (canMsg[2] << 8) | canMsg[3];
  if (pgn != 0xFEF1 || spn != 92) {
    return -1.0f;  // Invalid message
  }

  // Extract engine load value (range: 0-100%)
  uint8_t value = canMsg[5];
  float load = static_cast<float>(value) * 0.4f;

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
  // Serial.println("requesting water temp?" + (String)Can2.getTXQueueCount());
  // Can2.events();
  Can1.write(msg);
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

void CumminsBus::initialize() {
  Serial.println("Cummins Bus initializing");

  Can1.begin();
  Can1.setBaudRate(250000);
  Can1.setMaxMB(16);
  Can1.enableFIFO();
  Can1.enableFIFOInterrupt();
  Can1.onReceive(CumminsBusSniff);
  Can1.mailboxStatus();

  // pgn to request water temp pgn :)
  msg.flags.extended = 1;
  msg.id = 2364145912;
  msg.len = 3;
  msg.buf[0] = 238;
  msg.buf[1] = 254;
  msg.buf[2] = 0;
}

#endif
