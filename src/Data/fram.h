#ifndef OPCM_FRAM_H
#define OPCM_FRAM_H

#include "../Configuration.h"

#ifdef FRAM
#include <SPI.h>
#include "AppData.h"

#include "Adafruit_FRAM_SPI.h"

class OPCM_Fram {
 private:
  static bool isInitialized;
  static Adafruit_FRAM_SPI fram;

 public:
  static void initialize();
  static void saveData(AppData *currentData);
  static AppData loadData();
};

#endif
#endif  // OPCM_FRAM_H