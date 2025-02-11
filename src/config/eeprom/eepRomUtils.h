#ifndef EEPROM_UTILS_H
#define EEPROM_UTILS_H

#include <Arduino.h>

namespace EEPROMUtils {
    void writeStringToFlash(const char* toStore, int startAddr);
    String readStringFromFlash(int startAddr);
}

#endif