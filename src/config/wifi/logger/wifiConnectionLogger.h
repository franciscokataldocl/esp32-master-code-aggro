#ifndef WIFI_CONNECTION_LOGGER_H
#define WIFI_CONNECTION_LOGGER_H

#include <Arduino.h>

class ConnectionLogger {
public:
    static void logConnectionEvent();
    static void sendWifiLog();

};

#endif
