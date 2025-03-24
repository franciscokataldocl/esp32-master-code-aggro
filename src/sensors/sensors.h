#ifndef SENSORS_H
#define SENSORS_H

// Incluye headers de sensores individuales
#include "humidity-SZHJW/SoilMoistureSensor.h"
#include "color-sensor/sensor-color.h"

extern SensorColor colorSensor;           // 👈 Aquí declaras su existencia
extern SoilMoistureSensor soilSensor;     // (si también estás usando este afuera)

// Inicializa todos los sensores disponibles
void initSensors();

// Actualiza los valores de todos los sensores según un intervalo
void updateSensors();


#endif
