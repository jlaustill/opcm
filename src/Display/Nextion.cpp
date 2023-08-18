//
// Created by jlaustill on 8/27/21.
//
#include "Configuration.h"

#ifdef NEXTION

#include <Arduino.h>
#include <cctype>
#include "Nextion.h"
#include <EEPROM.h>
#include "Data/memory.h"

void Nextion::sendCmd(String cmd)
{
    nexSer.print(cmd);
    nexSer.write("\xFF\xFF\xFF");
#ifdef DEBUG
    if (cmd.length() > 0)
    {
        Serial.print("Sending command : ");
        Serial.println(cmd);
    }
    else
    {
        Serial.println("Empty command issued to clear the buffer");
    }
#endif
}

void Nextion::initialize()
{
    nexSer.begin(115200);
    while (!nexSer)
    {
        // wait for connect
    }
    sendCmd(""); // clear the buffer
}

String formatNumber(double number)
{
    String finalText = "";
    char buffer[20]; // Adjust the buffer size according to your needs

    // Convert the input number to a formatted string
    snprintf(buffer, sizeof(buffer), "%.2f", number);

    int numDigits = 0;
    size_t periodIndex = strcspn(buffer, "."); // Find the position of the decimal point

    for (size_t i = 0; i < periodIndex; i++)
    {
        if (isdigit(buffer[i]))
        {
            numDigits++;
        }
    }

    // Loop through the input string and copy each number to the output string,
    // inserting commas along the way
    for (size_t i = 0; i < periodIndex; i++)
    {
        finalText += buffer[i];

        // Decrease the distance from the decimal point
        size_t distance = numDigits - i - 1;

        // Insert a comma every three decimal positions away from the decimal point
        if ((distance > 0) && (distance % 3 == 0))
        {
            finalText += ',';
        }
    }

    finalText += '.';
    finalText += buffer[periodIndex + 1];

    return finalText;
}

void Nextion::updateDisplayData(AppData *currentData)
{
    sendCmd("mph.val=" + (String)currentData->speedInMph);
    sendCmd("odometer.txt=\"" + formatNumber(currentData->odometer) + "\"");
    sendCmd("tripA.txt=\"" + formatNumber(currentData->tripA) + "\"");
    sendCmd("tripB.txt=\"" + formatNumber(currentData->tripB) + "\"");
    sendCmd("oc.txt=\"" + formatNumber(currentData->oilChange) + "\"");
    sendCmd("tfc.txt=\"" + formatNumber(currentData->transmissionFluidChange) + "\"");
    sendCmd("tcfc.txt=\"" + formatNumber(currentData->transferCaseFluidChange) + "\"");
    sendCmd("fdfc.txt=\"" + formatNumber(currentData->frontDifferentialFluidChange) + "\"");
    sendCmd("rdfc.txt=\"" + formatNumber(currentData->rearDifferentialFluidChange) + "\"");
    sendCmd("ffc.txt=\"" + formatNumber(currentData->fuelFilterChange) + "\"");
    sendCmd("tr.txt=\"" + formatNumber(currentData->tireRotation) + "\"");

    sendCmd("egt.val=" + (String)(int)currentData->egt);

    // double transPressureDegrees = 360.0 - (double)currentData->transmissionPressure * 30 / 400;
    sendCmd("transPres.val=" + (String)(int)currentData->transmissionPressure);
    // sendCmd("transprestxt.txt=\"" + (String)currentData->transmissionPressure + " PSI\"");
    // Serial.println("trans pres? " + (String)currentData->transmissionPressure);

    double coolTempF = ((double)currentData->coolantTemp * 9 / 5) + 32;
    sendCmd("h20t.val=" + (String)(int)coolTempF);

    double coolTemp2F = ((double)currentData->coolantTemp2 * 9 / 5) + 32;
    sendCmd("h20t2.val=" + (String)(int)coolTemp2F);

    double oilTempF = ((double)currentData->oilTempC * 9 / 5) + 32;
    sendCmd("ot.val=" + (String)(int)oilTempF);

    sendCmd("fueltmp.val=" + (String)currentData->fuelTempF);

    sendCmd("rpm.val=" + (String)currentData->rpm);

    double transmissionTemperateDegrees = (((double)currentData->transmissionTempC * 9 / 5) + 32);
    sendCmd("trantemp.val=" + (String)(int)transmissionTemperateDegrees);

    sendCmd("oilPres.val=" + (String)(int)currentData->oilPressureInPsi);

    sendCmd("load.val=" + (String)(int)currentData->load);
    sendCmd("throttle.val=" + (String)(int)currentData->throttlePercentage);

    String serialBuffer;
    while (nexSer.available())
    {
        int newData = nexSer.read();
        if (newData == ';')
        {
            Serial.println("Execute!" + serialBuffer);
            if (serialBuffer.indexOf("resetTripA") > 0)
            {
                Serial.println("reset trip A!");
                double zero = 0;
                memory::setTripA(zero);
                currentData->tripA = zero;
            }
            if (serialBuffer.indexOf("resetTripB") > 0)
            {
                Serial.println("reset trip B!");
                double zero = 0;
                memory::setTripB(zero);
                currentData->tripB = zero;
            }
            if (serialBuffer.indexOf("resetOC") > 0)
            {
                Serial.println("reset Oil Change Mileage!");
                double zero = 0;
                memory::setOilChange(zero);
                currentData->oilChange = zero;
            }
            if (serialBuffer.indexOf("resetTFC") > 0)
            {
                Serial.println("reset Transmission Fluid Change Mileage!");
                double zero = 0;
                memory::setTransmissionFluidChange(zero);
                currentData->transmissionFluidChange = zero;
            }
            if (serialBuffer.indexOf("resetTCFC") > 0)
            {
                Serial.println("reset Transfer Case Fluid Change Mileage!");
                double zero = 0;
                memory::setTransferCaseFluidChange(zero);
                currentData->transferCaseFluidChange = zero;
            }
            if (serialBuffer.indexOf("resetFDFC") > 0)
            {
                Serial.println("reset Front Differential Fluid Change Mileage!");
                double zero = 0;
                memory::setFrontDifferentialFluidChange(zero);
                currentData->frontDifferentialFluidChange = zero;
            }
            if (serialBuffer.indexOf("resetRDFC") > 0)
            {
                Serial.println("reset Rear Differential Fluid Change Mileage!");
                double zero = 0;
                memory::setRearDifferentialFluidChange(zero);
                currentData->rearDifferentialFluidChange = zero;
            }
            if (serialBuffer.indexOf("resetFFC") > 0)
            {
                Serial.println("reset Fuel Filter Change Mileage!");
                double zero = 0;
                memory::setFuelFilterChange(zero);
                currentData->fuelFilterChange = zero;
            }
            if (serialBuffer.indexOf("resetTR") > 0)
            {
                Serial.println("reset Tire Rotation Mileage!");
                double zero = 0;
                memory::setTireRotation(zero);
                currentData->tireRotation = zero;
            }
            serialBuffer = "";
        }
        else
        {
            serialBuffer += (char)newData;
            // Serial.println(serialBuffer);
        }
    }
}

#endif