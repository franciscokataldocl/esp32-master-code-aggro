#include "timeHelper.h"
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <time.h>

// Definir las variables externas
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", -3 * 3600);  // UTC-3 (Chile en horario de verano)

// Función para inicializar el cliente NTP
void initializeTimeClient() {
  timeClient.begin();  // Iniciar el cliente NTP
  while (!timeClient.update()) {  // Esperar hasta que se obtenga la hora
    Serial.println("Esperando la hora NTP...");
    delay(1000);
  }
  Serial.println("Hora NTP sincronizada.");
}

// Función para obtener la fecha y hora actual en el formato deseado "DD-MM-YYYYTHH:MM:SS"
String getCurrentTime() {
  timeClient.update();  // Asegúrate de que el cliente esté actualizado

  // Obtener la hora formateada
  String formattedTime = timeClient.getFormattedTime();  // "HH:MM:SS"

  // Obtener la fecha a partir del tiempo NTP
  time_t rawTime = timeClient.getEpochTime();
  struct tm* timeinfo = localtime(&rawTime);
  
  // Formatear la fecha como "DD-MM-YYYY"
  char dateStr[20];
  strftime(dateStr, sizeof(dateStr), "%d-%m-%Y", timeinfo);  // "06-02-2025"
  
  // Combinar fecha y hora en formato "DD-MM-YYYYTHH:MM:SS"
  String dateTime = String(dateStr) + "T" + formattedTime;  // "06-02-2025T13:00:00"

  Serial.print(dateTime);  // Imprimir la fecha y hora en consola
  return dateTime;  // Devolver la fecha y hora en el formato "DD-MM-YYYYTHH:MM:SS"
}
