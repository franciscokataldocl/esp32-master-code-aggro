#include "sensors.h"

// Instancias globales de los sensores
SoilMoistureSensor soilSensor(34, 4095, 1200, false);
SensorColor colorSensor;

void initSensors() {
    colorSensor.begin();
    // Aquí puedes inicializar otros sensores si fuera necesario
}

void updateSensors(unsigned long interval) {
    soilSensor.update(interval);      // Imprime humedad con delay no bloqueante
    colorSensor.print();              // Imprime color cada 10s (controlado internamente)
}
