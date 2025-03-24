#include "connectionLogger.h"
#include <ArduinoJson.h>
#include <time.h>

ConnectionLogger::ConnectionLogger(MicroSD& sd) : _sd(sd) {}

String ConnectionLogger::getCurrentTimestamp() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
      Serial.println("⚠️ getCurrentTimestamp() - No hay hora NTP");
      return "1970-01-01T00:00:00";
  }

  char buffer[25];
  strftime(buffer, sizeof(buffer), "%Y-%m-%dT%H:%M:%S", &timeinfo);
  return String(buffer);
}


void ConnectionLogger::logEvent(const String& status) {
  String fileContent = _sd.readFromFile(filename);
  DynamicJsonDocument doc(2048);


  if (fileContent.length() > 0) {
    deserializeJson(doc, fileContent);
  } else {
    doc["events"] = JsonArray();
  }

  JsonObject newEvent = doc["events"].createNestedObject();
  newEvent["timestamp"] = getCurrentTimestamp();
  newEvent["status"] = status;

  String output;
  serializeJson(doc, output);
  _sd.writeToFile(filename, output);
}

String getFormattedTime(){
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
      Serial.println("⚠️ Fecha inválida, verifica conexión NTP");
      return "1970-01-01T00:00:00"; 
  }

  char buffer[25];
  strftime(buffer, sizeof(buffer), "%Y-%m-%dT%H:%M:%S", &timeinfo);
  return String(buffer);
}