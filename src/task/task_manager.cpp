#include "task_manager.h"          // 👈 Esto primero SIEMPRE
#include "setup/setup_helpers.h"
#include "config/wifi/wifiManager.h"
#include "common/microsd/microSd.h"
#include "config/wifi/internet/connectionLogger/connectionLogger.h"
#include "sensors/color-sensor/sensor-color.h"
#include "config/wifi/reset/resetWifiCredentials.h" 

unsigned long lastSensorRead = 0;

void handleTasks() {
    ResetWifiCredentials::checkReset();  // 👈 este va al inicio
    WiFiManager::handleWiFi();
    WiFiManager::handleInternetCheck(logger);
    handlePeriodicTasks();
}

void handlePeriodicTasks() {
    if (millis() - lastSensorRead >= 1800000) { // 30 minutos
        lastSensorRead = millis();

        readSensorsAndSendMQTT();
        printStoredEvents();
    }
}

void readSensorsAndSendMQTT() {
    int r, g, b;
    colorSensor.getRGB(r, g, b);
    Serial.print("🎨 Color leído RGB: ");
    Serial.print(r); Serial.print(", "); Serial.print(g); Serial.print(", "); Serial.println(b);

    String logs = sdCard.readFromFile("/events.json");
    if (logs != "") {
        Serial.println("Enviando datos por MQTT: " + logs);
        sdCard.deleteFile("/events.json");
    }
}

void printStoredEvents() {
    Serial.println("📄 Eventos actuales:");
    String data = sdCard.readFromFile("/events.json");

    if (data.length() > 0) {
        Serial.println("Contenido del archivo:");
        Serial.println(data);  // 👈 claramente aquí imprimes el contenido
    } else {
        Serial.println("⚠️ Sin eventos almacenados o archivo vacío.");
    }
}

