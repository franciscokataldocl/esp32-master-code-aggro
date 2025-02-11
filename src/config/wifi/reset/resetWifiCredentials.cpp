#include "resetWifiCredentials.h"
#include "../../eeprom/eepRomUtils.h" // Asegúrate de incluir el archivo donde esté la lógica de EEPROM
#include <EEPROM.h>

#define EEPROM_SIZE 200  // Tamaño de EEPROM, asegúrate de que sea suficiente

namespace {
    int WiFi_rst;  // Variable para almacenar el pin del botón
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
        if (millis() - rst_millis >= 3000) { 
            Serial.println("\n🔄 Eliminando credenciales WiFi...");
            
            // Borrar credenciales sobrescribiendo los mismos espacios de memoria
            EEPROMUtils::writeStringToFlash("", 0);   // Borrar SSID
            EEPROMUtils::writeStringToFlash("", 40);  // Borrar contraseña
            
            Serial.println("✅ Credenciales eliminadas. Reiniciando dispositivo...");
            delay(500);
            ESP.restart();
        }
    } else { // Si el botón no está presionado, reiniciar la lógica
        buttonPressed = false;
    }
}
