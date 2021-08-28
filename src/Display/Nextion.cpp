//
// Created by jlaustill on 8/27/21.
//
#include "Configuration.h"

#ifdef NEXTION

#include "Nextion.h"

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

void Nextion::updateDisplayData(AppData currentData) {
    double speedDegrees = (double)currentData.speedInMph * 170 / 90;
    sendCmd("speedometer.val=" + (String)(int)speedDegrees);

    double waterTempDegrees = (((double)currentData.coolantTemp * 9 / 5 + 32)  * 50) / 250;
    sendCmd("watertemp.val=" + (String)(int)waterTempDegrees);

    double rpmDegrees = ((double)currentData.rpm * 33) / 4000;
    Serial.println("rpmDegrees: " + (String)rpmDegrees);
    sendCmd("tachometer.val=" + (String)(int)rpmDegrees);

    double transmissionTemperateDegrees = 360 - (((double)currentData.transmissionTempC * 9 / 5) + 32) * 45 / 250;
//    sendCmd("transtemp.val=" + (String)(int)transmissionTemperateDegrees);
    sendCmd("transtemp.val=0");

    double oilPressureDegrees = 360 - (double)currentData.oilPressureInPsi * 40 / 100;
    sendCmd("oilpres.val=" + (String)(int)oilPressureDegrees);
}

#endif