#ifndef SOILMOISTURESENSOR_H
#define SOILMOISTURESENSOR_H

#include <Arduino.h>

class SoilMoistureSensor {
private:
    int pin;        // Pin del sensor
    int dryValue;   // Valor RAW en suelo seco
    int wetValue;   // Valor RAW en suelo húmedo
    bool invert;    // Si el sensor está invertido o no
    unsigned long lastReadTime;  // 📌 Guarda el último tiempo de lectura


public:
    SoilMoistureSensor(int pin, int dryValue, int wetValue, bool invert = false);

    int readHumidity();  // Devuelve la humedad en %
    float readVoltage(); // Devuelve el voltaje leído
    void printValues();  // 📌 Nuevo método para imprimir los valores
    void update(unsigned long interval); // 📌 Nueva función para controlar el tiempo de lectura


};

#endif
