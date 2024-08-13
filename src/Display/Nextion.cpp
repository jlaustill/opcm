//
// Created by jlaustill on 8/27/21.
//
#include "Configuration.h"

#ifdef NEXTION

#include <Arduino.h>
#include <EEPROM.h>

#include <cctype>

#include "Nextion.h"

// Track last sent data
AppData lastSentData;

// Track update timings
unsigned long last100msUpdate = 0;
unsigned long last1sUpdate = 0;

// Buffer for batching commands
String batchCmdBuffer = "";

void Nextion::sendCmd(String cmd) { batchCmdBuffer += cmd + "\xFF\xFF\xFF"; }

void Nextion::sendBatch() {
  if (batchCmdBuffer.length() > 0) {
    nexSer.print(batchCmdBuffer);
    batchCmdBuffer = "";
#ifdef DEBUG
    Serial.print("Sending batch command: ");
    Serial.println(batchCmdBuffer);
#endif
  }
}

void Nextion::initialize() {
  nexSer.begin(115200);
  while (!nexSer) {
    // wait for connect
  }
  sendCmd("");  // clear the buffer
}

String formatNumber(double number) {
  String finalText = "";
  char buffer[20];  // Adjust the buffer size according to your needs

  // Convert the input number to a formatted string
  snprintf(buffer, sizeof(buffer), "%.2f", number);

  int numDigits = 0;
  size_t periodIndex =
      strcspn(buffer, ".");  // Find the position of the decimal point

  for (size_t i = 0; i < periodIndex; i++) {
    if (isdigit(buffer[i])) {
      numDigits++;
    }
  }

  // Loop through the input string and copy each number to the output string,
  // inserting commas along the way
  for (size_t i = 0; i < periodIndex; i++) {
    finalText += buffer[i];

    // Decrease the distance from the decimal point
    size_t distance = numDigits - i - 1;

    // Insert a comma every three decimal positions away from the decimal point
    if ((distance > 0) && (distance % 3 == 0)) {
      finalText += ',';
    }
  }

  finalText += '.';
  finalText += buffer[periodIndex + 1];

  return finalText;
}

