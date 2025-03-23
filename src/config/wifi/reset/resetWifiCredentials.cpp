#include "resetWifiCredentials.h"
#include "../../eeprom/eepRomUtils.h" 
#include <EEPROM.h>
#include <WiFi.h>  // Asegúrate de incluir esta librería
#include "config/config.h"  

namespace {
    int WiFi_rst;  
    unsigned long rst_millis = 0;
    bool buttonPressed = false;
}

void ResetWifiCredentials::begin(int pin) {
    WiFi_rst = pin;
    pinMode(WiFi_rst, INPUT_PULLUP); // Configurar el botón con pull-up
}

void ResetWifiCredentials::checkReset() {
    if (digitalRead(WiFi_rst) == LOW) { // Botón presionado
        if (!buttonPressed) { 
            rst_millis = millis(); // Guardar tiempo de inicio
            buttonPressed = true;
        }
        // Si han pasado más de 3 segundos
        if (millis() - rst_millis >= WIFI_RESET_BUTTON_PRESSED_TIME) { 
            Serial.println("\n🔄 Eliminando credenciales WiFi...");
            
            // 1️⃣ Borrar credenciales de EEPROM
            EEPROMUtils::writeStringToFlash("", SSID_ADDRESS);
            EEPROMUtils::writeStringToFlash("", PASSWORD_ADDRESS);

            // 2️⃣ Borrar credenciales de WiFi NVS
            Serial.println("🔥 Eliminando credenciales de NVS...");
            WiFi.disconnect(true, true);  // Borra credenciales y desconecta
            WiFi.mode(WIFI_OFF); // Apaga el WiFi para asegurar el borrado

            delay(1000);  // Espera 1 segundo para aplicar los cambios

            Serial.println("✅ Credenciales eliminadas. Reiniciando dispositivo...");
            delay(500);
            ESP.restart(); // Reinicia el ESP32
        }
    } else { 
        buttonPressed = false;
    }
}
