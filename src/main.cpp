#include <Arduino.h>
#include "config/wifi/wifiManager.h"

void setup() {
    Serial.begin(115200);
    WiFiManager::initWiFi();
}

void loop() {
    WiFiManager::handleWiFi();
}