void Nextion::updateDisplayData(AppData *currentData) {
  unsigned long currentMillis = millis();

  // Update the display every 100ms
  if (currentMillis - last100msUpdate >= 100) {
    last100msUpdate = currentMillis;

    // Check if the data has changed
    if (memcmp(currentData, &lastSentData, sizeof(AppData)) != 0) {
      // Send the updated data to the display
      if (currentData->speedInMph != lastSentData.speedInMph) {
        sendCmd("mph.val=" + (String)currentData->speedInMph);
        lastSentData.speedInMph = currentData->speedInMph;
      }
      if (currentData->rpm != lastSentData.rpm) {
        sendCmd("rpm.val=" + (String)currentData->rpm);
        lastSentData.rpm = currentData->rpm;
      }
      if (currentData->egt != lastSentData.egt) {
        sendCmd("egt.val=" + (String)(int)currentData->egt);
        lastSentData.egt = currentData->egt;
      }
      if (currentData->throttlePercentage != lastSentData.throttlePercentage) {
        sendCmd("throttle.val=" + (String)(int)currentData->throttlePercentage);
        lastSentData.throttlePercentage = currentData->throttlePercentage;
      }
      if (currentData->selectedGear != lastSentData.selectedGear) {
        sendCmd("selGear.val=" + (String)currentData->selectedGear);
        lastSentData.selectedGear = currentData->selectedGear;
      }
      if (currentData->currentGear != lastSentData.currentGear) {
        sendCmd("curGear.val=" + (String)currentData->currentGear);
        lastSentData.currentGear = currentData->currentGear;
      }
      if (currentData->load != lastSentData.load) {
        sendCmd("load.val=" + (String)(int)currentData->load);
        lastSentData.load = currentData->load;
      }
    }
    last100msUpdate = currentMillis;
  }

  // Update slow data every 1 second
  if (currentMillis - last1sUpdate >= 1000) {
    last1sUpdate = currentMillis;
    if (currentData->requestedRange != lastSentData.requestedRange) {
      sendCmd("reqRange.txt=\"" + (String)currentData->requestedRange + "\"");
      lastSentData.requestedRange = currentData->requestedRange;
    }
    if (currentData->odometer != lastSentData.odometer) {
      sendCmd("odometer.txt=\"" + formatNumber(currentData->odometer) + "\"");
      lastSentData.odometer = currentData->odometer;
    }
    if (currentData->tripA != lastSentData.tripA) {
      sendCmd("tripA.txt=\"" + formatNumber(currentData->tripA) + "\"");
      lastSentData.tripA = currentData->tripA;
    }
    if (currentData->tripB != lastSentData.tripB) {
      sendCmd("tripB.txt=\"" + formatNumber(currentData->tripB) + "\"");
      lastSentData.tripB = currentData->tripB;
    }
    if (currentData->oilChange != lastSentData.oilChange) {
      sendCmd("oc.txt=\"" + formatNumber(currentData->oilChange) + "\"");
      lastSentData.oilChange = currentData->oilChange;
    }
    if (currentData->transmissionFluidChange !=
        lastSentData.transmissionFluidChange) {
      sendCmd("tfc.txt=\"" +
              formatNumber(currentData->transmissionFluidChange) + "\"");
      lastSentData.transmissionFluidChange =
          currentData->transmissionFluidChange;
    }
    if (currentData->transferCaseFluidChange !=
        lastSentData.transferCaseFluidChange) {
      sendCmd("tcfc.txt=\"" +
              formatNumber(currentData->transferCaseFluidChange) + "\"");
      lastSentData.transferCaseFluidChange =
          currentData->transferCaseFluidChange;
    }
    if (currentData->frontDifferentialFluidChange !=
        lastSentData.frontDifferentialFluidChange) {
      sendCmd("fdfc.txt=\"" +
              formatNumber(currentData->frontDifferentialFluidChange) + "\"");
      lastSentData.frontDifferentialFluidChange =
          currentData->frontDifferentialFluidChange;
    }
    if (currentData->rearDifferentialFluidChange !=
        lastSentData.rearDifferentialFluidChange) {
      sendCmd("rdfc.txt=\"" +
              formatNumber(currentData->rearDifferentialFluidChange) + "\"");
      lastSentData.rearDifferentialFluidChange =
          currentData->rearDifferentialFluidChange;
    }
    if (currentData->fuelFilterChange != lastSentData.fuelFilterChange) {
      sendCmd("ffc.txt=\"" + formatNumber(currentData->fuelFilterChange) +
              "\"");
      lastSentData.fuelFilterChange = currentData->fuelFilterChange;
    }
    if (currentData->tireRotation != lastSentData.tireRotation) {
      sendCmd("tr.txt=\"" + formatNumber(currentData->tireRotation) + "\"");
      lastSentData.tireRotation = currentData->tireRotation;
    }
    if (currentData->transmissionPressure) {
      sendCmd("transPres.val=" +
              (String)(int)currentData->transmissionPressure);
      lastSentData.transmissionPressure = currentData->transmissionPressure;
    }
    if (currentData->coolantTemp != lastSentData.coolantTemp) {
      double coolTempF = ((double)currentData->coolantTemp * 9 / 5) + 32;
      sendCmd("h20t.val=" + (String)(int)coolTempF);
      lastSentData.coolantTemp = currentData->coolantTemp;
    }
    if (currentData->coolantTemp2 != lastSentData.coolantTemp2) {
      double coolTemp2F = ((double)currentData->coolantTemp2 * 9 / 5) + 32;
      sendCmd("h20t2.val=" + (String)(int)coolTemp2F);
      lastSentData.coolantTemp2 = currentData->coolantTemp2;
    }
    if (currentData->oilTempC != lastSentData.oilTempC) {
      double oilTempF = ((double)currentData->oilTempC * 9 / 5) + 32;
      sendCmd("ot.val=" + (String)(int)oilTempF);
      lastSentData.oilTempC = currentData->oilTempC;
    }
    if (currentData->fuelTempF != lastSentData.fuelTempF) {
      sendCmd("fueltmp.val=" + (String)currentData->fuelTempF);
      lastSentData.fuelTempF = currentData->fuelTempF;
    }

    if (currentData->transmissionTempC != lastSentData.transmissionTempC) {
      double transmissionTemperateDegrees =
          (((double)currentData->transmissionTempC * 9 / 5) + 32);
      sendCmd("trantemp.val=" + (String)(int)transmissionTemperateDegrees);
      lastSentData.transmissionTempC = currentData->transmissionTempC;
    }
    if (currentData->oilPressureInPsi != lastSentData.oilPressureInPsi) {
      sendCmd("oilPres.val=" + (String)(int)currentData->oilPressureInPsi);
      lastSentData.oilPressureInPsi = currentData->oilPressureInPsi;
    }
  }

  // Send batch commands
  sendBatch();

  // Process incoming commands
  processCommands(currentData);
}

void Nextion::processCommands(AppData *currentData) {
  static String serialBuffer;
  while (nexSer.available()) {
    int newData = nexSer.read();
    if (newData == ';') {
      Serial.println("Execute!" + serialBuffer);
      if (serialBuffer.indexOf("resetTripA") > 0) {
        Serial.println("reset trip A!");
        currentData->tripA = 0;
      }
      if (serialBuffer.indexOf("resetTripB") > 0) {
        Serial.println("reset trip B!");
        currentData->tripB = 0;
      }
      if (serialBuffer.indexOf("resetOC") > 0) {
        Serial.println("reset Oil Change Mileage!");
        currentData->oilChange = 0;
      }
      if (serialBuffer.indexOf("resetTFC") > 0) {
        Serial.println("reset Transmission Fluid Change Mileage!");
        currentData->transmissionFluidChange = 0;
      }
      if (serialBuffer.indexOf("resetTCFC") > 0) {
        Serial.println("reset Transfer Case Fluid Change Mileage!");
        currentData->transferCaseFluidChange = 0;
      }
      if (serialBuffer.indexOf("resetFDFC") > 0) {
        Serial.println("reset Front Differential Fluid Change Mileage!");
        currentData->frontDifferentialFluidChange = 0;
      }
      if (serialBuffer.indexOf("resetRDFC") > 0) {
        Serial.println("reset Rear Differential Fluid Change Mileage!");
        currentData->rearDifferentialFluidChange = 0;
      }
      if (serialBuffer.indexOf("resetFFC") > 0) {
        Serial.println("reset Fuel Filter Change Mileage!");
        currentData->fuelFilterChange = 0;
      }
      if (serialBuffer.indexOf("resetTR") > 0) {
        Serial.println("reset Tire Rotation Mileage!");
        currentData->tireRotation = 0;
      }
      serialBuffer = "";
    } else {
      serialBuffer += (char)newData;
    }
  }
}

#endif