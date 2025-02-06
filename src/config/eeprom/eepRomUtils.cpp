#include "eepRomUtils.h"
#include <EEPROM.h>

#define EEPROM_SIZE 200

void EEPROMUtils::writeStringToFlash(const char* toStore, int startAddr) {
    int i = 0;
    for (; i < strlen(toStore) + 1; i++) {
        EEPROM.write(startAddr + i, toStore[i]);
    }
    EEPROM.write(startAddr + i, '\0');
    EEPROM.commit();
}

String EEPROMUtils::readStringFromFlash(int startAddr) {
    char in[128];
    int i = 0;
    for (; i < 128; i++) {
        in[i] = EEPROM.read(startAddr + i);
    }
    return String(in);
}
