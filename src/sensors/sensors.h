#ifndef SENSORS_H
#define SENSORS_H

// Incluye headers de sensores individuales
#include "humidity-SZHJW/SoilMoistureSensor.h"
#include "color-sensor/sensor-color.h"

// Inicializa todos los sensores disponibles
void initSensors();

// Actualiza los valores de todos los sensores según un intervalo
void updateSensors();


#endif
