#ifndef WIFI_LED_CONTROL_H
#define WIFI_LED_CONTROL_H

#include <Arduino.h>

class WifiLedControl {
public:
    static void turnOnLed(bool internet);
};

#endif
