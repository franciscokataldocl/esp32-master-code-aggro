#include "resetWifiCredentials.h"
#include <WiFi.h>
#include "config/config.h"
#include "common/microsd/microSd.h"

extern MicroSD sdCard;

namespace {
    int WiFi_rst;
    unsigned long rst_millis = 0;
    bool buttonPressed = false;
}

void ResetWifiCredentials::begin(int pin) {
    WiFi_rst = pin;
    pinMode(WiFi_rst, INPUT_PULLUP);
}

void ResetWifiCredentials::checkReset() {
    if (digitalRead(WiFi_rst) == LOW) {
        if (!buttonPressed) {
            rst_millis = millis();
            buttonPressed = true;
        }

        if (millis() - rst_millis >= WIFI_RESET_BUTTON_PRESSED_TIME) {
            Serial.println("\n🔄 Eliminando credenciales WiFi...");

            // ✅ BORRAR ARCHIVO DE LA SD
            bool deleted = sdCard.deleteFile(WIFI_CREDENTIALS_PATH);
            if (deleted) {
                Serial.println("🧹 Credenciales WiFi eliminadas de SD.");
            } else {
                Serial.println("⚠️ No se pudieron eliminar las credenciales desde SD.");
            }

            // ✅ También borra las de NVS
            WiFi.disconnect(true, true);
            WiFi.mode(WIFI_OFF);

            delay(1000);
            Serial.println("✅ Reiniciando dispositivo...");
            delay(500);
            ESP.restart();
        }
    } else {
        buttonPressed = false;
    }
}
