#include "task_manager.h"          // 👈 Esto primero SIEMPRE
#include "setup/setup_helpers.h"
#include "config/wifi/wifiManager.h"
#include "common/microsd/microSd.h"
#include "config/wifi/internet/connectionLogger/connectionLogger.h"
#include "sensors/color-sensor/sensor-color.h"
#include "config/wifi/reset/resetWifiCredentials.h"
#include "sensors/sensors.h"



extern MicroSD sdCard;
extern ConnectionLogger logger;
extern SensorColor colorSensor;

unsigned long taskStartTime = 0;
bool taskTimerStarted = false;
bool firstSensorRunDone = false;

const unsigned long SENSOR_INTERVAL = 1800000;  // 30 minutos
const unsigned long INITIAL_DELAY = DEBUG_MODE ? 5000 : 60000;  // 1 minuto normal, 5 seg debug

unsigned long lastSensorRead = 0;


void handleTasks() {
    ResetWifiCredentials::checkReset();  // 👈 este va al inicio
    WiFiManager::handleWiFi();
    WiFiManager::handleInternetCheck(logger);
    handlePeriodicTasks();
}

void handlePeriodicTasks() {
    unsigned long now = millis();

    if (!taskTimerStarted) {
        taskStartTime = now;
        taskTimerStarted = true;
        return;
    }

    if (!firstSensorRunDone) {
        if (now - taskStartTime >= INITIAL_DELAY) {
            Serial.printf("✅ Espera inicial superada (%lu ms)\n", now - taskStartTime);
            updateSensors();
            printStoredEvents();
            firstSensorRunDone = true;
            lastSensorRead = now;
        }
        return;
    }

    if (now - lastSensorRead >= SENSOR_INTERVAL) {
        lastSensorRead = now;
        updateSensors();
        printStoredEvents();
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
    String data = sdCard.readFromFile(EVENTS_LOG_PATH);

    if (data.length() > 0) {
        Serial.println("📄 Eventos actuales:");
        Serial.println(data);  // 👈 Solo imprime si hay contenido
    } else {
        // No mostrar nada si no hay eventos
        // Si quieres dejar una advertencia, puedes descomentar esto:
        // Serial.println("⚠️ Sin eventos almacenados o archivo vacío.");
    }
}

