#include "setup_helpers.h"
#include "config/wifi/wifiManager.h"
#include "config/wifi/reset/resetWifiCredentials.h"
#include "time.h"
#include "sensors/color-sensor/sensor-color.h"
#include "sensors/sensors.h"  // Incluye la cabecera donde está extern




#define WIFI_RESET_PIN 22

const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = -3 * 3600;   // Chile GMT-3
const int daylightOffset_sec = 0;


MicroSD sdCard;
ConnectionLogger logger(sdCard);

extern unsigned long bootTime; 

void initSystem() {
    ResetWifiCredentials::begin(WIFI_RESET_PIN);

    if (!sdCard.begin()) {
        Serial.println("⚠️ Falló inicialización SD.");
        while(true);
    } else {
        Serial.println("✅ SD inicializada correctamente");
    }

    String contenido = sdCard.readFromFile(EVENTS_LOG_PATH);
    if (contenido != "") {
     
    } else {
        Serial.println("⚠️ Archivo vacío o error en lectura.");
    }

    WiFiManager::initWiFi(logger);
    initSensors();
}

void initSensorColorCalibration() {
    String data = sdCard.readFromFile(CALIBRATION_FILE_PATH);
    
    if (data == "") {
        Serial.println("⚠️ No hay datos de calibración. Calibrando...");
        colorSensor.calibrate();

        // Tras calibrar, obtener valores reales del sensor (debes implementar esto):
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
}


void initTime() {
    Serial.println("⏳ Intentando sincronizar hora NTP...");
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

    struct tm timeinfo;
    int retry = 0;
    const int retry_count = 10;

    while (!getLocalTime(&timeinfo) && retry < retry_count) {
        Serial.print(".");
        delay(1000);
        retry++;
    }

    if(retry == retry_count){
        Serial.println("\n❌ Falló sincronización NTP.");
    } else {
        Serial.println("\n✅ Tiempo sincronizado:");
        Serial.println(&timeinfo, "%Y-%m-%dT%H:%M:%S");
    }
}