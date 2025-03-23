#include "eepRomUtils.h"
#include <EEPROM.h>
#include "config/config.h"  // Incluir archivo de configuración

void EEPROMUtils::writeStringToFlash(const char* toStore, int startAddr) {
    int len = strlen(toStore);

    // Verificar si el SSID es demasiado largo
    if (startAddr == SSID_ADDRESS && len > 32) { // Usar la constante SSID_ADDRESS
        Serial.println("❌ Error: El SSID es demasiado largo");
        return;
    }
    // Verificar si la contraseña es demasiado larga
    if (startAddr == PASSWORD_ADDRESS && len > 64) { // Usar la constante PASSWORD_ADDRESS
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

    // Leer hasta encontrar un carácter nulo
    while (i < 128) {
        in[i] = EEPROM.read(startAddr + i);
        if (in[i] == '\0') break; // Detener si se encuentra un terminador nulo
        i++;
    }
    in[i] = '\0'; // Asegurarse de que la cadena esté terminada en nulo
    String result(in);

    return result;
}
