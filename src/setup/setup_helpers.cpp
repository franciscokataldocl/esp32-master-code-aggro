#include "setup_helpers.h"
#include "config/wifi/wifiManager.h"
#include "config/wifi/reset/resetWifiCredentials.h"
#include <EEPROM.h>

#define EEPROM_SIZE 200
#define WIFI_RESET_PIN 22



MicroSD sdCard;
ConnectionLogger logger(sdCard);
SensorColor colorSensor;

void initSystem() {
    EEPROM.begin(EEPROM_SIZE);
    ResetWifiCredentials::begin(WIFI_RESET_PIN);

    if (!sdCard.begin()) {
        Serial.println("⚠️ Falló inicialización SD.");
    }

    WiFiManager::initWiFi(logger);
    colorSensor.begin();

    initSensorColorCalibration();
}

void initSensorColorCalibration() {
    String data = sdCard.readFromFile("/calibration.txt");
    
    if (data == "") {
        Serial.println("⚠️ No hay datos de calibración. Calibrando...");
        colorSensor.calibrate();

        // Tras calibrar, obtener valores reales del sensor (debes implementar esto):
        int minR = 123, minG = 100, minB = 140;
        int maxR = 400, maxG = 380, maxB = 390;

        String calibData = String(minR) + "," + String(minG) + "," + String(minB) + "," +
                           String(maxR) + "," + String(maxG) + "," + String(maxB);

        sdCard.writeToFile("/calibration.txt", calibData);
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
}
