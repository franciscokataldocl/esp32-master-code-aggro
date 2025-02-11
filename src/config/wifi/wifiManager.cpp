#include "wifiManager.h"
#include "../eeprom/eepRomUtils.h"
#include "../wifi/logger/wifiConnectionLogger.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include "../../sensors/wifi/wifiLedControl.h"

// Variables globales
bool haveInternet = false;
bool loggedNoInternet = false;

unsigned long lastCheckTime = 0;
const unsigned long checkInterval = 10 * 1000;  // Verifica cada 10 segundos
unsigned long lastRetryTime = 0;
const unsigned long retryInterval = 5 * 1000;   // Reintenta conectar cada 5 segundos

void WiFiManager::initWiFi() {
    String ssid = EEPROMUtils::readStringFromFlash(0);
    String pss = EEPROMUtils::readStringFromFlash(40);

    WiFi.begin(ssid.c_str(), pss.c_str());
    
    unsigned long startWait = millis();
    while (millis() - startWait < 5000) {
        // Permite que otras tareas se ejecuten
    }

    if (WiFi.status() == WL_CONNECTED) {
        haveInternet = true;
        WifiLedControl::turnOnLed(false);
    } else {
        startSmartConfig();
    }
    ConnectionLogger::logConnectionEvent();
    WiFiManager::printWifiData();
}

void WiFiManager::handleWiFi() {
    if (WiFi.status() != WL_CONNECTED) {
        WifiLedControl::turnOnLed(true);
        if (millis() - lastRetryTime >= retryInterval) {
            lastRetryTime = millis();
            Serial.println("❌ No Internet, retrying...");
            reconnectWiFi();
        }
    } else if (millis() - lastCheckTime >= checkInterval) {
        lastCheckTime = millis();
        checkInternetStatus();
    }
}

void WiFiManager::startSmartConfig() {
    Serial.println("\n❌ No WiFi, usando SmartConfig...\n");
    WifiLedControl::turnOnLed(true);
    
    WiFi.mode(WIFI_AP_STA);
    WiFi.beginSmartConfig();

    unsigned long startTime = millis();
    while (!WiFi.smartConfigDone() && millis() - startTime < 30000) {
        // Espera sin bloquear
    }

    if (!WiFi.smartConfigDone()) {
        Serial.println("⏳ SmartConfig falló, abortando...");
        return;
    }

    startTime = millis();
    while (WiFi.status() != WL_CONNECTED && millis() - startTime < 30000) {
        // Espera sin bloquear
    }

    if (WiFi.status() == WL_CONNECTED) {
        EEPROMUtils::writeStringToFlash(WiFi.SSID().c_str(), 0);
        EEPROMUtils::writeStringToFlash(WiFi.psk().c_str(), 40);
        Serial.println("✅ WiFi guardado en EEPROM");
    } else {
        Serial.println("⏳ No se pudo conectar con SmartConfig.");
    }
}

void WiFiManager::checkInternetStatus() {
    bool internetStatus = isInternetAvailable();

    if (internetStatus) {
        if (!haveInternet) {
            Serial.println("✅ Internet restored!");
            WifiLedControl::turnOnLed(false);
            ConnectionLogger::logConnectionEvent();
        }
        haveInternet = true;
        loggedNoInternet = false;
    } else {
        Serial.println("❌ Internet lost, switching to retry mode...");
        lastRetryTime = millis();
        haveInternet = false;
        WifiLedControl::turnOnLed(true);

        if (!loggedNoInternet) {
            loggedNoInternet = true;
        }
    }
}

bool WiFiManager::isInternetAvailable() {
    if (WiFi.status() != WL_CONNECTED) return false;

    WiFiClient client;
    if (!client.connect("clients3.google.com", 80)) {
        return false;
    }

    client.print(String("GET /generate_204 HTTP/1.1\r\n") +
                 "Host: clients3.google.com\r\n" +
                 "Connection: close\r\n\r\n");

    unsigned long startTime = millis();
    while (!client.available() && millis() - startTime < 3000) {
        // Espera sin bloquear
    }

    if (!client.available()) return false;

    String response = client.readString();
    return response.indexOf("204 No Content") != -1;
}

void WiFiManager::reconnectWiFi() {
    static unsigned long startAttemptTime = 0;

    if (WiFi.status() != WL_CONNECTED) {
        if (millis() - startAttemptTime > 5000) {
            startAttemptTime = millis();
            Serial.println("🔄 Reintentando conexión WiFi...");
            WiFi.disconnect();
            WiFi.reconnect();
        }
    }
}

void WiFiManager::printWifiData() {
    Serial.println("\n✅ WiFi Connected!");
    Serial.println("Nombre red: " + WiFi.SSID());
    Serial.println("IP: " + WiFi.localIP().toString());
}
