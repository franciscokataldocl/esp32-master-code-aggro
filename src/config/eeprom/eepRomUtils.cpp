#include "eepRomUtils.h"
#include <EEPROM.h>

void EEPROMUtils::writeStringToFlash(const char* toStore, int startAddr) {
    int len = strlen(toStore);

    if (startAddr == 0 && len > 32) { // Verificar longitud máxima del SSID
        Serial.println("❌ Error: El SSID es demasiado largo");
        return;
    }
    if (startAddr == 40 && len > 64) { // Verificar longitud máxima de la contraseña
        Serial.println("❌ Error: La contraseña es demasiado larga");
        return;
    }

    // Escribir la cadena en EEPROM
    for (int i = 0; i < len; i++) {
        EEPROM.write(startAddr + i, toStore[i]);
    }
    EEPROM.write(startAddr + len, '\0'); // Agregar terminador nulo
    EEPROM.commit(); // Guardar cambios en EEPROM
    // Serial.println("credenciales almacenadas en memoria");
}


String EEPROMUtils::readStringFromFlash(int startAddr) {
    char in[128];
    int i = 0;

    // Read until we find a null character
    while (i < 128) {
        in[i] = EEPROM.read(startAddr + i);
        if (in[i] == '\0') break; // Stop if null terminator is found
        i++;
    }
    in[i] = '\0'; // Ensure the string is null-terminated
        String result(in);

    // Verificar si el string leído está vacío
    // if (result.length() == 0) {
    //     Serial.println("⚠️ Error: Cadena vacía leída desde EEPROM");
    // } else {
    //     Serial.println("📡 String leído desde EEPROM: " + result);
    // }

    return result;
}