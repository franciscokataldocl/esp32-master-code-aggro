#include "wifiManager.h"
#include "../eeprom/eepRomUtils.h"
#include <WiFi.h>
#include <HTTPClient.h>

#define WiFi_rst 0
unsigned long lastCheckTime = 0;
const unsigned long checkInterval = 30 * 60 * 1000;
const unsigned long retryInterval = 3 * 60 * 1000;

void WiFiManager::initWiFi() {
    pinMode(WiFi_rst, INPUT);
    
    String ssid = EEPROMUtils::readStringFromFlash(0);
    String pss = EEPROMUtils::readStringFromFlash(40);
    
    WiFi.begin(ssid.c_str(), pss.c_str());
    delay(3500);

    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("❌ No WiFi, using SmartConfig...");
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
    Serial.println("\n ✅ WiFi Connected!");
    Serial.println(WiFi.SSID());
    Serial.println("IP Address: ");
    Serial.println(WiFi.localIP());
}