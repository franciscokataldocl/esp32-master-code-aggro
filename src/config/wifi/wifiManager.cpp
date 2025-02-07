#include "wifiManager.h"
#include "../eeprom/eepRomUtils.h"
#include <WiFi.h>
#include <HTTPClient.h>

#define WiFi_rst 0
unsigned long lastCheckTime = 0;
const unsigned long checkInterval = 30 * 60 * 1000;
const unsigned long retryInterval = 3 * 60 * 1000;
unsigned long rst_millis = 0;


void WiFiManager::initWiFi() {
    pinMode(WiFi_rst, INPUT);
    
    String ssid = EEPROMUtils::readStringFromFlash(0);
    String pss = EEPROMUtils::readStringFromFlash(40);
//     Serial.print("Leyendo SSID desde EEPROM: ");
// Serial.println(ssid);
// Serial.print("Leyendo Password desde EEPROM: ");
// Serial.println(pss);
    WiFi.begin(ssid.c_str(), pss.c_str());
    delay(5000);

    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("\n");
        Serial.println("❌ No WiFi, using SmartConfig...\n");
        WiFi.mode(WIFI_AP_STA);
        WiFi.beginSmartConfig();
        
        while (!WiFi.smartConfigDone()) {
            delay(500);
            Serial.print(".");
        }

        while (WiFi.status() != WL_CONNECTED) {
            delay(500);
            Serial.print(".");
        }

        WiFiManager::printWifiData();

        EEPROMUtils::writeStringToFlash(WiFi.SSID().c_str(), 0);
        EEPROMUtils::writeStringToFlash(WiFi.psk().c_str(), 40);
//         Serial.print("Guardando SSID: ");
// Serial.println(WiFi.SSID());
// Serial.print("Guardando Password: ");
// Serial.println(WiFi.psk());
    } else {
        WiFiManager::printWifiData();

    }
}

void WiFiManager::handleWiFi() {
    static unsigned long lastRetryTime = 0;

    if (WiFi.status() != WL_CONNECTED) {
        if (millis() - lastRetryTime >= retryInterval) {
            lastRetryTime = millis();
            Serial.println("❌ No Internet, retrying...");
            reconnectWiFi();
        }
    } else {
        if (millis() - lastCheckTime >= checkInterval) {
            lastCheckTime = millis();
            if (!isInternetAvailable()) {
                Serial.println("❌ Internet lost, switching to retry mode...");
                lastRetryTime = millis();
            } else {
                Serial.println("✅ Internet is available!");
            }
        }
    }
}

bool WiFiManager::isInternetAvailable() {
    if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;
        http.begin("http://clients3.google.com/generate_204");
        int httpCode = http.GET();
        http.end();
        return (httpCode == 204);
    }
    return false;
}

void WiFiManager::reconnectWiFi() {
    WiFi.disconnect();
    WiFi.reconnect();
    delay(5000);

    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("✅ Reconnected to WiFi!");
        lastCheckTime = millis();
    } else {
        Serial.println("❌ Failed to reconnect.");
    }
}

void WiFiManager::printWifiData()  { 
    Serial.println("\n ✅ WiFi Connected!........................................................................");
    Serial.println("Nombre red: " + WiFi.SSID());
    Serial.println("Ip: " + WiFi.localIP().toString()); 
    Serial.println("............................................................................................\n ");
}

void WiFiManager::resetWiFiCredentials() {
    if (digitalRead(WiFi_rst) == LOW) {  // Asegúrate de que el botón esté presionado
        if (millis() - rst_millis >= 3000) {  // Mantén presionado por 3 segundos
        Serial.println("\n 🔫 Eliminando credenciales wifi....................................................");
            Serial.println("");
            EEPROMUtils::writeStringToFlash("", 0);  // Borrar SSID
            EEPROMUtils::writeStringToFlash("", 40); // Borrar la contraseña
            Serial.println("Credenciales Wifi eliminadas");
            Serial.println("Reiniciando dispositivo");
             Serial.println(".................................................................................\n");
            delay(500);
            ESP.restart(); // Reiniciar el ESP
        }
    } else {
        rst_millis = millis(); // Resetear el tiempo si el botón no está presionado
    }
}