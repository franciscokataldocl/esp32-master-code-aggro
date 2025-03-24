#include "task_manager.h"          // 👈 Esto primero SIEMPRE
#include "setup/setup_helpers.h"
#include "config/wifi/wifiManager.h"
#include "common/microsd/microSd.h"
#include "config/wifi/internet/connectionLogger/connectionLogger.h"
#include "sensors/color-sensor/sensor-color.h"
#include "config/wifi/reset/resetWifiCredentials.h"
#include "sensors/sensors.h"

#define DEBUG_MODE true

extern MicroSD sdCard;
extern ConnectionLogger logger;
extern SensorColor colorSensor;



const unsigned long SENSOR_INTERVAL = 1800000;  // 30 minutos
const unsigned long INITIAL_DELAY = DEBUG_MODE ? 5000 : 60000;  // 5 seg en debug, 1 min en prod

unsigned long lastSensorRead = millis() - (SENSOR_INTERVAL - INITIAL_DELAY);

void handleTasks() {
    ResetWifiCredentials::checkReset();  // 👈 este va al inicio
    WiFiManager::handleWiFi();
    WiFiManager::handleInternetCheck(logger);
    handlePeriodicTasks();
}

void handlePeriodicTasks() {
    if (millis() - lastSensorRead >= SENSOR_INTERVAL) {
        lastSensorRead = millis();

        updateSensors();
  // actualiza todos los sensores
        printStoredEvents();   // imprime eventos almacenados
    }
}

void readSensorsAndSendMQTT() {
    int r, g, b;
    colorSensor.getRGB(r, g, b);
    Serial.print("🎨 Color leído RGB: ");
    Serial.print(r); Serial.print(", "); Serial.print(g); Serial.print(", "); Serial.println(b);

    String logs = sdCard.readFromFile(EVENTS_LOG_PATH);
    if (logs != "") {
        Serial.println("Enviando datos por MQTT: " + logs);
        sdCard.deleteFile(EVENTS_LOG_PATH);
    }
}

void printStoredEvents() {
    Serial.println("📄 Eventos actuales:");
    String data = sdCard.readFromFile(EVENTS_LOG_PATH);

    if (data.length() > 0) {
        Serial.println("Contenido del archivo:");
        Serial.println(data);  // 👈 claramente aquí imprimes el contenido
    } else {
        Serial.println("⚠️ Sin eventos almacenados o archivo vacío.");
    }
}

