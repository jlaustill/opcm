//
// Created by jlaustill on 8/27/21.
//
#include "Configuration.h"

#ifdef NEXTION

#include <Arduino.h>
#include "Nextion.h"
#include <EEPROM.h>

void Nextion::sendCmd(String cmd) {
    nexSer.print(cmd);
    nexSer.write("\xFF\xFF\xFF");
#ifdef DEBUG
    if(cmd.length() > 0)
    {
      Serial.print("Sending command : ");
      Serial.println(cmd);
    } else
    {
      Serial.println("Empty command issued to clear the buffer");
    }
#endif
}

void Nextion::initialize() {
    nexSer.begin(115200);
    while(!nexSer)
    {
        // wait for connect
    }
    sendCmd(""); // clear the buffer
}

String formatNumber(double number) {
    String finalText = "";    /* String to contain formatted output text */
    char buffer[11];        /* Temporary string buffer to perform formatting operations */
    int index = 0;            /* Index counter for position in output text string */

    /* Convert the input number to a formatted string */
    sprintf(buffer, "%f", number);

    /* Calculate the number of digits before the decimal point */
    int periodIndex = 0;
    int numDigits = 0;
    int i, distance;

    for (i = 0; i < 11; i++) {
        if (buffer[i] == '.') {
            periodIndex = i;
            i = 99;
        }
    }

    for (i = 0; i < periodIndex; i++) {
        if (buffer[i] == '0'
            || buffer[i] == '1'
               || buffer[i] == '2'
                  || buffer[i] == '3'
                     || buffer[i] == '4'
                        || buffer[i] == '5'
                           || buffer[i] == '6'
                              || buffer[i] == '7'
                                 || buffer[i] == '8'
                                    || buffer[i] == '9'
            ) {
            numDigits++;
        }
    }
    /* Loop through the input string and copy each number to the
      outputText string, inserting commas along the way */
    for (i = 0; i < periodIndex; i++) {
        /* Copy character from the input string to the output string */
        finalText += buffer[i];
        index++;

        /* Decrease the distance from the decimal point */
        distance = numDigits - i - 1;

        /* Insert a comma every three decimal positions away from the decimal point */
        if ((distance > 0) && (distance % 3 == 0)) {
            finalText += ',';
            index++;
        }
    }
    finalText += '.';
    finalText += buffer[periodIndex + 1];

//    Serial.println("buffer: " + (String)buffer + " periodIndex: " + (String)periodIndex + " numDigits: " + (String)numDigits + " index: " + (String)index + " finalText: " + finalText);
    return finalText;
}

void Nextion::updateDisplayData(AppData* currentData) {
    sendCmd("mph.val=" + (String)currentData->speedInMph);
    sendCmd("odometer.txt=\"" + formatNumber(currentData->odometer) + "\"");
    sendCmd("tripA.txt=\"" + formatNumber(currentData->tripA) + "\"");
    sendCmd("tripB.txt=\"" + formatNumber(currentData->tripB) + "\"");
    // Serial.print("Odom: "); Serial.print(currentData->odometer); Serial.print(" tripA: "); Serial.print(currentData->tripA); Serial.print(" tripB: "); Serial.println(currentData->tripB);

    sendCmd("egt.val=" + (String)(int)currentData->egt);

    sendCmd("leftturn.aph=" + (String)(currentData->leftBlinker ? "127" : "0"));
    sendCmd("rightturn.aph=" + (String)(currentData->rightBlinker ? "127" : "0"));
    sendCmd("highbeam.aph=" + (String)(currentData->highBeams ? "127" : "0"));
    sendCmd("wts.aph=" + (String)(currentData->waitToStart ? "127" : "0"));
    sendCmd("fourx4.aph=" + (String)(currentData->fourByFour ? "127" : "0"));
    sendCmd("seatbelt.aph=" + (String)(currentData->seatBeltWarning ? "127" : "0"));
    sendCmd("doorajar.aph=" + (String)(currentData->doorAjarWarning ? "127" : "0"));
    sendCmd("parkbrake.aph=" + (String)(currentData->brakeLightWarning ? "127" : "0"));

    double transPressureDegrees = 360.0 - (double)currentData->transmissionPressure * 30 / 400;
    sendCmd("tranpresgauge.val=" + (String)(int)transPressureDegrees);
    sendCmd("transprestxt.txt=\"" + (String)currentData->transmissionPressure + " PSI\"");

    double coolTempF = ((double)currentData->coolantTemp * 9 / 5) + 32;
    sendCmd("h20t.val=" + (String)(int)coolTempF);

    sendCmd("fueltmp.val=" + (String)currentData->fuelTempF);

//    double rpmDegrees = ((double)currentData.rpm * 33) / 4000;
//    Serial.println("rpmDegrees: " + (String)rpmDegrees);
    sendCmd("rpm.val=" + (String)currentData->rpm);

    double transmissionTemperateDegrees = (((double)currentData->transmissionTempC * 9 / 5) + 32);
    sendCmd("trantemp.val=" + (String)(int)transmissionTemperateDegrees);

    sendCmd("oilPres.val=" + (String)(int)currentData->oilPressureInPsi);

    sendCmd("load.val=" + (String)(int)currentData->load);
    sendCmd("throttlePercentage.val=" + (String)(int)currentData->throttlePercentage);

    String serialBuffer;
    while (nexSer.available()) {
    int newData = nexSer.read();
    if (newData == ';') {
        Serial.println("Execute!" + serialBuffer);
        if (serialBuffer.indexOf("resetTripA") > 0) {
            Serial.println("reset trip A!");
            double zero = 0;
            EEPROM.put(8, zero);
            currentData->tripA = zero;
        }
        if (serialBuffer.indexOf("resetTripB") > 0) {
            Serial.println("reset trip B!");
            double zero = 0;
            EEPROM.put(16, zero);
            currentData->tripB = zero;
        }
        serialBuffer = "";
    } else {
        serialBuffer += (char)newData;
    }
    }
}



#endif