#include "sensors.h"
#include <vector>
#include "../common/microsd/microSd.h"
#include "../config/config.h"

// Instancias globales de sensores
SoilMoistureSensor soilSensor(34, 4095, 1200, false);
SensorColor colorSensor;

extern MicroSD sdCard;

unsigned long lastSensorTime = 0;
const unsigned long sensorInterval = 10000;

std::vector<std::function<void()>> sensorSequence;
size_t currentSensorIndex = 0;

void initSensors() {
    colorSensor.begin();

    // 🔁 Calibración automática desde archivo
    String data = sdCard.readFromFile(CALIBRATION_FILE_PATH);
    
    if (data == "") {
        Serial.println("⚠️ No hay datos de calibración. Calibrando...");
        colorSensor.calibrate();

        // Valores simulados, reemplaza con valores reales si quieres
        int minR = 123, minG = 100, minB = 140;
        int maxR = 400, maxG = 380, maxB = 390;

        String calibData = String(minR) + "," + String(minG) + "," + String(minB) + "," +
                           String(maxR) + "," + String(maxG) + "," + String(maxB);

        sdCard.writeToFile(CALIBRATION_FILE_PATH, calibData);
        colorSensor.setCalibration(minR, minG, minB, maxR, maxG, maxB);
    } else {
        int values[6];
        int index = 0;
        char tempData[data.length() + 1];
        data.toCharArray(tempData, data.length() + 1);

        char *token = strtok(tempData, ",");
        while (token != NULL && index < 6) {
            values[index++] = atoi(token);
            token = strtok(NULL, ",");
        }

        if (index == 6) {
            colorSensor.setCalibration(values[0], values[1], values[2],
                                       values[3], values[4], values[5]);
            Serial.println("✅ Datos de calibración cargados correctamente.");
        } else {
            Serial.println("⚠️ Error en formato de calibración. Recalibrando...");
            colorSensor.calibrate();
        }
    }

    // 🧠 Lógica secuencial de sensores
    sensorSequence.push_back([]() {
        Serial.println("🌱 Ejecutando sensor de humedad...");
        soilSensor.printValues();
    });

    sensorSequence.push_back([]() {
        int r, g, b;
        colorSensor.getRGB(r, g, b);
        Serial.printf("🎨 Sensor color RGB: %d, %d, %d\n", r, g, b);
    });
}

void updateSensors() {
    Serial.println("🚨 Ejecutando updateSensors()...");

    for (auto& sensorFunc : sensorSequence) {
        sensorFunc();  // Ejecuta cada sensor en orden
    }

    Serial.println("✅ Sensores ejecutados correctamente.");
}
