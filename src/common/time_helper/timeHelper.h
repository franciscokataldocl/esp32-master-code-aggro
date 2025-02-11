#ifndef TIMEHELPER_H
#define TIMEHELPER_H

#include <NTPClient.h>
#include <WiFiUdp.h>

// Inicialización del cliente NTP
extern WiFiUDP ntpUDP;
extern NTPClient timeClient;

// Función para inicializar el cliente NTP
void initializeTimeClient();

// Función para obtener la fecha y hora actual
String getCurrentTime();

#endif
