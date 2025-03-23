#include "device_id.h"
#include <WiFi.h>
#include   "config/config.h"

String DeviceID::getUniqueID() {
    uint8_t mac[6];
    WiFi.macAddress(mac);

    // Convertir la MAC en un string único en formato hexadecimal
    char uniqueID[13];  // 6 bytes * 2 caracteres + null terminator
    snprintf(uniqueID, sizeof(uniqueID), "%02X%02X%02X%02X%02X%02X",
             mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

    return String(ESP32_UNIQUE_ID_PART + String(uniqueID));
}
