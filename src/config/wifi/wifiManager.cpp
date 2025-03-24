#include "wifiManager.h"
#include "../eeprom/eepRomUtils.h"
#include "internet/connectionLogger/connectionLogger.h"
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

#include "../../sensors/wifi/wifiLedControl.h"
#include "config/config.h"
#include "../../setup/setup_helpers.h"
#include "common/microsd/microSd.h"

extern MicroSD sdCard;
extern ConnectionLogger logger;


 unsigned long lastCheckTime = 0;
unsigned long lastRetryTime = 0;
bool haveInternet = false;
bool loggedNoInternet = false;
unsigned long lastInternetCheck = 0;
int reconnectCounter = 0;

void WiFiManager::initWiFi(ConnectionLogger &logger) { //✅ Corrige esta firma
    String ssid = "";
    String pss = "";

    String credData = sdCard.readFromFile("/wifi_credentials.json");

    if (credData != "") {
        StaticJsonDocument<512> doc;
        DeserializationError error = deserializeJson(doc, credData);
        if (!error) {
            ssid = doc["ssid"].as<String>();
            pss = doc["password"].as<String>();
        } else {
            Serial.println("⚠️ Error al leer credenciales desde SD");
        }
    } else {
        Serial.println("⚠️ No se encontró el archivo de credenciales en SD");
    }

    WiFi.begin(ssid.c_str(), pss.c_str());

    unsigned long startWait = millis();
    while (millis() - startWait < 5000) {
        // Permite que otras tareas se ejecuten
    }

    if (WiFi.status() == WL_CONNECTED) {
        haveInternet = true;
        initTime();  // 👈 Llamar explícitamente aquí

        WifiLedControl::turnOnLed(false);

        logger.logEvent("ONLINE");  // ✅ ahora logger es accesible
    } else {
        startSmartConfig();
    }

    WiFiManager::printWifiData();
}


void WiFiManager::handleWiFi() {
    if (WiFi.status() != WL_CONNECTED) {
        WifiLedControl::turnOnLed(true);
        if (millis() - lastRetryTime >= RETRY_INTERVAL) {
            lastRetryTime = millis();
            Serial.println("❌ No Internet, retrying...");
            reconnectWiFi();
        }
    } else if (millis() - lastCheckTime >= CHECK_INTERVAL) {
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
        DynamicJsonDocument doc(512);

        doc["ssid"] = WiFi.SSID();
        doc["password"] = WiFi.psk();
        
        String json;
        serializeJson(doc, json);
        sdCard.writeToFile("/wifi_credentials.json", json);
        
        Serial.println("✅ WiFi guardado en SD (/wifi_credentials.json)");
        
        initTime(); // ⏱️ sincroniza hora tras conexión
        logger.logEvent("ONLINE");  // ✅ AGREGA ESTA LÍNEA
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

void WiFiManager::handleInternetCheck(ConnectionLogger &logger) {
    if (millis() - lastInternetCheck < 1000) return; // comprobar cada segundo la lógica interna
    lastInternetCheck = millis();

    bool internetStatus = isInternetAvailable();
    static bool previousInternetStatus = true;

    if (internetStatus) {
        if (!previousInternetStatus) {
            Serial.println("✅ Internet restaurado.");
            logger.logEvent("ONLINE"); // ✅ Solo eventos ONLINE
        }
        previousInternetStatus = true;
        reconnectCounter = 0;
    } else {
        if (previousInternetStatus || reconnectCounter == 0) {
            Serial.println("❌ Internet perdido.");
        }
        previousInternetStatus = false;
        reconnectCounter++;

        if (reconnectCounter <= 3) {
            delay(60000); // Espera 1 minuto primeros intentos
        } else {
            delay(300000); // Espera 5 minutos después
        }
    }
}
