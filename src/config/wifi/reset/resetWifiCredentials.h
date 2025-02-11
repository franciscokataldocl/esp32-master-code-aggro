#ifndef RESET_WIFI_CREDENTIALS_H
#define RESET_WIFI_CREDENTIALS_H

#include <Arduino.h>

class ResetWifiCredentials {
public:
    static void begin(int pin);   // Inicializa el botón
    static void checkReset();     // Comprueba si el botón ha sido presionado
};

#endif
