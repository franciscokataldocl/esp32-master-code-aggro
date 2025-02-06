#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <Arduino.h>

class WiFiManager {
public:
    static void initWiFi();
    static void handleWiFi();
    static bool isInternetAvailable();
    static void reconnectWiFi();
    static void printWifiData();
};

#endif
