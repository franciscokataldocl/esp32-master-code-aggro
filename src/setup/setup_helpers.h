#ifndef SETUP_HELPERS_H
#define SETUP_HELPERS_H

#include <Arduino.h>
#include "common/microsd/microSd.h"
#include "config/wifi/internet/connectionLogger/connectionLogger.h"
#include "sensors/sensors.h"


// Extern aquí claramente:
extern MicroSD sdCard;
extern ConnectionLogger logger;
extern SensorColor colorSensor;

void initSystem();
void initSensorColorCalibration();
void initTime();  
void initSensors(); // ✅ Inicializa sensores unificados



#endif
