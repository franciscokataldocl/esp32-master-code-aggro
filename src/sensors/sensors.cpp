#include "sensors.h"
#include <vector>

SoilMoistureSensor soilSensor(34, 4095, 1200, false);
SensorColor colorSensor;

unsigned long lastSensorTime = 0;
const unsigned long sensorInterval = 10000;  // tiempo entre sensores 10 segundos

// Vector de funciones (orden = orden de ejecución)
std::vector<std::function<void()>> sensorSequence;
size_t currentSensorIndex = 0;

void initSensors() {
    colorSensor.begin();  // 👉 Esto inicializa el sensor de color

    // ORDEN DE EJECUCIÓN
    sensorSequence.push_back([]() {
        Serial.println("🌱 Ejecutando sensor de humedad...");
        soilSensor.printValues();  // 💧
    });

    sensorSequence.push_back([]() {
        Serial.println("🎨 Ejecutando sensor de color...");
        int r, g, b;
        colorSensor.getRGB(r, g, b);
        Serial.printf("🎨 Sensor color RGB: %d, %d, %d\n", r, g, b);
    });
}

void updateSensors() {
    unsigned long now = millis();
    if (now - lastSensorTime >= sensorInterval) {
        lastSensorTime = now;

        if (currentSensorIndex < sensorSequence.size()) {
            sensorSequence[currentSensorIndex]();  // ejecuta el sensor actual
            currentSensorIndex++;
        }

        // Reinicia al llegar al final
        if (currentSensorIndex >= sensorSequence.size()) {
            currentSensorIndex = 0;
        }
    }
}
