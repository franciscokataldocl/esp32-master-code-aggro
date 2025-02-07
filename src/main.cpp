#include <Arduino.h>
#include "config/wifi/wifiManager.h"
#include <EEPROM.h>

// Definir el tamaño de la EEPROM (en bytes)
#define EEPROM_SIZE 200

void setup() {
    Serial.begin(115200);

    EEPROM.begin(EEPROM_SIZE);  // Inicializar EEPROM con el tamaño especificado
    WiFiManager::initWiFi();  // Inicializar WiFi
    
}

void loop() {
    WiFiManager::handleWiFi();  // Manejar el WiFi en el loop
    WiFiManager::resetWiFiCredentials(); // presiona boot button por 3 segundos y elimina las credenciales wifi
}
