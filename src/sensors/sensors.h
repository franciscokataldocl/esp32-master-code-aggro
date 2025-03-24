#ifndef SENSORS_H
#define SENSORS_H

#include "humidity-SZHJW/SoilMoistureSensor.h"
#include "color-sensor/sensor-color.h"

void initSensors();
void updateSensors(unsigned long interval);

#endif
