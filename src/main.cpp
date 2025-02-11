#include <Arduino.h>
#include "config/wifi/wifiManager.h"
#include "config/wifi/reset/resetWifiCredentials.h"
#include <EEPROM.h>


// Definir el tamaño de la EEPROM (en bytes)
#define EEPROM_SIZE 200
#define WIFI_RESET_PIN 22


void setup() {
    Serial.begin(115200);
 
    EEPROM.begin(EEPROM_SIZE);  // Inicializar EEPROM con el tamaño especificado
    ResetWifiCredentials::begin(WIFI_RESET_PIN);
   WiFiManager::initWiFi();  // Inicializar WiFi

    

    
}

void loop() {
    ResetWifiCredentials::checkReset();
    WiFiManager::handleWiFi();  // Manejar el WiFi en el loop

}