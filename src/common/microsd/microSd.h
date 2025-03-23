#ifndef MICRO_SD_H
#define MICRO_SD_H

#include "config/config.h"  // Incluir configuración global
#include <Arduino.h>
#include <SD.h>

class MicroSD {
    public:
        MicroSD();  // Constructor sin parámetros
        bool begin();  // Inicializar SD
        bool writeToFile(const char* filename, const String &data);  // Escribir datos en la SD
        String readFromFile(const char* filename);  // Leer datos desde la SD
        bool deleteFile(const char* filename);  // Eliminar un archivo
    
    private:
    const int _csPin = MICROSD_PIN;  // Se inicializa en el constructor
    };
    
    #endif