#include "wifiConnectionLogger.h"
#include <EEPROM.h>
#include <WiFi.h>
#include <HTTPClient.h>  // Agrega la librería HTTPClient
#include <Arduino_JSON.h> // Agrega la librería Arduino_JSON
#include "config/device_id/device_id.h"
#include "common/time_helper/timeHelper.h"

void ConnectionLogger::logConnectionEvent() {
    Serial.println("✅ Conectado a Internet. Registrando evento...");
        
    // Llamar a saveOnlineLog para enviar los datos cuando haya conexión a Internet
    sendWifiLog();
}

void ConnectionLogger::sendWifiLog() {
    // Verificar si hay conexión a internet
    if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;
        String onlineData = getCurrentTime(); // Fecha de recuperación de internet

        // Crear el objeto JSON
        JSONVar doc;

        // Asignar valores al objeto JSON
        doc["esp32Id"] = DeviceID::getUniqueID();

        // Crear el objeto anidado 'data'
        JSONVar onlineEvent;
        onlineEvent["event_type"] = "ONLINE";
        onlineEvent["event_time"] = onlineData;

        // Asignar el objeto anidado 'data' al objeto principal
        doc["data"] = onlineEvent;

        // Convertir el objeto JSON a una cadena
        String jsonString = JSON.stringify(doc);

        // Imprimir por consola lo que se enviará
        Serial.println("Enviando los siguientes datos al servidor:");
        Serial.println(jsonString);

        // Enviar el POST a la ruta /wifistatus
        http.begin("http://localhost:8080/wifistatus");
        http.addHeader("Content-Type", "application/json");
        int httpResponseCode = http.POST(jsonString);

        if (httpResponseCode == 200) {
            Serial.println("Datos de conexion wifi enviados al servidor");
        } else {
            Serial.print("Error al enviar datos: ");
            Serial.println(httpResponseCode);
        }

        http.end();
    } else {
        Serial.print("No hay conexion a internet para enviar datos de wifi ");
    }
}
