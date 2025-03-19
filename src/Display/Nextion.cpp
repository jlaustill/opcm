//
// Created by jlaustill on 8/27/21.
//
#include "Configuration.h"

#ifdef NEXTION

#include <Arduino.h>

#include <cctype>

#include "Nextion.h"

// Track update timings
unsigned long last100msUpdate = 0;
unsigned long last1sUpdate = 0;

// Buffer for batching commands
String batchCmdBuffer = "";

void Nextion::sendCmd(String cmd) { batchCmdBuffer += cmd + "\xFF\xFF\xFF"; }

void Nextion::sendBatch() {
  if (batchCmdBuffer.length() > 0) {
#ifdef NEXTION_DEBUG
    Serial.print("Sending batch command: ");
    Serial.println(batchCmdBuffer);
#endif
    nexSer.print(batchCmdBuffer);
    batchCmdBuffer = "";
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

    sendCmd("mph.val=" + (String)currentData->speedInMph);
    sendCmd("rpm.val=" + (String)currentData->rpm);
    String bpt = "bp.val=" + (String)(int)currentData->boost;
    sendCmd(bpt);
    float boostTempF = (currentData->manifoldTempC * 9.0 / 5.0) + 32.0;
    String btt = "bt.val=" + (String)(int)boostTempF;
    sendCmd(btt);
    sendCmd("throttle.val=" + (String)(int)currentData->throttlePercentage);
    sendCmd("selGear.val=" + (String)currentData->selectedGear);
    sendCmd("curGear.val=" + (String)currentData->currentGear);
    sendCmd("reqRange.txt=\"" + (String)currentData->requestedRange + "\"");
    sendCmd("load.val=" + (String)(int)currentData->load);
    sendCmd("fuelPres.val=" + (String)(int)currentData->fuelPressure);
      
    sendBatch();
    last100msUpdate = currentMillis;
  }

  // Update slow data every 1 second
  if (currentMillis - last1sUpdate >= 1000) {
    sendCmd("odometer.txt=\"" + formatNumber(currentData->odometer) + "\"");
    sendCmd("tripA.txt=\"" + formatNumber(currentData->tripA) + "\"");
    sendCmd("tripB.txt=\"" + formatNumber(currentData->tripB) + "\"");
    sendCmd("oc.txt=\"" + formatNumber(currentData->oilChange) + "\"");
    sendCmd("tfc.txt=\"" +
            formatNumber(currentData->transmissionFluidChange) + "\"");
    sendCmd("tcfc.txt=\"" +
            formatNumber(currentData->transferCaseFluidChange) + "\"");
    sendCmd("fdfc.txt=\"" +
            formatNumber(currentData->frontDifferentialFluidChange) + "\"");
    sendCmd("rdfc.txt=\"" +
            formatNumber(currentData->rearDifferentialFluidChange) + "\"");
    sendCmd("ffc.txt=\"" + formatNumber(currentData->fuelFilterChange) +
            "\"");
    sendCmd("tr.txt=\"" + formatNumber(currentData->tireRotation) + "\"");
    sendCmd("transPres.val=" +
            (String)(int)currentData->transmissionPressure);
    double coolTempF = ((double)currentData->coolantTemp * 9 / 5) + 32;
    sendCmd("h20t.val=" + (String)(int)coolTempF);
    double coolTemp2F = ((double)currentData->coolantTemp2 * 9 / 5) + 32;
    sendCmd("h20t2.val=" + (String)(int)coolTemp2F);
    double oilTempF = ((double)currentData->oilTempC * 9 / 5) + 32;
    sendCmd("ot.val=" + (String)(int)oilTempF);
    sendCmd("fueltmp.val=" + (String)currentData->fuelTempF);
    double transmissionTemperateDegrees =
        (((double)currentData->transmissionTempC * 9 / 5) + 32);
    sendCmd("trantemp.val=" + (String)(int)transmissionTemperateDegrees);
    sendCmd("oilPres.val=" + (String)(int)currentData->oilPressureInPsi);

    // Send batch commands
    sendBatch();
    last1sUpdate = currentMillis;
  }

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